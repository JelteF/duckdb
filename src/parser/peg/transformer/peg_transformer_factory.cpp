#include "duckdb/parser/peg/transformer/peg_transformer.hpp"
#include "duckdb/parser/peg/compiled_grammar.hpp"
#include "duckdb/parser/expression/star_expression.hpp"
#include "duckdb/common/enums/trigger_type.hpp"
#include "duckdb/common/query_location.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/parser/peg/matcher.hpp"
#include "duckdb/common/to_string.hpp"
#include "duckdb/parser/sql_statement.hpp"
#include "duckdb/parser/token_iterator.hpp"
#include "duckdb/parser/tableref/showref.hpp"
#include "duckdb/common/enums/date_part_specifier.hpp"
#include "duckdb/common/enums/merge_action_type.hpp"
#include "duckdb/common/enums/subquery_type.hpp"
#include "duckdb/common/exception/conversion_exception.hpp"
#include "duckdb/parser/expression/cast_expression.hpp"
#include "duckdb/parser/query_node/set_operation_node.hpp"
#include "duckdb/parser/statement/merge_into_statement.hpp"
#include "duckdb/parser/constraints/foreign_key_constraint.hpp"

namespace duckdb {

static unique_ptr<SQLStatement> ExtractAndTransformStatement(PEGTransformer &transformer,
                                                             const TokenIterator &token_iterator, ParseResult &stmt_pr,
                                                             optional_idx terminator_offset) {
	auto stmt = transformer.Transform<unique_ptr<SQLStatement>>(stmt_pr);

	if (!transformer.named_parameter_map.empty()) {
		stmt->named_param_map = transformer.named_parameter_map;
	}
	if (!transformer.pivot_entries.empty()) {
		stmt = transformer.CreatePivotStatement(std::move(stmt));
		// Unpacking discards the wrapper, so the parts keep their map; the wrapper carries it for introspection.
		stmt->named_param_map = transformer.named_parameter_map;
	}
	transformer.Clear();

	// Calculate location and length cleanly
	if (stmt_pr.offset.IsValid()) {
		auto start = stmt_pr.offset.GetIndex();
		idx_t end_index = terminator_offset.IsValid() ? terminator_offset.GetIndex() : token_iterator.EndOffset();
		stmt->stmt_location = QueryLocation(start, end_index - start);
	}

	return stmt;
}

unique_ptr<SQLStatement> PEGTransformerFactory::TransformTopLevelStatement(TokenIterator &token_iterator,
                                                                           ParserOptions &options,
                                                                           const CompiledGrammar &grammar) {
	if (!token_iterator.Current()) {
		return nullptr;
	}
	vector<MatcherSuggestion> suggestions;
	ParseResultAllocator parse_result_allocator;
	ParserPackratCache packrat_cache(token_iterator.Size(), grammar.PackratMatcherCount());
	idx_t max_token_index = token_iterator.Position();
	ArenaAllocator process_allocator(Allocator::DefaultAllocator());
	MatchContext match_context(suggestions, parse_result_allocator, process_allocator, max_token_index,
	                           MatchMode::BUILD_PARSE_RESULT, options.identifier_case_mode, &packrat_cache);
	MatchState state(token_iterator, match_context);
	auto match_result = grammar.TopLevelStatementMatcher().MatchParseResult(state);
	process_allocator.FreeAll();
	if (!match_result.IsSuccess()) {
		// syntax error — surface as a parser exception in the same shape as Transform()
		auto token_stream = token_iterator.ToString();
		idx_t error_token_idx = state.GetMaxTokenIndex();
		if (error_token_idx >= token_iterator.Size()) {
			error_token_idx = token_iterator.Size() - 1;
		}
		// Walk back past the EOI sentinel so the error message names a real token.
		if (error_token_idx > 0 &&
		    (token_iterator.GetToken(error_token_idx).type == TokenType::END_OF_INPUT ||
		     token_iterator.GetToken(error_token_idx).type == TokenType::END_OF_INPUT_AUTOCOMPLETE)) {
			error_token_idx--;
		}
		auto &error_token = token_iterator.GetToken(error_token_idx);
		auto error_message = "syntax error at or near \"" + error_token.text + "\"";
		throw ParserException::SyntaxError(token_stream, error_message,
		                                   QueryLocation(error_token.offset, error_token.length));
	}
	D_ASSERT(match_result.HasParseResult());

	// Advance the caller's cursor past the consumed tokens.
	token_iterator.SetPosition(state.token_iterator);

	// TopLevelStatement <- Statement? (';'+ / EndOfInput)
	//   child 0: Optional<Statement>
	//   child 1: bracket-wrapper list around Choice<';'+ | EndOfInput>
	auto &tls = match_result.GetParseResult()->Cast<ListParseResult>();
	auto &stmt_opt = tls.Child<OptionalParseResult>(0);
	if (!stmt_opt.HasResult()) {
		// separator-only or EOI-only TopLevelStatement — no statement to yield
		return nullptr;
	}
	auto &term_wrapper = tls.Child<ListParseResult>(1);
	auto &term_inner = term_wrapper.Child<ChoiceParseResult>(0).GetResult();
	optional_idx terminator_offset;
	if (term_inner.type != ParseResultType::END_OF_INPUT) {
		auto semi_children = term_inner.Cast<RepeatParseResult>().GetChildren();
		if (!semi_children.empty()) {
			terminator_offset = semi_children[0].get().offset;
		}
	}

	ArenaAllocator transformer_allocator(Allocator::DefaultAllocator());
	PEGTransformer transformer(transformer_allocator, token_iterator, options, grammar);

	return ExtractAndTransformStatement(transformer, token_iterator, stmt_opt.GetResult(), terminator_offset);
}

PEGTransformerFactory::PEGTransformerFactory(ParsedGrammar &grammar_p) : grammar(grammar_p) {
	for (auto &entry : GeneratedTransformFrameOps()) {
		grammar.SetGeneratedTransformOps(entry.first, *entry.second);
	}
	// rules whose transformer never asks for a child, so TransformStack runs them without a frame
	//===--------------------------------------------------------------------===//
	// START GENERATED CHILDLESS TRANSFORM RULES
	//===--------------------------------------------------------------------===//
	grammar.SetChildlessTransform("IdentifierDot");
	grammar.SetChildlessTransform("RenameAlter");
	grammar.SetChildlessTransform("ResetPartitionedBy");
	grammar.SetChildlessTransform("ResetSortedBy");
	grammar.SetChildlessTransform("DropDefault");
	grammar.SetChildlessTransform("DropNullability");
	grammar.SetChildlessTransform("SetNullability");
	grammar.SetChildlessTransform("AnalyzeVerbose");
	grammar.SetChildlessTransform("CheckpointForce");
	grammar.SetChildlessTransform("CommentTable");
	grammar.SetChildlessTransform("CommentSequence");
	grammar.SetChildlessTransform("CommentFunction");
	grammar.SetChildlessTransform("CommentMacroTable");
	grammar.SetChildlessTransform("CommentMacro");
	grammar.SetChildlessTransform("CommentView");
	grammar.SetChildlessTransform("CommentDatabase");
	grammar.SetChildlessTransform("CommentIndex");
	grammar.SetChildlessTransform("CommentSchema");
	grammar.SetChildlessTransform("CommentType");
	grammar.SetChildlessTransform("CommentColumn");
	grammar.SetChildlessTransform("StringLiteralValue");
	grammar.SetChildlessTransform("AnalyzeKeyword");
	grammar.SetChildlessTransform("IndexName");
	grammar.SetChildlessTransform("SequenceName");
	grammar.SetChildlessTransform("CollationName");
	grammar.SetChildlessTransform("NumberLiteral");
	grammar.SetChildlessTransform("StringLiteral");
	grammar.SetChildlessTransform("IntervalWithoutSpecifier");
	grammar.SetChildlessTransform("YearKeyword");
	grammar.SetChildlessTransform("MonthKeyword");
	grammar.SetChildlessTransform("DayKeyword");
	grammar.SetChildlessTransform("HourKeyword");
	grammar.SetChildlessTransform("MinuteKeyword");
	grammar.SetChildlessTransform("SecondKeyword");
	grammar.SetChildlessTransform("MillisecondKeyword");
	grammar.SetChildlessTransform("MicrosecondKeyword");
	grammar.SetChildlessTransform("WeekKeyword");
	grammar.SetChildlessTransform("QuarterKeyword");
	grammar.SetChildlessTransform("DecadeKeyword");
	grammar.SetChildlessTransform("CenturyKeyword");
	grammar.SetChildlessTransform("MillenniumKeyword");
	grammar.SetChildlessTransform("VariantType");
	grammar.SetChildlessTransform("IntType");
	grammar.SetChildlessTransform("IntegerType");
	grammar.SetChildlessTransform("SmallintType");
	grammar.SetChildlessTransform("BigintType");
	grammar.SetChildlessTransform("RealType");
	grammar.SetChildlessTransform("BooleanType");
	grammar.SetChildlessTransform("DoubleType");
	grammar.SetChildlessTransform("FloatType");
	grammar.SetChildlessTransform("TypeNameAsQualifiedName");
	grammar.SetChildlessTransform("ArrayKeyword");
	grammar.SetChildlessTransform("TimeTypeId");
	grammar.SetChildlessTransform("TimestampTypeId");
	grammar.SetChildlessTransform("WithRule");
	grammar.SetChildlessTransform("WithoutRule");
	grammar.SetChildlessTransform("DisconnectStatement");
	grammar.SetChildlessTransform("LocalSessionTarget");
	grammar.SetChildlessTransform("CatalogSessionTarget");
	grammar.SetChildlessTransform("CopyFrom");
	grammar.SetChildlessTransform("CopyTo");
	grammar.SetChildlessTransform("CopyFileNameStringLiteral");
	grammar.SetChildlessTransform("CopyFileNameIdentifier");
	grammar.SetChildlessTransform("BinaryOption");
	grammar.SetChildlessTransform("FreezeOption");
	grammar.SetChildlessTransform("OidsOption");
	grammar.SetChildlessTransform("CsvOption");
	grammar.SetChildlessTransform("HeaderOption");
	grammar.SetChildlessTransform("NullAsOption");
	grammar.SetChildlessTransform("DelimiterAsOption");
	grammar.SetChildlessTransform("QuoteAsOption");
	grammar.SetChildlessTransform("EscapeAsOption");
	grammar.SetChildlessTransform("EncodingOption");
	grammar.SetChildlessTransform("ForceQuote");
	grammar.SetChildlessTransform("StarPartitionByColumnList");
	grammar.SetChildlessTransform("ForceNotNull");
	grammar.SetChildlessTransform("CopySchema");
	grammar.SetChildlessTransform("CopyData");
	grammar.SetChildlessTransform("WithOids");
	grammar.SetChildlessTransform("WithoutOids");
	grammar.SetChildlessTransform("UniqueIndex");
	grammar.SetChildlessTransform("IndexType");
	grammar.SetChildlessTransform("DefArgKeyword");
	grammar.SetChildlessTransform("DefArgStringLiteral");
	grammar.SetChildlessTransform("NoneLiteral");
	grammar.SetChildlessTransform("MacroKeyword");
	grammar.SetChildlessTransform("FunctionKeyword");
	grammar.SetChildlessTransform("SecretStorageSpecifier");
	grammar.SetChildlessTransform("SeqCycle");
	grammar.SetChildlessTransform("SeqNoCycle");
	grammar.SetChildlessTransform("MinValue");
	grammar.SetChildlessTransform("MaxValue");
	grammar.SetChildlessTransform("OrReplace");
	grammar.SetChildlessTransform("Persistent");
	grammar.SetChildlessTransform("TempPersistent");
	grammar.SetChildlessTransform("TemporaryPersistent");
	grammar.SetChildlessTransform("WithDataOnly");
	grammar.SetChildlessTransform("WithNoData");
	grammar.SetChildlessTransform("IdentifierList");
	grammar.SetChildlessTransform("IfNotExists");
	grammar.SetChildlessTransform("CatalogQualification");
	grammar.SetChildlessTransform("SchemaQualification");
	grammar.SetChildlessTransform("ReservedSchemaQualification");
	grammar.SetChildlessTransform("TableQualification");
	grammar.SetChildlessTransform("ReservedTableQualification");
	grammar.SetChildlessTransform("NullConstraint");
	grammar.SetChildlessTransform("NotNullColumnConstraint");
	grammar.SetChildlessTransform("UniqueConstraint");
	grammar.SetChildlessTransform("PrimaryKeyConstraint");
	grammar.SetChildlessTransform("NoKeyAction");
	grammar.SetChildlessTransform("RestrictKeyAction");
	grammar.SetChildlessTransform("CascadeKeyAction");
	grammar.SetChildlessTransform("SetNullKeyAction");
	grammar.SetChildlessTransform("SetDefaultKeyAction");
	grammar.SetChildlessTransform("Identifier");
	grammar.SetChildlessTransform("StringLiteralIdentifier");
	grammar.SetChildlessTransform("PreserveRows");
	grammar.SetChildlessTransform("DeleteRows");
	grammar.SetChildlessTransform("VirtualGeneratedColumn");
	grammar.SetChildlessTransform("StoredGeneratedColumn");
	grammar.SetChildlessTransform("TriggerName");
	grammar.SetChildlessTransform("TriggerBefore");
	grammar.SetChildlessTransform("TriggerAfter");
	grammar.SetChildlessTransform("TriggerInsteadOf");
	grammar.SetChildlessTransform("TriggerEventInsert");
	grammar.SetChildlessTransform("TriggerEventDelete");
	grammar.SetChildlessTransform("TriggerEventUpdate");
	grammar.SetChildlessTransform("ForEachRow");
	grammar.SetChildlessTransform("ForEachStatement");
	grammar.SetChildlessTransform("EnumStringLiteralList");
	grammar.SetChildlessTransform("CreateRecursive");
	grammar.SetChildlessTransform("CreateSecure");
	grammar.SetChildlessTransform("DeallocatePrepare");
	grammar.SetChildlessTransform("ShowSettingName");
	grammar.SetChildlessTransform("DescribeStringLiteral");
	grammar.SetChildlessTransform("SummarizeRule");
	grammar.SetChildlessTransform("ShowRule");
	grammar.SetChildlessTransform("DescribeLongRule");
	grammar.SetChildlessTransform("DescRule");
	grammar.SetChildlessTransform("QualifiedIndexNameString");
	grammar.SetChildlessTransform("MaterializedViewEntry");
	grammar.SetChildlessTransform("FunctionTypeMacroKeyword");
	grammar.SetChildlessTransform("FunctionTypeFunction");
	grammar.SetChildlessTransform("CascadeDropBehavior");
	grammar.SetChildlessTransform("RestrictDropBehavior");
	grammar.SetChildlessTransform("IfExists");
	grammar.SetChildlessTransform("DropSecretStorage");
	grammar.SetChildlessTransform("ExportSource");
	grammar.SetChildlessTransform("ImportStatement");
	grammar.SetChildlessTransform("FunctionNameAsQualifiedName");
	grammar.SetChildlessTransform("DistinctKeyword");
	grammar.SetChildlessTransform("AllKeyword");
	grammar.SetChildlessTransform("IgnoreNulls");
	grammar.SetChildlessTransform("RespectNulls");
	grammar.SetChildlessTransform("LiteralExpression");
	grammar.SetChildlessTransform("NullLiteral");
	grammar.SetChildlessTransform("TrueLiteral");
	grammar.SetChildlessTransform("FalseLiteral");
	grammar.SetChildlessTransform("CastKeyword");
	grammar.SetChildlessTransform("TryCastKeyword");
	grammar.SetChildlessTransform("SubqueryNot");
	grammar.SetChildlessTransform("SubqueryExists");
	grammar.SetChildlessTransform("IntervalStringParameter");
	grammar.SetChildlessTransform("RowsFraming");
	grammar.SetChildlessTransform("RangeFraming");
	grammar.SetChildlessTransform("GroupsFraming");
	grammar.SetChildlessTransform("FrameCurrentRow");
	grammar.SetChildlessTransform("PrecedingFrame");
	grammar.SetChildlessTransform("FollowingFrame");
	grammar.SetChildlessTransform("ExcludeCurrentRow");
	grammar.SetChildlessTransform("ExcludeGroup");
	grammar.SetChildlessTransform("ExcludeTies");
	grammar.SetChildlessTransform("ExcludeNoOthers");
	grammar.SetChildlessTransform("IdentifierWindowFrame");
	grammar.SetChildlessTransform("ParensIdentifier");
	grammar.SetChildlessTransform("BaseWindowName");
	grammar.SetChildlessTransform("GroupingKeyword");
	grammar.SetChildlessTransform("GroupingIdKeyword");
	grammar.SetChildlessTransform("QuestionMarkNumberedParameter");
	grammar.SetChildlessTransform("AnonymousParameter");
	grammar.SetChildlessTransform("NumberedParameter");
	grammar.SetChildlessTransform("PositionalExpression");
	grammar.SetChildlessTransform("DefaultExpression");
	grammar.SetChildlessTransform("NotKeyword");
	grammar.SetChildlessTransform("UnknownLiteral");
	grammar.SetChildlessTransform("NotNullKeyword");
	grammar.SetChildlessTransform("NotNullOperator");
	grammar.SetChildlessTransform("IsNullOperator");
	grammar.SetChildlessTransform("IsDistinctFromOp");
	grammar.SetChildlessTransform("OperatorEqual");
	grammar.SetChildlessTransform("OperatorNotEqual");
	grammar.SetChildlessTransform("OperatorLessThan");
	grammar.SetChildlessTransform("OperatorGreaterThan");
	grammar.SetChildlessTransform("OperatorLessThanEquals");
	grammar.SetChildlessTransform("OperatorGreaterThanEquals");
	grammar.SetChildlessTransform("LikeToken");
	grammar.SetChildlessTransform("ILikeToken");
	grammar.SetChildlessTransform("GlobToken");
	grammar.SetChildlessTransform("SimilarToToken");
	grammar.SetChildlessTransform("RegexMatchToken");
	grammar.SetChildlessTransform("RegexInsensitiveMatchToken");
	grammar.SetChildlessTransform("NotILikeOp");
	grammar.SetChildlessTransform("NotLikeOp");
	grammar.SetChildlessTransform("NotRegexInsensitiveMatchOp");
	grammar.SetChildlessTransform("NotSimilarToOp");
	grammar.SetChildlessTransform("OperatorLiteral");
	grammar.SetChildlessTransform("SubqueryAny");
	grammar.SetChildlessTransform("SubqueryAll");
	grammar.SetChildlessTransform("InetOperator");
	grammar.SetChildlessTransform("JsonOperator");
	grammar.SetChildlessTransform("ListOperator");
	grammar.SetChildlessTransform("StringOperator");
	grammar.SetChildlessTransform("AnyOp");
	grammar.SetChildlessTransform("BitOperator");
	grammar.SetChildlessTransform("Term");
	grammar.SetChildlessTransform("Factor");
	grammar.SetChildlessTransform("ExponentOperator");
	grammar.SetChildlessTransform("MinusPrefixOperator");
	grammar.SetChildlessTransform("PlusPrefixOperator");
	grammar.SetChildlessTransform("TildePrefixOperator");
	grammar.SetChildlessTransform("EndSliceMinus");
	grammar.SetChildlessTransform("PostfixOperator");
	grammar.SetChildlessTransform("TrimBoth");
	grammar.SetChildlessTransform("TrimLeading");
	grammar.SetChildlessTransform("TrimTrailing");
	grammar.SetChildlessTransform("ExtractIdentifierArgument");
	grammar.SetChildlessTransform("ExtractStringArgument");
	grammar.SetChildlessTransform("ShowAllModifier");
	grammar.SetChildlessTransform("InsertOrReplace");
	grammar.SetChildlessTransform("InsertOrIgnore");
	grammar.SetChildlessTransform("InsertByName");
	grammar.SetChildlessTransform("InsertByPosition");
	grammar.SetChildlessTransform("InsertAlias");
	grammar.SetChildlessTransform("DefaultValues");
	grammar.SetChildlessTransform("OnConflictNothing");
	grammar.SetChildlessTransform("ExtensionAlias");
	grammar.SetChildlessTransform("InstallAndLoad");
	grammar.SetChildlessTransform("UpdateExtensionsStatement");
	grammar.SetChildlessTransform("FromSourceIdentifier");
	grammar.SetChildlessTransform("FromSourceString");
	grammar.SetChildlessTransform("RepositoryPrefix");
	grammar.SetChildlessTransform("RepositoryPublicKey");
	grammar.SetChildlessTransform("DeleteMatchClause");
	grammar.SetChildlessTransform("InsertDefaultValues");
	grammar.SetChildlessTransform("DoNothingMatchClause");
	grammar.SetChildlessTransform("BySource");
	grammar.SetChildlessTransform("ByTarget");
	grammar.SetChildlessTransform("IncludeNulls");
	grammar.SetChildlessTransform("ExcludeNulls");
	grammar.SetChildlessTransform("SetopUnion");
	grammar.SetChildlessTransform("SetopExcept");
	grammar.SetChildlessTransform("Materialized");
	grammar.SetChildlessTransform("DistinctAll");
	grammar.SetChildlessTransform("PivotEnumTarget");
	grammar.SetChildlessTransform("Lateral");
	grammar.SetChildlessTransform("UnqualifiedBaseTableName");
	grammar.SetChildlessTransform("WithOrdinality");
	grammar.SetChildlessTransform("VersionAtUnit");
	grammar.SetChildlessTransform("TimestampAtUnit");
	grammar.SetChildlessTransform("NearestApprox");
	grammar.SetChildlessTransform("NearestExact");
	grammar.SetChildlessTransform("NearestDistance");
	grammar.SetChildlessTransform("NearestSimilarity");
	grammar.SetChildlessTransform("Asof");
	grammar.SetChildlessTransform("UsingClause");
	grammar.SetChildlessTransform("CrossJoinPrefix");
	grammar.SetChildlessTransform("PositionalJoinPrefix");
	grammar.SetChildlessTransform("FullJoin");
	grammar.SetChildlessTransform("LeftJoin");
	grammar.SetChildlessTransform("RightJoin");
	grammar.SetChildlessTransform("SemiJoin");
	grammar.SetChildlessTransform("AntiJoin");
	grammar.SetChildlessTransform("InnerJoin");
	grammar.SetChildlessTransform("SampleSeed");
	grammar.SetChildlessTransform("SamplePercentage");
	grammar.SetChildlessTransform("SampleRows");
	grammar.SetChildlessTransform("GroupByAll");
	grammar.SetChildlessTransform("EmptyGroupingItem");
	grammar.SetChildlessTransform("CubeKeyword");
	grammar.SetChildlessTransform("RollupKeyword");
	grammar.SetChildlessTransform("DescendingOrder");
	grammar.SetChildlessTransform("AscendingOrder");
	grammar.SetChildlessTransform("NullsFirst");
	grammar.SetChildlessTransform("NullsLast");
	grammar.SetChildlessTransform("LimitAll");
	grammar.SetChildlessTransform("LimitLiteralPercent");
	grammar.SetChildlessTransform("SetSchema");
	grammar.SetChildlessTransform("ZoneLocal");
	grammar.SetChildlessTransform("ZoneDefault");
	grammar.SetChildlessTransform("ZoneStringLiteral");
	grammar.SetChildlessTransform("ZoneIdentifier");
	grammar.SetChildlessTransform("ZoneIntervalWithPrecision");
	grammar.SetChildlessTransform("VariableScope");
	grammar.SetChildlessTransform("LocalScope");
	grammar.SetChildlessTransform("SessionScope");
	grammar.SetChildlessTransform("GlobalScope");
	grammar.SetChildlessTransform("RollbackTransaction");
	grammar.SetChildlessTransform("CommitTransaction");
	grammar.SetChildlessTransform("ReadOnly");
	grammar.SetChildlessTransform("ReadWrite");
	grammar.SetChildlessTransform("SchemaNameAsUseTarget");
	grammar.SetChildlessTransform("CatalogNameAsUseTarget");
	grammar.SetChildlessTransform("DotIdentifier");
	grammar.SetChildlessTransform("OptFull");
	grammar.SetChildlessTransform("OptFreeze");
	grammar.SetChildlessTransform("OptVerbose");
	//===--------------------------------------------------------------------===//
	// END GENERATED CHILDLESS TRANSFORM RULES
	//===--------------------------------------------------------------------===//
}

void PEGTransformerFactory::RegisterDefaultTransforms(ParsedGrammar &grammar) {
	PEGTransformerFactory factory(grammar);
}

vector<reference<ParseResult>> PEGTransformerFactory::ExtractParseResultsFromList(ParseResult &parse_result) {
	// List(D) <- D (',' D)* ','?
	vector<reference<ParseResult>> result;
	auto &list_pr = parse_result.Cast<ListParseResult>();
	result.push_back(list_pr.GetChild(0));
	auto &opt_child = list_pr.Child<OptionalParseResult>(1);
	if (opt_child.HasResult()) {
		auto &repeat_result = opt_child.GetResult().Cast<RepeatParseResult>();
		for (auto &child : repeat_result.GetChildren()) {
			auto &list_child = child.get().Cast<ListParseResult>();
			result.push_back(list_child.GetChild(1));
		}
	}
	return result;
}

ParseResult &PEGTransformerFactory::ExtractResultFromParens(ParseResult &parse_result) {
	// Parens(D) <- '(' D ')'
	auto &list_pr = parse_result.Cast<ListParseResult>();
	return list_pr.GetChild(1);
}

bool PEGTransformerFactory::ExpressionIsEmptyStar(const ParsedExpression &expr) {
	if (expr.GetExpressionClass() != ExpressionClass::STAR) {
		return false;
	}
	auto &star = expr.Cast<StarExpression>();
	if (!star.IsColumns() && star.ExcludeList().empty() && star.ReplaceList().empty()) {
		return true;
	}
	return false;
}

QualifiedName PEGTransformerFactory::StringToQualifiedName(vector<string> input) {
	if (input.empty()) {
		throw InternalException("QualifiedName cannot be made with an empty input.");
	}
	if (input.size() == 1) {
		return QualifiedName(Identifier(input[0]));
	} else if (input.size() == 2) {
		return QualifiedName({Identifier(input[0])}, Identifier(input[1]));
	} else if (input.size() == 3) {
		return QualifiedName(Identifier(input[0]), Identifier(input[1]), Identifier(input[2]));
	} else {
		throw ParserException("Too many qualifications found - expected [catalog.schema.name] or [schema.name]");
	}
}

QualifiedColumnName PEGTransformerFactory::StringToQualifiedColumnName(const vector<string> &input) {
	if (input.empty()) {
		throw InternalException("QualifiedColumnName cannot be made with an empty input.");
	}
	auto identifiers = StringsToIdentifiers(input);
	if (identifiers.size() == 1) {
		return QualifiedColumnName(std::move(identifiers[0]));
	} else if (identifiers.size() == 2) {
		return QualifiedColumnName(std::move(identifiers[0]), std::move(identifiers[1]));
	} else if (identifiers.size() == 3) {
		QualifiedColumnName result;
		result.schema = std::move(identifiers[0]);
		result.table = std::move(identifiers[1]);
		result.column = std::move(identifiers[2]);
		return result;
	} else if (identifiers.size() == 4) {
		QualifiedColumnName result;
		result.catalog = std::move(identifiers[0]);
		result.schema = std::move(identifiers[1]);
		result.table = std::move(identifiers[2]);
		result.column = std::move(identifiers[3]);
		return result;
	}
	throw ParserException("Expected at most 4 entries (catalog.schema.table.column), but found %zu entries (input: %s)",
	                      input.size(), StringUtil::Join(input, "."));
}

LogicalType PEGTransformerFactory::GetIntervalTargetType(DatePartSpecifier date_part) {
	switch (date_part) {
	case DatePartSpecifier::YEAR:
	case DatePartSpecifier::MONTH:
	case DatePartSpecifier::DAY:
	case DatePartSpecifier::WEEK:
	case DatePartSpecifier::QUARTER:
	case DatePartSpecifier::DECADE:
	case DatePartSpecifier::CENTURY:
	case DatePartSpecifier::MILLENNIUM:
		return LogicalType::INTEGER;
	case DatePartSpecifier::HOUR:
	case DatePartSpecifier::MINUTE:
	case DatePartSpecifier::MICROSECONDS:
		return LogicalType::BIGINT;
	case DatePartSpecifier::MILLISECONDS:
	case DatePartSpecifier::SECOND:
		return LogicalType::DOUBLE;
	default:
		throw InternalException("Unsupported interval post-fix");
	}
}

} // namespace duckdb
