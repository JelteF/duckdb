#pragma once

#include "duckdb/parser/peg/ast/unpivot_name_values.hpp"
#include "duckdb/parser/qualified_name_set.hpp"
#include "duckdb/parser/peg/transformer/parse_result.hpp"
#include "duckdb/parser/peg/transformer/transform_result.hpp"
#include "duckdb/parser/peg/ast/add_column_entry.hpp"
#include "duckdb/parser/peg/ast/column_constraint_entry.hpp"
#include "duckdb/parser/peg/ast/analyze_target.hpp"
#include "duckdb/parser/peg/ast/column_elements.hpp"
#include "duckdb/parser/peg/ast/create_table_column_element.hpp"
#include "duckdb/parser/peg/ast/create_table_definition.hpp"
#include "duckdb/parser/peg/ast/partition_sorted_options.hpp"
#include "duckdb/parser/peg/ast/distinct_clause.hpp"
#include "duckdb/parser/peg/ast/describe_target.hpp"
#include "duckdb/parser/peg/ast/extension_repository_info.hpp"
#include "duckdb/parser/peg/ast/generated_column_definition.hpp"
#include "duckdb/parser/peg/ast/generic_copy_option.hpp"
#include "duckdb/parser/peg/ast/generic_copy_option_value.hpp"
#include "duckdb/parser/peg/ast/insert_values.hpp"
#include "duckdb/parser/peg/ast/create_pivot_entry.hpp"
#include "duckdb/parser/peg/ast/join_prefix.hpp"
#include "duckdb/parser/peg/ast/join_qualifier.hpp"
#include "duckdb/parser/peg/ast/key_actions.hpp"
#include "duckdb/parser/peg/ast/limit_percent_result.hpp"
#include "duckdb/parser/peg/ast/macro_parameter.hpp"
#include "duckdb/parser/peg/ast/on_conflict_expression_target.hpp"
#include "duckdb/parser/peg/ast/sequence_option.hpp"
#include "duckdb/parser/peg/ast/setting_info.hpp"
#include "duckdb/parser/peg/ast/table_alias.hpp"
#include "duckdb/parser/peg/ast/cast_arguments.hpp"
#include "duckdb/parser/peg/ast/expression_chain.hpp"
#include "duckdb/parser/peg/ast/method_arguments.hpp"
#include "duckdb/parser/peg/ast/trim_arguments.hpp"
#include "duckdb/parser/peg/ast/trigger_event_info.hpp"
#include "duckdb/parser/peg/ast/trigger_table_referencing_info.hpp"
#include "duckdb/parser/peg/ast/window_frame.hpp"
#include "duckdb/function/macro_function.hpp"
#include "duckdb/common/optional.hpp"
#include "duckdb/parser/query_node/set_operation_node.hpp"
#include "duckdb/parser/parser_options.hpp"
#include "duckdb/common/stack_checker.hpp"
#include "duckdb/parser/expression/case_expression.hpp"
#include "duckdb/parser/expression/function_expression.hpp"
#include "duckdb/parser/expression/parameter_expression.hpp"
#include "duckdb/parser/expression/window_expression.hpp"
#include "duckdb/parser/parsed_data/connect_info.hpp"
#include "duckdb/parser/parsed_data/create_type_info.hpp"
#include "duckdb/parser/parsed_data/transaction_info.hpp"
#include "duckdb/parser/parsed_data/vacuum_info.hpp"
#include "duckdb/parser/statement/copy_database_statement.hpp"
#include "duckdb/parser/statement/set_statement.hpp"
#include "duckdb/parser/statement/create_statement.hpp"
#include "duckdb/parser/statement/transaction_statement.hpp"
#include "duckdb/parser/tableref/basetableref.hpp"
#include "duckdb/parser/peg/parsed_grammar.hpp"
#include "duckdb/storage/arena_allocator.hpp"
#include "duckdb/common/arena_containers/arena_vector.hpp"
#include "duckdb/common/arena_containers/arena_ptr.hpp"
#include "duckdb/parser/query_node/select_node.hpp"
#include "duckdb/parser/statement/drop_statement.hpp"
#include "duckdb/parser/statement/insert_statement.hpp"
#include "duckdb/parser/statement/merge_into_statement.hpp"
#include "duckdb/parser/tableref/pivotref.hpp"

namespace duckdb {

// Forward declare
struct QualifiedName;
struct CompiledGrammar;
struct MatcherToken;
struct GroupingExpressionMap;
class Matcher;
class TokenIterator;

enum class GroupByExpressionInfoType : uint8_t { EXPRESSION, EMPTY, CUBE, ROLLUP, GROUPING_SETS };

struct GroupByExpressionInfo {
	GroupByExpressionInfoType type = GroupByExpressionInfoType::EMPTY;
	unique_ptr<ParsedExpression> expression;
	vector<unique_ptr<ParsedExpression>> expressions;
	vector<GroupByExpressionInfo> children;
};

//===--------------------------------------------------------------------===//
// START GENERATED TRANSFORM RESULT TYPES
//===--------------------------------------------------------------------===//
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.AddColumnEntry", AddColumnEntry);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.AnalyzeTarget", AnalyzeTarget);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.BetweenInLikeOperator", BetweenInLikeOperator);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.BinaryExpressionTail", BinaryExpressionTail);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.CaseCheck", CaseCheck);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.CastArguments", CastArguments);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.CatalogType", CatalogType);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.ColumnConstraintEntry", ColumnConstraintEntry);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.ColumnElements", ColumnElements);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.ColumnList", ColumnList);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.CommonTableExpressionMap", CommonTableExpressionMap);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.ComparisonExpressionTail", ComparisonExpressionTail);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.ConstraintColumnDefinition", ConstraintColumnDefinition);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.CopyDatabaseType", CopyDatabaseType);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.CreateTableColumnElement", CreateTableColumnElement);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.CreateTableDefinition", CreateTableDefinition);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.DatePartSpecifier", DatePartSpecifier);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.DescribeTarget", DescribeTarget);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.DistinctClause", DistinctClause);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.ExpressionType", ExpressionType);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.ExtensionRepositoryInfo", ExtensionRepositoryInfo);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.FunctionArgument", FunctionArgument);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.GeneratedColumnDefinition", GeneratedColumnDefinition);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.GenericCopyOption", GenericCopyOption);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.GenericCopyOptionValue", GenericCopyOptionValue);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.GroupByExpressionInfo", GroupByExpressionInfo);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.GroupByNode", GroupByNode);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.Identifier", Identifier);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.InsertColumnOrder", InsertColumnOrder);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.InsertValues", InsertValues);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.IsDistinctFromTail", IsDistinctFromTail);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.JoinPrefix", JoinPrefix);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.JoinQualifier", JoinQualifier);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.JoinType", JoinType);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.KeyActions", KeyActions);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.LimitPercentResult", LimitPercentResult);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.LogicalType", LogicalType);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.LogicalTypeId", LogicalTypeId);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.MacroParameter", MacroParameter);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.MergeActionCondition", MergeActionCondition);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.MethodArguments", MethodArguments);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.OnConflictAction", OnConflictAction);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.OnConflictExpressionTarget", OnConflictExpressionTarget);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.OrderByNode", OrderByNode);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.OrderByNullType", OrderByNullType);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.OrderType", OrderType);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.OtherOperatorTail", OtherOperatorTail);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.ParsedOperator", ParsedOperator);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.PartitionSortedOptions", PartitionSortedOptions);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.PivotColumn", PivotColumn);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.QualifiedColumnName", QualifiedColumnName);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.QualifiedName", QualifiedName);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.SampleMethod", SampleMethod);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.SecretPersistType", SecretPersistType);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.SetOperationType", SetOperationType);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.SetScope", SetScope);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.SettingInfo", SettingInfo);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.ShowType", ShowType);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.TableAlias", TableAlias);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.TransactionModifierType", TransactionModifierType);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.TriggerEventInfo", TriggerEventInfo);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.TriggerForEach", TriggerForEach);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.TriggerTableReferencingInfo",
                                      TriggerTableReferencingInfo);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.TriggerTiming", TriggerTiming);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.TrimArguments", TrimArguments);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.UnpivotNameValues", UnpivotNameValues);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.VacuumOptions", VacuumOptions);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.Value", Value);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.WindowBoundaryExpression", WindowBoundaryExpression);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.WindowExcludeMode", WindowExcludeMode);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.WindowFrame", WindowFrame);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.bool", bool);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.case_insensitive_map_t<unique_ptr<ParsedExpression>>",
                                      case_insensitive_map_t<unique_ptr<ParsedExpression>>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.child_list_t<LogicalType>", child_list_t<LogicalType>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.int64_t", int64_t);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.optional_idx", optional_idx);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.pair<Identifier, LogicalType>",
                                      pair<Identifier, LogicalType>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.pair<Identifier, unique_ptr<CommonTableExpressionInfo>>",
                                      pair<Identifier, unique_ptr<CommonTableExpressionInfo>>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.pair<Identifier, unique_ptr<ParsedExpression>>",
                                      pair<Identifier, unique_ptr<ParsedExpression>>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.pair<MergeActionCondition, unique_ptr<MergeIntoAction>>",
                                      pair<MergeActionCondition, unique_ptr<MergeIntoAction>>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.pair<QualifiedColumnName, string>",
                                      pair<QualifiedColumnName, string>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.pair<SampleMethod, optional_idx>",
                                      pair<SampleMethod, optional_idx>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.pair<string, bool>", pair<string, bool>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.pair<string, unique_ptr<ParsedExpression>>",
                                      pair<string, unique_ptr<ParsedExpression>>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.pair<string, unique_ptr<SequenceOption>>",
                                      pair<string, unique_ptr<SequenceOption>>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE(
    "duckdb.transform_result.pair<unique_ptr<SetOperationNode>, unique_ptr<SelectStatement>>",
    pair<unique_ptr<SetOperationNode>, unique_ptr<SelectStatement>>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.qualified_column_map_t<string>",
                                      qualified_column_map_t<string>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.qualified_column_set_t", qualified_column_set_t);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.string", string);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.transform_result_ptr", transform_result_ptr);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<AlterInfo>", unique_ptr<AlterInfo>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<AlterTableInfo>", unique_ptr<AlterTableInfo>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<AtClause>", unique_ptr<AtClause>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<BaseTableRef>", unique_ptr<BaseTableRef>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<ColumnRefExpression>",
                                      unique_ptr<ColumnRefExpression>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<ConnectInfo>", unique_ptr<ConnectInfo>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<Constraint>", unique_ptr<Constraint>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<CreateStatement>",
                                      unique_ptr<CreateStatement>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<CreateTypeInfo>", unique_ptr<CreateTypeInfo>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<DropStatement>", unique_ptr<DropStatement>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<ExternalResourceOptions>",
                                      unique_ptr<ExternalResourceOptions>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<MacroFunction>", unique_ptr<MacroFunction>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<MergeIntoAction>",
                                      unique_ptr<MergeIntoAction>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<OnConflictInfo>", unique_ptr<OnConflictInfo>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<ParsedExpression>",
                                      unique_ptr<ParsedExpression>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<QueryNode>", unique_ptr<QueryNode>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<ResultModifier>", unique_ptr<ResultModifier>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<SQLStatement>", unique_ptr<SQLStatement>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<SampleOptions>", unique_ptr<SampleOptions>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<SelectNode>", unique_ptr<SelectNode>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<SelectStatement>",
                                      unique_ptr<SelectStatement>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<SetOperationNode>",
                                      unique_ptr<SetOperationNode>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<SetStatement>", unique_ptr<SetStatement>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<TableRef>", unique_ptr<TableRef>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<UpdateSetInfo>", unique_ptr<UpdateSetInfo>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.unique_ptr<WindowExpression>",
                                      unique_ptr<WindowExpression>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.vector<FunctionArgument>", vector<FunctionArgument>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.vector<GenericCopyOption>", vector<GenericCopyOption>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.vector<Identifier>", vector<Identifier>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.vector<LogicalType>", vector<LogicalType>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.vector<MacroParameter>", vector<MacroParameter>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.vector<OrderByNode>", vector<OrderByNode>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.vector<PivotColumn>", vector<PivotColumn>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.vector<PivotColumnEntry>", vector<PivotColumnEntry>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.vector<WindowBoundaryExpression>",
                                      vector<WindowBoundaryExpression>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.vector<bool>", vector<bool>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.vector<string>", vector<string>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.vector<unique_ptr<ParsedExpression>>",
                                      vector<unique_ptr<ParsedExpression>>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.vector<unique_ptr<ResultModifier>>",
                                      vector<unique_ptr<ResultModifier>>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.vector<unique_ptr<SQLStatement>>",
                                      vector<unique_ptr<SQLStatement>>);
DUCKDB_REGISTER_TRANSFORM_RESULT_TYPE("duckdb.transform_result.vector<unique_ptr<TableRef>>",
                                      vector<unique_ptr<TableRef>>);
//===--------------------------------------------------------------------===//
// END GENERATED TRANSFORM RESULT TYPES
//===--------------------------------------------------------------------===//

class PEGTransformer;
class TransformStack;
class TransformProcess;
class GeneratedTransformProcess;

using transform_process_initialize_t = void (*)(PEGTransformer &transformer, GeneratedTransformProcess &process);
using transform_process_finalize_t = transform_result_ptr (*)(PEGTransformer &transformer,
                                                              GeneratedTransformProcess &process);

struct TransformFrameOps {
	const char *name;
	transform_process_initialize_t initialize;
	transform_process_finalize_t finalize;
};

template <typename T>
unique_ptr<TypedTransformResult<T>> TryBridgeTransformResultValue(TransformResultValue &base_result);

//! Input to start a transformer execution. The rule can be supplied explicitly for transparent parse nodes.
struct TransformInput {
	TransformInput(ParseResult &parse_result_p) // NOLINT(google-explicit-constructor): enables braced PushChild calls.
	    : parse_result(parse_result_p) {
	}
	TransformInput(const CompiledGrammarRule &rule_p, ParseResult &parse_result_p)
	    : rule(rule_p), parse_result(parse_result_p) {
	}

	optional_ptr<const CompiledGrammarRule> GetRule() const {
		// A collapsed result stands in for a rule that would have returned it unchanged; transform it as itself
		if (parse_result.collapsed) {
			return parse_result.GetRule();
		}
		return rule ? rule : parse_result.GetRule();
	}

	optional_ptr<const CompiledGrammarRule> rule;
	ParseResult &parse_result;
};

//! Essentially a std::variant<TransformInput, transform_result_ptr>.
//! Produced by TransformProcess::Resume to control the next execution step.
class TransformStep {
public:
	static TransformStep Child(TransformInput input);
	static TransformStep Complete(transform_result_ptr result);

	optional<TransformInput> GetChild();
	transform_result_ptr TakeResult();

private:
	TransformStep(optional<TransformInput> child_p, transform_result_ptr result_p)
	    : child(std::move(child_p)), result(std::move(result_p)) {
	}

private:
	optional<TransformInput> child;
	transform_result_ptr result;
};

class TransformProcess {
public:
	virtual ~TransformProcess() = default;

	//! Resume transforming, optionally with the result of the previously requested child.
	virtual TransformStep Resume(transform_result_ptr child_result) = 0;
};

class GeneratedTransformProcess final : public TransformProcess {
public:
	GeneratedTransformProcess(PEGTransformer &transformer, TransformInput input, const TransformFrameOps &info);

	void ReserveChildSlots(idx_t count);
	void SetChildResult(idx_t slot, transform_result_ptr result);
	void PushChild(TransformInput input, idx_t slot);
	TransformStep Resume(transform_result_ptr child_result) override;

	template <class T>
	T TakeResult(idx_t slot) {
		if (slot >= child_results.size() || !child_results[slot]) {
			throw InternalException("Missing transformer result for slot %llu in rule '%s'", slot, info.name);
		}
		auto *result_value = TryGetTransformResult<T>(*child_results[slot]);
		if (!result_value) {
			auto bridged = TryBridgeTransformResultValue<T>(*child_results[slot]);
			if (bridged) {
				auto bridged_result = std::move(bridged->value);
				child_results[slot].reset();
				return bridged_result;
			}
			throw InternalException("Unexpected transformer result type for slot %llu in rule '%s'", slot, info.name);
		}
		auto result = std::move(*result_value);
		child_results[slot].reset();
		return result;
	}

	template <class T>
	T &GetResult(idx_t slot) {
		if (slot >= child_results.size() || !child_results[slot]) {
			throw InternalException("Missing transformer result for slot %llu in rule '%s'", slot, info.name);
		}
		auto *result_value = TryGetTransformResult<T>(*child_results[slot]);
		if (!result_value) {
			throw InternalException("Unexpected transformer result type for slot %llu in rule '%s'", slot, info.name);
		}
		return *result_value;
	}

	ParseResult &parse_result;
	const TransformFrameOps &info;
	idx_t manual_state = 0;
	arena_vector<transform_result_ptr> child_results;

private:
	struct PendingChild {
		TransformInput input;
		idx_t slot;
	};

	TransformStep NextStep();

private:
	PEGTransformer &transformer;
	arena_vector<PendingChild> pending_children;
	optional_idx child_result_slot;
	bool completed = false;
};

using transform_finalize_function_t =
    std::function<transform_result_ptr(PEGTransformer &transformer, ParseResult &parse_result)>;

class FinalizeTransformProcess final : public TransformProcess {
public:
	FinalizeTransformProcess(PEGTransformer &transformer, ParseResult &parse_result,
	                         transform_finalize_function_t finalize);
	TransformStep Resume(transform_result_ptr child_result) override;

private:
	PEGTransformer &transformer;
	ParseResult &parse_result;
	transform_finalize_function_t finalize;
	bool completed = false;
};

struct TransformStackFrame {
	explicit TransformStackFrame(TransformInput input);

	optional_ptr<const CompiledGrammarRule> rule;
	ParseResult &parse_result;
	arena_ptr<TransformProcess> process;
	transform_result_ptr child_result;
};

#ifdef DEBUG
using frame_stack_t = InspectableStack<TransformStackFrame>;
#else
using frame_stack_t = stack<TransformStackFrame>;
#endif

class TransformStack {
public:
	explicit TransformStack(PEGTransformer &transformer);
	transform_result_ptr Execute(TransformInput input);

	template <class T>
	T Execute(TransformInput input) {
		auto base_result = Execute(input);
		auto *result_value = TryGetTransformResult<T>(*base_result);
		if (!result_value) {
			throw InternalException("Unexpected transformer result type for root rule '%s'", input.parse_result.Name());
		}
		return std::move(*result_value);
	}

#ifdef DEBUG
	string FormatStack() const;
#endif

private:
	void PushFrame(TransformInput input);
	void InitializeFrame(TransformStackFrame &frame);
	transform_result_ptr ExecuteFrame(TransformStackFrame &frame);

private:
	PEGTransformer &transformer;
	frame_stack_t frames;
};

class PEGTransformer {
public:
	PEGTransformer(ArenaAllocator &allocator, TokenIterator &token_iterator, ParserOptions &options_p,
	               const CompiledGrammar &grammar_p)
	    : allocator(allocator), token_iterator(token_iterator), options(options_p), grammar(grammar_p) {
	}

	const CompiledGrammarRule &GetRule(const string &rule_name) const;

public:
	template <typename T>
	T Transform(ParseResult &parse_result) {
		auto base_result = TransformInternal(parse_result);

		auto *result_value = TryGetTransformResult<T>(*base_result);
		if (!result_value) {
			// allow transparent bridging between string-typed and Identifier-typed rules
			auto bridged = TryBridgeTransformResult<T>(*base_result);
			if (bridged) {
				auto bridged_result = std::move(bridged->value);
				SetResultLocation(bridged_result, parse_result.GetLocation());
				return bridged_result;
			}
			throw InternalException("Transformer for rule '" + parse_result.Name() + "' returned an unexpected type.");
		}

		auto result = std::move(*result_value);
		SetResultLocation(result, parse_result.GetLocation());
		return result;
	}

	//! Bridge between string-typed and Identifier-typed rule results (and their vector forms).
	//! The generic form performs no bridging; the specializations below convert transparently.
	template <typename T>
	static unique_ptr<TypedTransformResult<T>> TryBridgeTransformResult(TransformResultValue &base_result) {
		return TryBridgeTransformResultValue<T>(base_result);
	}

	template <typename T>
	T Transform(ListParseResult &parse_result, idx_t child_index) {
		auto &child_parse_result = parse_result.GetChild(child_index);
		return Transform<T>(child_parse_result);
	}

	template <typename T>
	void TransformOptional(ListParseResult &list_pr, idx_t child_idx, T &target) {
		auto &opt = list_pr.Child<OptionalParseResult>(child_idx);
		if (opt.HasResult()) {
			target = Transform<T>(opt.GetResult());
		}
	}

	// Make overloads return raw pointers, as ownership is handled by the ArenaAllocator.
	template <class T, typename... Args>
	T *Make(Args &&...args) {
		return allocator.Make<T>(std::forward<Args>(args)...);
	}

	//! Transform processes live exactly as long as one transform run, so they are carved out of the same arena
	template <class T, typename... Args>
	arena_ptr<TransformProcess> MakeProcess(Args &&...args) {
		return arena_ptr<TransformProcess>(allocator.Make<T>(std::forward<Args>(args)...));
	}

	//! The result a transformer hands back, carved out of the same arena
	template <class T, typename... Args>
	transform_result_ptr MakeResult(Args &&...args) {
		return transform_result_ptr(allocator.Make<TypedTransformResult<T>>(std::forward<Args>(args)...));
	}

	ArenaAllocator &GetAllocator() {
		return allocator;
	}

	void Clear();
	void ClearParameters();
	static void ParamTypeCheck(PreparedParamType last_type, PreparedParamType new_type);
	void SetParam(const Identifier &name, idx_t index, PreparedParamType type);
	bool GetParam(const Identifier &name, idx_t &index, PreparedParamType type);
	void SetParamCount(idx_t new_count);
	idx_t ParamCount() const;
	unique_ptr<SQLStatement> CreatePivotStatement(unique_ptr<SQLStatement> statement);
	unique_ptr<SQLStatement> GenerateCreateEnumStmt(unique_ptr<CreatePivotEntry> entry);
	void PivotEntryCheck(const string &type);
	void ExtractCTEsRecursive(CommonTableExpressionMap &cte_map);
	bool IsWindowFrameDefault(WindowBoundary start, WindowBoundary end);
	unique_ptr<WindowExpression> GetWindowClause(const Identifier &window_name);
	void SetQueryLocation(ParsedExpression &expr, QueryLocation query_location);
	void SetQueryLocation(TableRef &ref, QueryLocation query_location);

private:
	transform_result_ptr TransformInternal(ParseResult &parse_result);
	void SetResultLocation(ParseResult &parse_result, TransformResultValue &result);

	template <typename T>
	void SetResultLocation(T &, QueryLocation) {
	}
	void SetResultLocation(unique_ptr<ParsedExpression> &expr, QueryLocation location) {
		if (!expr) {
			return;
		}
		if (location.IsValid() && !expr->HasQueryLocation()) {
			SetQueryLocation(*expr, location);
		}
	}
	void SetResultLocation(unique_ptr<TableRef> &ref, QueryLocation location) {
		if (!ref) {
			return;
		}
		if (location.IsValid() && !ref->query_location.IsValid()) {
			SetQueryLocation(*ref, location);
		}
	}

public:
	ArenaAllocator &allocator;
	TokenIterator &token_iterator;
	identifier_map_t<idx_t> named_parameter_map;
	idx_t prepared_statement_parameter_index = 0;
	PreparedParamType last_param_type = PreparedParamType::INVALID;

	identifier_map_t<unique_ptr<WindowExpression>> window_clauses;

	vector<unique_ptr<CreatePivotEntry>> pivot_entries;
	vector<reference<CommonTableExpressionMap>> stored_cte_map;

	bool in_window_definition = false;
	bool has_anonymous_parameters = false;

	friend class StackChecker<PEGTransformer>;
	idx_t stack_depth = 0;

	StackChecker<PEGTransformer> StackCheck(idx_t extra_stack = 1) {
		if (stack_depth + extra_stack >= options.max_expression_depth) {
			throw ParserException(
			    "Max expression depth limit of %lld exceeded. Use \"SET max_expression_depth TO x\" to "
			    "increase the maximum expression depth.",
			    options.max_expression_depth);
		}
		return StackChecker<PEGTransformer>(*this, extra_stack);
	}

	ParserOptions options;
	const CompiledGrammar &grammar;

private:
	friend class GeneratedTransformProcess;
	friend class FinalizeTransformProcess;
	friend class TransformStack;
};

template <typename T>
inline unique_ptr<TypedTransformResult<T>> TryBridgeTransformResultValue(TransformResultValue &base_result) {
	return nullptr;
}

//! Transparent bridging between string-typed and Identifier-typed transform results.
template <>
inline unique_ptr<TypedTransformResult<string>>
TryBridgeTransformResultValue<string>(TransformResultValue &base_result) {
	if (auto *ident = TryGetTransformResult<Identifier>(base_result)) {
		return make_uniq<TypedTransformResult<string>>(ident->GetIdentifierName());
	}
	return nullptr;
}

template <>
inline unique_ptr<TypedTransformResult<Identifier>>
TryBridgeTransformResultValue<Identifier>(TransformResultValue &base_result) {
	if (auto *str = TryGetTransformResult<string>(base_result)) {
		return make_uniq<TypedTransformResult<Identifier>>(Identifier(*str));
	}
	return nullptr;
}

template <>
inline unique_ptr<TypedTransformResult<vector<string>>>
TryBridgeTransformResultValue<vector<string>>(TransformResultValue &base_result) {
	if (auto *idents = TryGetTransformResult<vector<Identifier>>(base_result)) {
		return make_uniq<TypedTransformResult<vector<string>>>(IdentifiersToStrings(*idents));
	}
	return nullptr;
}

template <>
inline unique_ptr<TypedTransformResult<vector<Identifier>>>
TryBridgeTransformResultValue<vector<Identifier>>(TransformResultValue &base_result) {
	if (auto *strs = TryGetTransformResult<vector<string>>(base_result)) {
		return make_uniq<TypedTransformResult<vector<Identifier>>>(StringsToIdentifiers(*strs));
	}
	return nullptr;
}

class PEGTransformerFactory {
public:
	static void RegisterDefaultTransforms(ParsedGrammar &grammar);

	//! Match a single TopLevelStatement from `tokens` starting at `token_cursor` and transform it
	//! into a SQLStatement. Returns nullptr if the matched TLS was separator-only (no statement).
	//! Throws on syntax error. `token_cursor` is in/out: it's the token index where matching
	//! starts, and on return holds the token index immediately past the last consumed token.
	static unique_ptr<SQLStatement> TransformTopLevelStatement(TokenIterator &token_iterator, ParserOptions &options,
	                                                           const CompiledGrammar &grammar);
	static ParseResult &ExtractResultFromParens(ParseResult &parse_result);
	static vector<reference<ParseResult>> ExtractParseResultsFromList(ParseResult &parse_result);
	static bool ExpressionIsEmptyStar(const ParsedExpression &expr);
	static QualifiedName StringToQualifiedName(vector<string> input);
	static QualifiedColumnName StringToQualifiedColumnName(const vector<string> &input);
	static LogicalType GetIntervalTargetType(DatePartSpecifier date_part);
	static void AddGroupByExpression(unique_ptr<ParsedExpression> expression, GroupingExpressionMap &map,
	                                 GroupByNode &result, vector<ProjectionIndex> &result_set);
	static vector<GroupingSet> GroupByExpressionUnfolding(GroupByExpressionInfo &group_by_expr,
	                                                      GroupingExpressionMap &map, GroupByNode &result);
	static unique_ptr<ResultModifier> VerifyLimitOffset(LimitPercentResult &limit, LimitPercentResult &offset);
	static unique_ptr<QueryNode> ToRecursiveCTE(unique_ptr<QueryNode> node, const Identifier &name,
	                                            vector<Identifier> &aliases,
	                                            vector<unique_ptr<ParsedExpression>> &key_targets);
	static void WrapRecursiveView(unique_ptr<CreateViewInfo> &info, unique_ptr<QueryNode> inner_node);
	static void ConvertToRecursiveView(unique_ptr<CreateViewInfo> &info, unique_ptr<QueryNode> &node);
	static void VerifyColumnRefs(const ParsedExpression &expr);
	static void RemoveOrderQualificationRecursive(unique_ptr<ParsedExpression> &root_expr);
	static void GetValueFromExpression(unique_ptr<ParsedExpression> &expr, vector<Value> &result);
	static bool TransformPivotInList(unique_ptr<ParsedExpression> &expr, PivotColumnEntry &entry);
	static void AddPivotEntry(PEGTransformer &transformer, string enum_name, unique_ptr<SelectNode> base,
	                          unique_ptr<ParsedExpression> column, unique_ptr<QueryNode> subquery, bool has_parameters);
	static Value GetConstantExpressionValue(unique_ptr<ParsedExpression> &expr);
	static void SplitGenericOptions(const vector<GenericCopyOption> &options_in,
	                                case_insensitive_map_t<unique_ptr<ParsedExpression>> &parsed_options,
	                                unordered_map<string, Value> &options, const char *statement_name);
	//! Fold `(k v, ...)` into unbound expressions, for statements that resolve every option at bind time
	//! instead of splitting literals out at parse time. Validates like SplitGenericOptions.
	static void CollectGenericOptions(const vector<GenericCopyOption> &options_in,
	                                  case_insensitive_map_t<unique_ptr<ParsedExpression>> &options,
	                                  const char *statement_name);
	static void AddToMultiStatement(const unique_ptr<MultiStatement> &multi_statement,
	                                unique_ptr<AlterInfo> alter_info);
	static void AddUpdateToMultiStatement(const unique_ptr<MultiStatement> &multi_statement, const string &column_name,
	                                      const AlterEntryData &table_data,
	                                      const unique_ptr<ParsedExpression> &original_expression);
	static unique_ptr<MultiStatement> TransformAndMaterializeAlter(AlterEntryData &data,
	                                                               unique_ptr<AlterInfo> info_with_null_placeholder,
	                                                               const string &column_name,
	                                                               unique_ptr<ParsedExpression> expression);

	static void InitializePivotStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePivotStatementTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeUnpivotStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUnpivotStatementTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeLiteralExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLiteralExpressionTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializePrefixExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePrefixExpressionTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeOverClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOverClauseTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeSelectStatementInternalTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSelectStatementInternalTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeSimpleSelectTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSimpleSelectTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeTableRefTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTableRefTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeWithClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWithClauseTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeWindowDefinitionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWindowDefinitionTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);

	//===--------------------------------------------------------------------===//
	// START GENERATED TRAMPOLINE RULES
	//===--------------------------------------------------------------------===//
	static void InitializeStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeStatementTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeIdentifierOrKeywordTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIdentifierOrKeywordTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeAlterStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAlterStatementTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeAlterOptionsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAlterOptionsTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeAlterTableStmtTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAlterTableStmtTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeAlterSchemaStmtTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAlterSchemaStmtTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeAlterTableOptionsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAlterTableOptionsTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeAddConstraintTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAddConstraintTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeDropConstraintTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDropConstraintTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeAddColumnTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAddColumnTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeAddColumnEntryTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAddColumnEntryTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeDropColumnTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDropColumnTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeAlterColumnTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAlterColumnTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeRenameColumnTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRenameColumnTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeNestedColumnNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNestedColumnNameTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeIdentifierDotTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIdentifierDotTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeRenameAlterTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRenameAlterTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeSetPartitionedByTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSetPartitionedByTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeResetPartitionedByTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeResetPartitionedByTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeSetSortedByTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSetSortedByTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeResetSortedByTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeResetSortedByTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeSetOptionsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSetOptionsTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeResetOptionsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeResetOptionsTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeAlterColumnEntryTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAlterColumnEntryTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeAddOrDropDefaultTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAddOrDropDefaultTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeAddDefaultTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAddDefaultTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeDropDefaultTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDropDefaultTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeChangeNullabilityTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeChangeNullabilityTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeDropOrSetTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDropOrSetTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeDropNullabilityTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDropNullabilityTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeSetNullabilityTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSetNullabilityTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeAlterTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAlterTypeTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeUsingExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUsingExpressionTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeAlterViewStmtTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAlterViewStmtTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeAlterSequenceStmtTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAlterSequenceStmtTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeQualifiedSequenceNameTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeQualifiedSequenceNameTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeAlterSequenceOptionsTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAlterSequenceOptionsTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeRenameAlterSequenceOptionsTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRenameAlterSequenceOptionsTrampoline(PEGTransformer &transformer,
	                                                                         GeneratedTransformProcess &process);
	static void InitializeSetSequenceOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSetSequenceOptionTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeAlterDatabaseStmtTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAlterDatabaseStmtTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeAnalyzeStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAnalyzeStatementTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeAnalyzeTargetTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAnalyzeTargetTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeAnalyzeVerboseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAnalyzeVerboseTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeAttachStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAttachStatementTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeDatabasePathTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDatabasePathTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeAttachAliasTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAttachAliasTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeAttachOptionsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAttachOptionsTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeCallStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCallStatementTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeCheckpointStatementTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCheckpointStatementTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeCheckpointForceTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCheckpointForceTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeCommentStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCommentStatementTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeCommentOnTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCommentOnTypeTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeCommentTableTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCommentTableTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeCommentSequenceTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCommentSequenceTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeCommentFunctionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCommentFunctionTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeCommentMacroTableTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCommentMacroTableTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeCommentMacroTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCommentMacroTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeCommentViewTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCommentViewTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeCommentDatabaseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCommentDatabaseTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeCommentIndexTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCommentIndexTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeCommentSchemaTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCommentSchemaTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeCommentTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCommentTypeTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeCommentColumnTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCommentColumnTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeCommentValueTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCommentValueTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeStringLiteralValueTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeStringLiteralValueTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeAnalyzeKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAnalyzeKeywordTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeExpressionStatementTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExpressionStatementTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeExpressionAliasTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExpressionAliasTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeIndexNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIndexNameTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeConstraintNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeConstraintNameTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeSequenceNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSequenceNameTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeCollationNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCollationNameTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeNumberLiteralTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNumberLiteralTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeStringLiteralTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeStringLiteralTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static void InitializeTypeVariationsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTypeVariationsTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeSimpleTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSimpleTypeTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeCharacterSimpleTypeTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCharacterSimpleTypeTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeQualifiedSimpleTypeTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeQualifiedSimpleTypeTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeIntervalTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIntervalTypeTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeIntervalIntervalTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIntervalIntervalTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeIntervalWithSpecifierTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIntervalWithSpecifierTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeIntervalWithRangeSpecifierTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIntervalWithRangeSpecifierTrampoline(PEGTransformer &transformer,
	                                                                         GeneratedTransformProcess &process);
	static void InitializeIntervalWithSimpleSpecifierTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIntervalWithSimpleSpecifierTrampoline(PEGTransformer &transformer,
	                                                                          GeneratedTransformProcess &process);
	static void InitializeIntervalWithoutSpecifierTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIntervalWithoutSpecifierTrampoline(PEGTransformer &transformer,
	                                                                       GeneratedTransformProcess &process);
	static void InitializeIntervalToIntervalAsTypeTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIntervalToIntervalAsTypeTrampoline(PEGTransformer &transformer,
	                                                                       GeneratedTransformProcess &process);
	static void InitializeYearKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeYearKeywordTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeMonthKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMonthKeywordTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeDayKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDayKeywordTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeHourKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeHourKeywordTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeMinuteKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMinuteKeywordTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeSecondKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSecondKeywordTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeMillisecondKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMillisecondKeywordTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeMicrosecondKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMicrosecondKeywordTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeWeekKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWeekKeywordTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeQuarterKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeQuarterKeywordTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeDecadeKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDecadeKeywordTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeCenturyKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCenturyKeywordTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeMillenniumKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMillenniumKeywordTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeIntervalTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIntervalTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeIntervalToIntervalTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIntervalToIntervalTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeYearToMonthTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeYearToMonthTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeDayToHourTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDayToHourTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeDayToMinuteTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDayToMinuteTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeDayToSecondTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDayToSecondTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeHourToMinuteTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeHourToMinuteTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeHourToSecondTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeHourToSecondTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeMinuteToSecondTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMinuteToSecondTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeBitTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBitTypeTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static void InitializeGeometryTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGeometryTypeTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeVariantTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeVariantTypeTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeNumericTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNumericTypeTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeSimpleNumericTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSimpleNumericTypeTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeDecimalNumericTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDecimalNumericTypeTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeIntTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIntTypeTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static void InitializeIntegerTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIntegerTypeTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeSmallintTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSmallintTypeTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeBigintTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBigintTypeTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeRealTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRealTypeTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeBooleanTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBooleanTypeTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeDoubleTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDoubleTypeTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeFloatTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFloatTypeTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeDecimalTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDecimalTypeTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeDecTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDecTypeTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static void InitializeNumericModTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNumericModTypeTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeQualifiedTypeNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeQualifiedTypeNameTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeTypeNameAsQualifiedNameTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTypeNameAsQualifiedNameTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeCatalogReservedSchemaTypeNameTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCatalogReservedSchemaTypeNameTrampoline(PEGTransformer &transformer,
	                                                                            GeneratedTransformProcess &process);
	static void InitializeSchemaReservedTypeNameTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSchemaReservedTypeNameTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeTypeModifiersTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTypeModifiersTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeRowTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRowTypeTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static void InitializeSetofTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSetofTypeTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeUnionTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUnionTypeTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeColIdTypeListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColIdTypeListTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeMapTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMapTypeTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static void InitializeTupleTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTupleTypeTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeColIdTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColIdTypeTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeArrayBoundsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeArrayBoundsTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeArrayKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeArrayKeywordTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeArrayKeywordWithBoundsTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeArrayKeywordWithBoundsTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeSquareBracketsArrayTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSquareBracketsArrayTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeTimeTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTimeTypeTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeTimeOrTimestampTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTimeOrTimestampTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeTimeTypeIdTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTimeTypeIdTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeTimestampTypeIdTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTimestampTypeIdTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeTimeZoneTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTimeZoneTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeWithOrWithoutTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWithOrWithoutTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeWithRuleTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWithRuleTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeWithoutRuleTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWithoutRuleTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeConnectStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeConnectStatementTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeDisconnectStatementTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDisconnectStatementTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeSessionTargetTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSessionTargetTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeLocalSessionTargetTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLocalSessionTargetTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeStringSessionTargetTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeStringSessionTargetTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeCatalogSessionTargetTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCatalogSessionTargetTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeCopyStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopyStatementTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeCopyVariationsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopyVariationsTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeCopyTableTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopyTableTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeFromOrToTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFromOrToTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeCopyFromTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopyFromTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeCopyToTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopyToTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static void InitializeCopySelectTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopySelectTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeCopyFileNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopyFileNameTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeCopyFileNameExpressionTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopyFileNameExpressionTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeCopyFileNameStringLiteralTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopyFileNameStringLiteralTrampoline(PEGTransformer &transformer,
	                                                                        GeneratedTransformProcess &process);
	static void InitializeCopyFileNameIdentifierTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopyFileNameIdentifierTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeCopyFileNameIdentifierColIdTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopyFileNameIdentifierColIdTrampoline(PEGTransformer &transformer,
	                                                                          GeneratedTransformProcess &process);
	static void InitializeIdentifierColIdTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIdentifierColIdTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeCopyFileNameSuffixTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopyFileNameSuffixTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeCopyOptionsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopyOptionsTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeCopyOptionListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopyOptionListTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeSpecializedOptionListTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSpecializedOptionListTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeSpecializedOptionTailTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSpecializedOptionTailTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeSpecializedOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSpecializedOptionTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeSingleOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSingleOptionTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeBinaryOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBinaryOptionTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeFreezeOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFreezeOptionTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeOidsOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOidsOptionTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeCsvOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCsvOptionTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeHeaderOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeHeaderOptionTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeNullAsOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNullAsOptionTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeDelimiterAsOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDelimiterAsOptionTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeQuoteAsOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeQuoteAsOptionTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeEscapeAsOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeEscapeAsOptionTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeEncodingOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeEncodingOptionTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeForceQuoteOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeForceQuoteOptionTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeStarSymbolColumnListTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeStarSymbolColumnListTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeForceQuoteTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeForceQuoteTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializePartitionByOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePartitionByOptionTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializePartitionByColumnListTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePartitionByColumnListTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeStarPartitionByColumnListTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeStarPartitionByColumnListTrampoline(PEGTransformer &transformer,
	                                                                        GeneratedTransformProcess &process);
	static void InitializeParenthesizedPartitionByColumnListTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static transform_result_ptr
	FinalizeParenthesizedPartitionByColumnListTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static void InitializeSinglePartitionByColumnListTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSinglePartitionByColumnListTrampoline(PEGTransformer &transformer,
	                                                                          GeneratedTransformProcess &process);
	static void InitializeForceNullOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeForceNullOptionTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeForceNotNullTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeForceNotNullTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeCopyGenericOptionListTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopyGenericOptionListTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeCopyGenericOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopyGenericOptionTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeOrderByCopyOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOrderByCopyOptionTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializePartitionedByCopyOptionTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePartitionedByCopyOptionTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeGenericCopyOptionListTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGenericCopyOptionListTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeGenericCopyOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGenericCopyOptionTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeGenericCopyOptionValueTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGenericCopyOptionValueTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeGenericCopyOptionOrderListTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGenericCopyOptionOrderListTrampoline(PEGTransformer &transformer,
	                                                                         GeneratedTransformProcess &process);
	static void InitializeGenericCopyOptionExpressionTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGenericCopyOptionExpressionTrampoline(PEGTransformer &transformer,
	                                                                          GeneratedTransformProcess &process);
	static void InitializeGenericCopyOptionParenthesizedExpressionListTrampoline(PEGTransformer &transformer,
	                                                                             GeneratedTransformProcess &process);
	static transform_result_ptr
	FinalizeGenericCopyOptionParenthesizedExpressionListTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeCopyFromDatabaseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopyFromDatabaseTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeCopyFromDatabaseWithFlagTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopyFromDatabaseWithFlagTrampoline(PEGTransformer &transformer,
	                                                                       GeneratedTransformProcess &process);
	static void InitializeCopyFromDatabaseWithoutFlagTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopyFromDatabaseWithoutFlagTrampoline(PEGTransformer &transformer,
	                                                                          GeneratedTransformProcess &process);
	static void InitializeCopyDatabaseFlagTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopyDatabaseFlagTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeSchemaOrDataTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSchemaOrDataTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeCopySchemaTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopySchemaTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeCopyDataTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCopyDataTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeCreateIndexStmtTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateIndexStmtTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeWithListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWithListTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeRelOptionOrOidsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRelOptionOrOidsTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeRelOptionListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRelOptionListTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeOidsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOidsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static void InitializeWithOrWithoutOidsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWithOrWithoutOidsTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeWithOidsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWithOidsTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeWithoutOidsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWithoutOidsTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeIndexElementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIndexElementTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeUniqueIndexTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUniqueIndexTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeIndexTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIndexTypeTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeRelOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRelOptionTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeRelOptionNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRelOptionNameTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeDottedIdentifierStringTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDottedIdentifierStringTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeRelOptionArgumentOptTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRelOptionArgumentOptTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeDefArgTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDefArgTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static void InitializeDefArgNullTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDefArgNullTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeDefArgKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDefArgKeywordTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeDefArgStringLiteralTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDefArgStringLiteralTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeNoneLiteralTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNoneLiteralTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeCreateMacroStmtTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateMacroStmtTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeMacroOrFunctionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMacroOrFunctionTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeMacroKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMacroKeywordTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeFunctionKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFunctionKeywordTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeMacroDefinitionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMacroDefinitionTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeMacroDefinitionBodyTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMacroDefinitionBodyTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeMacroParametersTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMacroParametersTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeMacroParameterTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMacroParameterTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeSimpleParameterTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSimpleParameterTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeScalarMacroDefinitionTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeScalarMacroDefinitionTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeTableMacroDefinitionTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTableMacroDefinitionTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeCreateSchemaStmtTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateSchemaStmtTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeCreateSecretStmtTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateSecretStmtTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeSecretStorageSpecifierTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSecretStorageSpecifierTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeSecretNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSecretNameTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeCreateSequenceStmtTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateSequenceStmtTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeSequenceOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSequenceOptionTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeSeqSetCycleTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSeqSetCycleTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeSeqCycleTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSeqCycleTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeSeqNoCycleTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSeqNoCycleTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeSeqSetIncrementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSeqSetIncrementTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeSeqSetMinMaxTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSeqSetMinMaxTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeSeqNoMinMaxTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSeqNoMinMaxTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeSeqStartWithTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSeqStartWithTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeSeqOwnedByTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSeqOwnedByTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeSeqMinOrMaxTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSeqMinOrMaxTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeMinValueTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMinValueTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeMaxValueTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMaxValueTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeCreateStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateStatementTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeCreateStatementVariationTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateStatementVariationTrampoline(PEGTransformer &transformer,
	                                                                       GeneratedTransformProcess &process);
	static void InitializeOrReplaceTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOrReplaceTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeTemporaryTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTemporaryTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializePersistentTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePersistentTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeTempPersistentTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTempPersistentTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeTemporaryPersistentTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTemporaryPersistentTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeCreateTableStmtTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateTableStmtTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeCreateTableDefinitionTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateTableDefinitionTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeCreateTableAsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateTableAsTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializePartitionSortedOptionsTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePartitionSortedOptionsTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializePartitionOptSortedOptionsTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePartitionOptSortedOptionsTrampoline(PEGTransformer &transformer,
	                                                                        GeneratedTransformProcess &process);
	static void InitializeSortedOptPartitionOptionsTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSortedOptPartitionOptionsTrampoline(PEGTransformer &transformer,
	                                                                        GeneratedTransformProcess &process);
	static void InitializePartitionOptionsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePartitionOptionsTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeSortedOptionsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSortedOptionsTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeWithDataTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWithDataTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeWithDataOnlyTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWithDataOnlyTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeWithNoDataTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWithNoDataTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeIdentifierListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIdentifierListTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeCreateColumnListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateColumnListTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeIfNotExistsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIfNotExistsTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeQualifiedNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeQualifiedNameTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeSchemaReservedIdentifierOrStringLiteralTrampoline(PEGTransformer &transformer,
	                                                                        GeneratedTransformProcess &process);
	static transform_result_ptr
	FinalizeSchemaReservedIdentifierOrStringLiteralTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeCatalogReservedSchemaIdentifierTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCatalogReservedSchemaIdentifierTrampoline(PEGTransformer &transformer,
	                                                                              GeneratedTransformProcess &process);
	static void InitializeIdentifierOrStringLiteralTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIdentifierOrStringLiteralTrampoline(PEGTransformer &transformer,
	                                                                        GeneratedTransformProcess &process);
	static void InitializeReservedIdentifierOrStringLiteralTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeReservedIdentifierOrStringLiteralTrampoline(PEGTransformer &transformer,
	                                                                                GeneratedTransformProcess &process);
	static void InitializeCatalogQualificationTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCatalogQualificationTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeSchemaQualificationTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSchemaQualificationTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeReservedSchemaQualificationTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeReservedSchemaQualificationTrampoline(PEGTransformer &transformer,
	                                                                          GeneratedTransformProcess &process);
	static void InitializeTableQualificationTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTableQualificationTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeReservedTableQualificationTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeReservedTableQualificationTrampoline(PEGTransformer &transformer,
	                                                                         GeneratedTransformProcess &process);
	static void InitializeCreateTableColumnListTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateTableColumnListTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeCreateTableColumnElementTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateTableColumnElementTrampoline(PEGTransformer &transformer,
	                                                                       GeneratedTransformProcess &process);
	static void InitializeCreateTableColumnDefinitionTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateTableColumnDefinitionTrampoline(PEGTransformer &transformer,
	                                                                          GeneratedTransformProcess &process);
	static void InitializeCreateTableConstraintTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateTableConstraintTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeColumnDefinitionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColumnDefinitionTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeColumnConstraintTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColumnConstraintTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeNotNullConstraintTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNotNullConstraintTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeNullConstraintTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNullConstraintTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeNotNullColumnConstraintTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNotNullColumnConstraintTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeUniqueConstraintTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUniqueConstraintTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializePrimaryKeyConstraintTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePrimaryKeyConstraintTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeDefaultValueTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDefaultValueTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeCheckConstraintTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCheckConstraintTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeForeignKeyConstraintTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeForeignKeyConstraintTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeColumnCollationTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColumnCollationTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeColumnCompressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColumnCompressionTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeKeyActionsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeKeyActionsTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeUpdateActionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUpdateActionTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeDeleteActionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDeleteActionTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeKeyActionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeKeyActionTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeNoKeyActionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNoKeyActionTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeRestrictKeyActionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRestrictKeyActionTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeCascadeKeyActionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCascadeKeyActionTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeSetNullKeyActionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSetNullKeyActionTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeSetDefaultKeyActionTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSetDefaultKeyActionTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeTopLevelConstraintTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTopLevelConstraintTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeTopLevelConstraintListTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTopLevelConstraintListTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeTopCheckConstraintTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTopCheckConstraintTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeTopPrimaryKeyConstraintTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTopPrimaryKeyConstraintTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeTopUniqueConstraintTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTopUniqueConstraintTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeTopForeignKeyConstraintTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTopForeignKeyConstraintTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeColumnIdListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColumnIdListTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeDottedIdentifierTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDottedIdentifierTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeDotColLabelTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDotColLabelTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeIdentifierTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIdentifierTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeColIdTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColIdTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static void InitializeColIdOrStringTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColIdOrStringTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeTypeFuncNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTypeFuncNameTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeTypeFuncKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTypeFuncKeywordTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeColLabelTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColLabelTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeColLabelOrStringTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColLabelOrStringTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeColLabelIdentifierTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColLabelIdentifierTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeStringLiteralIdentifierTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeStringLiteralIdentifierTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeGeneratedColumnTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGeneratedColumnTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeGeneratedColumnTypeTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGeneratedColumnTypeTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeCommitActionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCommitActionTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializePreserveOrDeleteTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePreserveOrDeleteTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializePreserveRowsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePreserveRowsTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeDeleteRowsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDeleteRowsTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeVirtualGeneratedColumnTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeVirtualGeneratedColumnTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeStoredGeneratedColumnTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeStoredGeneratedColumnTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeCreateTriggerStmtTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateTriggerStmtTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeTriggerBodyTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTriggerBodyTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeTriggerNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTriggerNameTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeReferencingClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeReferencingClauseTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeReferencingItemTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeReferencingItemTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeReferencingNewTableAsTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeReferencingNewTableAsTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeReferencingOldTableAsTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeReferencingOldTableAsTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeTriggerTimingTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTriggerTimingTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeTriggerBeforeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTriggerBeforeTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeTriggerAfterTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTriggerAfterTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeTriggerInsteadOfTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTriggerInsteadOfTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeTriggerEventTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTriggerEventTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeTriggerEventInsertTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTriggerEventInsertTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeTriggerEventDeleteTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTriggerEventDeleteTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeTriggerEventUpdateTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTriggerEventUpdateTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeTriggerEventUpdateOfTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTriggerEventUpdateOfTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeTriggerColumnListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTriggerColumnListTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeForEachClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeForEachClauseTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeForEachRowTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeForEachRowTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeForEachStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeForEachStatementTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeCreateTypeStmtTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateTypeStmtTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeCreateTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateTypeTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeCreateTypeFromTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateTypeFromTypeTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeEnumSelectTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeEnumSelectTypeTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeEnumStringLiteralListTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeEnumStringLiteralListTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeCreateViewStmtTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateViewStmtTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeCreateRecursiveTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateRecursiveTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeCreateSecureTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateSecureTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeDeallocateStatementTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDeallocateStatementTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeDeallocatePrepareTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDeallocatePrepareTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeDeleteStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDeleteStatementTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeTruncateStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTruncateStatementTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeTargetOptAliasTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTargetOptAliasTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeDeleteUsingClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDeleteUsingClauseTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeDescribeStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDescribeStatementTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeShowDeprecatedSelectTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeShowDeprecatedSelectTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeDescribeSelectTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDescribeSelectTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeShowAllTablesTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeShowAllTablesTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeShowTablesTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeShowTablesTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeShowByNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeShowByNameTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeDescribeByNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDescribeByNameTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeDescribeOrSummarizeTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDescribeOrSummarizeTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeShowTargetTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeShowTargetTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeShowDeprecatedQualifiedTableNameTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeShowDeprecatedQualifiedTableNameTrampoline(PEGTransformer &transformer,
	                                                                               GeneratedTransformProcess &process);
	static void InitializeShowSettingNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeShowSettingNameTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeDescribeTargetTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDescribeTargetTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeDescribeBaseTableNameTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDescribeBaseTableNameTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeDescribeStringLiteralTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDescribeStringLiteralTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeSummarizeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSummarizeTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeSummarizeRuleTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSummarizeRuleTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeShowOrDescribeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeShowOrDescribeTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeShowRuleTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeShowRuleTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeDescribeRuleTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDescribeRuleTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeDescribeLongRuleTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDescribeLongRuleTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeDescRuleTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDescRuleTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeDetachStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDetachStatementTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeDropStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDropStatementTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeDropEntriesTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDropEntriesTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeDropTriggerTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDropTriggerTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeDropTableTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDropTableTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeDropTableFunctionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDropTableFunctionTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeDropFunctionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDropFunctionTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeDropSchemaTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDropSchemaTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeDropIndexTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDropIndexTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeQualifiedIndexNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeQualifiedIndexNameTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeQualifiedIndexNameStringTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeQualifiedIndexNameStringTrampoline(PEGTransformer &transformer,
	                                                                       GeneratedTransformProcess &process);
	static void InitializeSchemaReservedIndexTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSchemaReservedIndexTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeCatalogReservedSchemaIndexTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCatalogReservedSchemaIndexTrampoline(PEGTransformer &transformer,
	                                                                         GeneratedTransformProcess &process);
	static void InitializeDropSequenceTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDropSequenceTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeDropCollationTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDropCollationTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeDropTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDropTypeTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeDropSecretTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDropSecretTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeTableOrViewTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTableOrViewTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeMaterializedViewEntryTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMaterializedViewEntryTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeFunctionTypeMacroTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFunctionTypeMacroTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeFunctionTypeMacroKeywordTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFunctionTypeMacroKeywordTrampoline(PEGTransformer &transformer,
	                                                                       GeneratedTransformProcess &process);
	static void InitializeFunctionTypeFunctionTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFunctionTypeFunctionTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeDropBehaviorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDropBehaviorTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeCascadeDropBehaviorTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCascadeDropBehaviorTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeRestrictDropBehaviorTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRestrictDropBehaviorTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeIfExistsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIfExistsTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeDropSecretStorageTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDropSecretStorageTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeExecuteStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExecuteStatementTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeExplainStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExplainStatementTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeExplainOptionListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExplainOptionListTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeExplainOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExplainOptionTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeExplainOptionNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExplainOptionNameTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeExplainSelectStatementTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExplainSelectStatementTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeExplainableStatementsTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExplainableStatementsTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeExportStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExportStatementTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeExportSourceTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExportSourceTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeImportStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeImportStatementTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeColumnReferenceTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColumnReferenceTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeNestedSchemaTableColumnNameTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNestedSchemaTableColumnNameTrampoline(PEGTransformer &transformer,
	                                                                          GeneratedTransformProcess &process);
	static void InitializeCatalogReservedSchemaTableColumnNameTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static transform_result_ptr
	FinalizeCatalogReservedSchemaTableColumnNameTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeSchemaReservedTableColumnNameTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSchemaReservedTableColumnNameTrampoline(PEGTransformer &transformer,
	                                                                            GeneratedTransformProcess &process);
	static void InitializeTableReservedColumnNameTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTableReservedColumnNameTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeFunctionExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFunctionExpressionTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeFunctionExpressionArgumentsTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFunctionExpressionArgumentsTrampoline(PEGTransformer &transformer,
	                                                                          GeneratedTransformProcess &process);
	static void InitializeFunctionExpressionArgumentListTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFunctionExpressionArgumentListTrampoline(PEGTransformer &transformer,
	                                                                             GeneratedTransformProcess &process);
	static void InitializeFunctionArgumentListTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFunctionArgumentListTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeFunctionIdentifierTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFunctionIdentifierTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeFunctionNameAsQualifiedNameTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFunctionNameAsQualifiedNameTrampoline(PEGTransformer &transformer,
	                                                                          GeneratedTransformProcess &process);
	static void InitializeCatalogReservedSchemaFunctionNameTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCatalogReservedSchemaFunctionNameTrampoline(PEGTransformer &transformer,
	                                                                                GeneratedTransformProcess &process);
	static void InitializeSchemaReservedFunctionNameTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSchemaReservedFunctionNameTrampoline(PEGTransformer &transformer,
	                                                                         GeneratedTransformProcess &process);
	static void InitializeDistinctOrAllTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDistinctOrAllTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeDistinctKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDistinctKeywordTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeAllKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAllKeywordTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeWithinGroupClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWithinGroupClauseTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeFilterClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFilterClauseTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeFilterClauseExpressionTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFilterClauseExpressionTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeFilterClauseContentsTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFilterClauseContentsTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeIgnoreOrRespectNullsTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIgnoreOrRespectNullsTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeIgnoreNullsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIgnoreNullsTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeRespectNullsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRespectNullsTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeParenthesisExpressionTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeParenthesisExpressionTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeConstantLiteralTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeConstantLiteralTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeNullLiteralTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNullLiteralTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeTrueLiteralTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTrueLiteralTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeFalseLiteralTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFalseLiteralTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeCastExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCastExpressionTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeCastArgumentsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCastArgumentsTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeCastOrTryCastTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCastOrTryCastTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeCastKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCastKeywordTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeTryCastKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTryCastKeywordTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeColIdDotTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColIdDotTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeStarExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeStarExpressionTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeStarQualifierListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeStarQualifierListTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeExcludeListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExcludeListTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeExcludeNamesTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExcludeNamesTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeExcludeNameListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExcludeNameListTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeExcludeNameSingleTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExcludeNameSingleTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeExcludeNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExcludeNameTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeExcludeDottedNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExcludeDottedNameTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeExcludeColumnNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExcludeColumnNameTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeReplaceListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeReplaceListTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeReplaceEntriesTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeReplaceEntriesTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeReplaceEntrySingleTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeReplaceEntrySingleTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeReplaceEntryListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeReplaceEntryListTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeReplaceEntryTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeReplaceEntryTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeRenameListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRenameListTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeRenameEntriesTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRenameEntriesTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeRenameEntryListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRenameEntryListTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeSingleRenameEntryTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSingleRenameEntryTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeRenameEntryTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRenameEntryTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeSubqueryExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSubqueryExpressionTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeSubqueryNotTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSubqueryNotTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeSubqueryExistsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSubqueryExistsTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeCaseExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCaseExpressionTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeCaseWhenThenTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCaseWhenThenTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeCaseElseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCaseElseTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeTypeLiteralTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTypeLiteralTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeIntervalLiteralTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIntervalLiteralTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeIntervalParameterTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIntervalParameterTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeIntervalStringParameterTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIntervalStringParameterTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeFrameClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFrameClauseTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeFramingTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFramingTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static void InitializeRowsFramingTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRowsFramingTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeRangeFramingTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRangeFramingTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeGroupsFramingTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGroupsFramingTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeFrameExtentTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFrameExtentTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeSingleFrameExtentTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSingleFrameExtentTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeBetweenFrameExtentTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBetweenFrameExtentTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeFrameBoundTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFrameBoundTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeFrameUnboundedTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFrameUnboundedTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeFrameExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFrameExpressionTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeFrameCurrentRowTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFrameCurrentRowTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializePrecedingOrFollowingTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePrecedingOrFollowingTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializePrecedingFrameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePrecedingFrameTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeFollowingFrameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFollowingFrameTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeWindowExcludeClauseTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWindowExcludeClauseTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeWindowExcludeElementTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWindowExcludeElementTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeExcludeCurrentRowTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExcludeCurrentRowTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeExcludeGroupTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExcludeGroupTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeExcludeTiesTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExcludeTiesTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeExcludeNoOthersTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExcludeNoOthersTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeWindowFrameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWindowFrameTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeIdentifierWindowFrameTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIdentifierWindowFrameTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeParensIdentifierTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeParensIdentifierTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeWindowFrameDefinitionTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWindowFrameDefinitionTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeWindowFrameNameContentsParensTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWindowFrameNameContentsParensTrampoline(PEGTransformer &transformer,
	                                                                            GeneratedTransformProcess &process);
	static void InitializeWindowFrameNameContentsTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWindowFrameNameContentsTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeWindowFrameContentsParensTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWindowFrameContentsParensTrampoline(PEGTransformer &transformer,
	                                                                        GeneratedTransformProcess &process);
	static void InitializeWindowFrameContentsTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWindowFrameContentsTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeBaseWindowNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBaseWindowNameTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeWindowPartitionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWindowPartitionTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeListExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeListExpressionTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeArrayBoundedListExpressionTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeArrayBoundedListExpressionTrampoline(PEGTransformer &transformer,
	                                                                         GeneratedTransformProcess &process);
	static void InitializeArrayParensSelectTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeArrayParensSelectTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeBoundedListExpressionTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBoundedListExpressionTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeStructExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeStructExpressionTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeStructFieldTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeStructFieldTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeMapExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMapExpressionTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeMapStructExpressionTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMapStructExpressionTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeMapStructFieldTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMapStructFieldTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeGroupingExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGroupingExpressionTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeGroupingOrGroupingIdTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGroupingOrGroupingIdTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeGroupingKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGroupingKeywordTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeGroupingIdKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGroupingIdKeywordTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeParameterTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeParameterTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeQuestionMarkNumberedParameterTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeQuestionMarkNumberedParameterTrampoline(PEGTransformer &transformer,
	                                                                            GeneratedTransformProcess &process);
	static void InitializeAnonymousParameterTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAnonymousParameterTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeNumberedParameterTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNumberedParameterTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeColLabelParameterTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColLabelParameterTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializePositionalExpressionTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePositionalExpressionTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeDefaultExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDefaultExpressionTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeListComprehensionExpressionTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeListComprehensionExpressionTrampoline(PEGTransformer &transformer,
	                                                                          GeneratedTransformProcess &process);
	static void InitializeListComprehensionFilterTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeListComprehensionFilterTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeParensExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeParensExpressionTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeSingleExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSingleExpressionTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExpressionTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeColumnDefaultExprTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColumnDefaultExprTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeLambdaArrowExpressionTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLambdaArrowExpressionTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeSingleArrowPairTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSingleArrowPairTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeLogicalOrExpressionTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLogicalOrExpressionTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeLogicalOrExpressionTailTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLogicalOrExpressionTailTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeColDefOrExprTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColDefOrExprTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeColDefOrExpressionTailTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColDefOrExpressionTailTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeLogicalAndExpressionTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLogicalAndExpressionTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeLogicalAndExpressionTailTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLogicalAndExpressionTailTrampoline(PEGTransformer &transformer,
	                                                                       GeneratedTransformProcess &process);
	static void InitializeColDefAndExprTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColDefAndExprTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeColDefAndExpressionTailTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColDefAndExpressionTailTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeLogicalNotExpressionTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLogicalNotExpressionTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeNotExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNotExpressionTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeNotKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNotKeywordTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeIsExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIsExpressionTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeIsTestTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIsTestTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static void InitializeIsLiteralTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIsLiteralTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeIsLiteralValueTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIsLiteralValueTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeUnknownLiteralTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUnknownLiteralTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeNotNullTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNotNullTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static void InitializeNotNullKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNotNullKeywordTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeNotNullOperatorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNotNullOperatorTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeIsNullTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIsNullTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static void InitializeIsNullOperatorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIsNullOperatorTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeIsDistinctFromExpressionTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIsDistinctFromExpressionTrampoline(PEGTransformer &transformer,
	                                                                       GeneratedTransformProcess &process);
	static void InitializeIsDistinctFromTailTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIsDistinctFromTailTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeIsDistinctFromOpTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIsDistinctFromOpTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeComparisonExpressionTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeComparisonExpressionTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeComparisonExpressionTailTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeComparisonExpressionTailTrampoline(PEGTransformer &transformer,
	                                                                       GeneratedTransformProcess &process);
	static void InitializeComparisonOperatorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeComparisonOperatorTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeOperatorEqualTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOperatorEqualTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeOperatorNotEqualTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOperatorNotEqualTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeOperatorLessThanTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOperatorLessThanTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeOperatorGreaterThanTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOperatorGreaterThanTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeOperatorLessThanEqualsTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOperatorLessThanEqualsTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeOperatorGreaterThanEqualsTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOperatorGreaterThanEqualsTrampoline(PEGTransformer &transformer,
	                                                                        GeneratedTransformProcess &process);
	static void InitializeBetweenInLikeExpressionTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBetweenInLikeExpressionTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeBetweenInLikeOpTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBetweenInLikeOpTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeBetweenInLikeOpExpressionTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBetweenInLikeOpExpressionTrampoline(PEGTransformer &transformer,
	                                                                        GeneratedTransformProcess &process);
	static void InitializeLikeClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLikeClauseTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeEscapeClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeEscapeClauseTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeLikeVariationsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLikeVariationsTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeLikeTokenTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLikeTokenTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeILikeTokenTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeILikeTokenTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeGlobTokenTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGlobTokenTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeSimilarToTokenTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSimilarToTokenTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeRegexMatchTokenTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRegexMatchTokenTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeRegexInsensitiveMatchTokenTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRegexInsensitiveMatchTokenTrampoline(PEGTransformer &transformer,
	                                                                         GeneratedTransformProcess &process);
	static void InitializeNotILikeOpTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNotILikeOpTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeNotLikeOpTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNotLikeOpTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeNotRegexInsensitiveMatchOpTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNotRegexInsensitiveMatchOpTrampoline(PEGTransformer &transformer,
	                                                                         GeneratedTransformProcess &process);
	static void InitializeNotSimilarToOpTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNotSimilarToOpTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeInClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInClauseTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeInExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInExpressionTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeInContainsExpressionTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInContainsExpressionTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeInExpressionListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInExpressionListTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeInSelectStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInSelectStatementTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeBetweenClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBetweenClauseTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeOtherOperatorExpressionTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOtherOperatorExpressionTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeOtherOperatorTailTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOtherOperatorTailTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeOtherOperatorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOtherOperatorTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeAnyAllParsedOperatorTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAnyAllParsedOperatorTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeNamedOtherOperatorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNamedOtherOperatorTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeOperatorLiteralTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOperatorLiteralTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeAnyAllOperatorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAnyAllOperatorTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeAnyOrAllTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAnyOrAllTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeSubqueryAnyTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSubqueryAnyTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeSubqueryAllTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSubqueryAllTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeInetOperatorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInetOperatorTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeJsonOperatorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeJsonOperatorTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeListOperatorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeListOperatorTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeStringOperatorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeStringOperatorTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeQualifiedOperatorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeQualifiedOperatorTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeQualifiedOperatorContentsTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeQualifiedOperatorContentsTrampoline(PEGTransformer &transformer,
	                                                                        GeneratedTransformProcess &process);
	static void InitializeAnyOpTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAnyOpTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static void InitializeBitwiseExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBitwiseExpressionTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeBitwiseExpressionTailTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBitwiseExpressionTailTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeBitOperatorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBitOperatorTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeAdditiveExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAdditiveExpressionTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeAdditiveExpressionTailTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAdditiveExpressionTailTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeTermTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTermTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static void InitializeMultiplicativeExpressionTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMultiplicativeExpressionTrampoline(PEGTransformer &transformer,
	                                                                       GeneratedTransformProcess &process);
	static void InitializeMultiplicativeExpressionTailTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMultiplicativeExpressionTailTrampoline(PEGTransformer &transformer,
	                                                                           GeneratedTransformProcess &process);
	static void InitializeFactorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFactorTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static void InitializeExponentiationExpressionTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExponentiationExpressionTrampoline(PEGTransformer &transformer,
	                                                                       GeneratedTransformProcess &process);
	static void InitializeExponentiationExpressionTailTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExponentiationExpressionTailTrampoline(PEGTransformer &transformer,
	                                                                           GeneratedTransformProcess &process);
	static void InitializeExponentOperatorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExponentOperatorTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeCollateExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCollateExpressionTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeCollateExpressionTailTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCollateExpressionTailTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeAtTimeZoneExpressionTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAtTimeZoneExpressionTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeAtTimeZoneExpressionTailTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAtTimeZoneExpressionTailTrampoline(PEGTransformer &transformer,
	                                                                       GeneratedTransformProcess &process);
	static void InitializePrefixOperatorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePrefixOperatorTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeMinusPrefixOperatorTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMinusPrefixOperatorTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializePlusPrefixOperatorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePlusPrefixOperatorTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeTildePrefixOperatorTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTildePrefixOperatorTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeBaseExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBaseExpressionTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeIndirectionListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIndirectionListTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeIndirectionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIndirectionTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeCastOperatorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCastOperatorTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeDotOperatorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDotOperatorTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeDotMethodOperatorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDotMethodOperatorTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeDotColumnOperatorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDotColumnOperatorTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeMethodExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMethodExpressionTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeMethodExpressionArgumentsTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMethodExpressionArgumentsTrampoline(PEGTransformer &transformer,
	                                                                        GeneratedTransformProcess &process);
	static void InitializeMethodExpressionArgumentListTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMethodExpressionArgumentListTrampoline(PEGTransformer &transformer,
	                                                                           GeneratedTransformProcess &process);
	static void InitializeMethodFunctionArgumentsTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMethodFunctionArgumentsTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeSliceExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSliceExpressionTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeSliceBoundTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSliceBoundTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeEndSliceBoundTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeEndSliceBoundTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeEndSliceValueTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeEndSliceValueTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeEndSliceMinusTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeEndSliceMinusTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeStepSliceBoundTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeStepSliceBoundTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializePostfixOperatorTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePostfixOperatorTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeSpecialFunctionExpressionTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSpecialFunctionExpressionTrampoline(PEGTransformer &transformer,
	                                                                        GeneratedTransformProcess &process);
	static void InitializeCoalesceExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCoalesceExpressionTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeUnpackExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUnpackExpressionTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeTryExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTryExpressionTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeColumnsExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColumnsExpressionTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeExtractExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExtractExpressionTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeExtractArgumentsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExtractArgumentsTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeLambdaExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLambdaExpressionTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeNullIfExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNullIfExpressionTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeNullIfArgumentsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNullIfArgumentsTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializePositionExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePositionExpressionTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializePositionArgumentsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePositionArgumentsTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeRowExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRowExpressionTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeSubstringExpressionTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSubstringExpressionTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeSubstringArgumentsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSubstringArgumentsTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeSubstringExpressionListTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSubstringExpressionListTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeSubstringParametersTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSubstringParametersTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeSubstringFromForTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSubstringFromForTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeSubstringFromOptionalForTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSubstringFromOptionalForTrampoline(PEGTransformer &transformer,
	                                                                       GeneratedTransformProcess &process);
	static void InitializeSubstringForTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSubstringForTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeTrimExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTrimExpressionTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeTrimArgumentsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTrimArgumentsTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeTrimDirectionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTrimDirectionTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeTrimBothTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTrimBothTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeTrimLeadingTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTrimLeadingTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeTrimTrailingTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTrimTrailingTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeTrimSourceTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTrimSourceTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeOverlayExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOverlayExpressionTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeOverlayArgumentsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOverlayArgumentsTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeOverlayParametersTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOverlayParametersTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeFromExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFromExpressionTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeForExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeForExpressionTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeOverlayExpressionListTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOverlayExpressionListTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeExtractArgumentTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExtractArgumentTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeExtractDatePartArgumentTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExtractDatePartArgumentTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeExtractIdentifierArgumentTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExtractIdentifierArgumentTrampoline(PEGTransformer &transformer,
	                                                                        GeneratedTransformProcess &process);
	static void InitializeExtractStringArgumentTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExtractStringArgumentTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeExtractDatePartTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExtractDatePartTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeExternalResourceStatementTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExternalResourceStatementTrampoline(PEGTransformer &transformer,
	                                                                        GeneratedTransformProcess &process);
	static void InitializeCreateExternalResourceStmtTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateExternalResourceStmtTrampoline(PEGTransformer &transformer,
	                                                                         GeneratedTransformProcess &process);
	static void InitializeRegisterExternalResourceStmtTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRegisterExternalResourceStmtTrampoline(PEGTransformer &transformer,
	                                                                           GeneratedTransformProcess &process);
	static void InitializeDestroyExternalResourceStmtTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDestroyExternalResourceStmtTrampoline(PEGTransformer &transformer,
	                                                                          GeneratedTransformProcess &process);
	static void InitializeShowExternalResourcesStmtTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeShowExternalResourcesStmtTrampoline(PEGTransformer &transformer,
	                                                                        GeneratedTransformProcess &process);
	static void InitializeShowAllModifierTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeShowAllModifierTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeExternalResourceCreationOptionsTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExternalResourceCreationOptionsTrampoline(PEGTransformer &transformer,
	                                                                              GeneratedTransformProcess &process);
	static void InitializeAttachToExternalResourceTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAttachToExternalResourceTrampoline(PEGTransformer &transformer,
	                                                                       GeneratedTransformProcess &process);
	static void InitializeConnectToExternalResourceTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeConnectToExternalResourceTrampoline(PEGTransformer &transformer,
	                                                                        GeneratedTransformProcess &process);
	static void InitializeExternalResourceSourceTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExternalResourceSourceTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeExternalResourceCreateClauseTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExternalResourceCreateClauseTrampoline(PEGTransformer &transformer,
	                                                                           GeneratedTransformProcess &process);
	static void InitializeExternalResourceReferenceClauseTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExternalResourceReferenceClauseTrampoline(PEGTransformer &transformer,
	                                                                              GeneratedTransformProcess &process);
	static void InitializeInsertStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInsertStatementTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeOrActionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOrActionTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeInsertOrReplaceTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInsertOrReplaceTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeInsertOrIgnoreTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInsertOrIgnoreTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeByNameOrPositionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeByNameOrPositionTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeInsertByNameOrderTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInsertByNameOrderTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeInsertByPositionOrderTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInsertByPositionOrderTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeInsertByNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInsertByNameTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeInsertByPositionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInsertByPositionTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeInsertTargetTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInsertTargetTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeInsertAliasTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInsertAliasTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeColumnListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColumnListTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeInsertColumnListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInsertColumnListTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeInsertValuesTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInsertValuesTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeSelectInsertValuesTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSelectInsertValuesTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeDefaultValuesTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDefaultValuesTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeOnConflictClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOnConflictClauseTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeOnConflictTargetTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOnConflictTargetTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeOnConflictExpressionTargetTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOnConflictExpressionTargetTrampoline(PEGTransformer &transformer,
	                                                                         GeneratedTransformProcess &process);
	static void InitializeOnConflictIndexTargetTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOnConflictIndexTargetTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeOnConflictActionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOnConflictActionTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeOnConflictUpdateTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOnConflictUpdateTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeOnConflictNothingTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOnConflictNothingTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeReturningClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeReturningClauseTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeLoadStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLoadStatementTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeExtensionAliasTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExtensionAliasTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeInstallStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInstallStatementTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeInstallAndLoadTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInstallAndLoadTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeUpdateExtensionsStatementTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUpdateExtensionsStatementTrampoline(PEGTransformer &transformer,
	                                                                        GeneratedTransformProcess &process);
	static void InitializeFromSourceTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFromSourceTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeFromSourceIdentifierTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFromSourceIdentifierTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeFromSourceStringTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFromSourceStringTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeVersionNumberTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeVersionNumberTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeExtensionRepositoryStatementTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExtensionRepositoryStatementTrampoline(PEGTransformer &transformer,
	                                                                           GeneratedTransformProcess &process);
	static void InitializeCreateExtensionRepositoryStmtTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCreateExtensionRepositoryStmtTrampoline(PEGTransformer &transformer,
	                                                                            GeneratedTransformProcess &process);
	static void InitializeRepositoryPrefixTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRepositoryPrefixTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeRepositoryPublicKeyTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRepositoryPublicKeyTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeDropExtensionRepositoryStmtTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDropExtensionRepositoryStmtTrampoline(PEGTransformer &transformer,
	                                                                          GeneratedTransformProcess &process);
	static void InitializeMergeIntoStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMergeIntoStatementTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeMergeIntoUsingClauseTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMergeIntoUsingClauseTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeMergeMatchTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMergeMatchTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeMatchedClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMatchedClauseTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeMatchedClauseActionTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMatchedClauseActionTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeUpdateMatchClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUpdateMatchClauseTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeUpdateMatchInfoTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUpdateMatchInfoTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeUpdateMatchSetActionTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUpdateMatchSetActionTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeUpdateByNameOrPositionTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUpdateByNameOrPositionTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeDeleteMatchClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDeleteMatchClauseTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeInsertMatchClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInsertMatchClauseTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeInsertMatchInfoTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInsertMatchInfoTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeInsertDefaultValuesTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInsertDefaultValuesTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeInsertByNameOrPositionTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInsertByNameOrPositionTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeInsertValuesListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInsertValuesListTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeDoNothingMatchClauseTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDoNothingMatchClauseTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeErrorMatchClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeErrorMatchClauseTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeUpdateMatchSetClauseTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUpdateMatchSetClauseTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeUpdateMatchSetInfoTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUpdateMatchSetInfoTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeAndExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAndExpressionTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeNotMatchedClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNotMatchedClauseTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeBySourceOrTargetTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBySourceOrTargetTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeBySourceTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBySourceTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeByTargetTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeByTargetTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializePivotOnTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePivotOnTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static void InitializePivotUsingTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePivotUsingTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializePivotColumnListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePivotColumnListTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializePivotColumnEntryTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePivotColumnEntryTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializePivotColumnExpressionTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePivotColumnExpressionTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializePivotColumnSubqueryTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePivotColumnSubqueryTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeIntoNameValuesTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIntoNameValuesTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeOptionalParensNameListTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOptionalParensNameListTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeParenthesizedNameListTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeParenthesizedNameListTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeBareNameListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBareNameListTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeIncludeOrExcludeNullsTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIncludeOrExcludeNullsTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeIncludeNullsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIncludeNullsTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeExcludeNullsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExcludeNullsTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeUnpivotHeaderTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUnpivotHeaderTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeUnpivotHeaderSingleTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUnpivotHeaderSingleTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeUnpivotHeaderListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUnpivotHeaderListTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializePragmaStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePragmaStatementTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializePragmaAssignOrFunctionTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePragmaAssignOrFunctionTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializePragmaAssignTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePragmaAssignTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializePragmaFunctionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePragmaFunctionTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializePragmaParametersTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePragmaParametersTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializePrepareStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePrepareStatementTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeTypeListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTypeListTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeSelectStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSelectStatementTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeSelectSetOpChainTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSelectSetOpChainTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeSelectSetOpChainTailTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSelectSetOpChainTailTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeIntersectChainTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIntersectChainTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeIntersectChainTailTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeIntersectChainTailTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeSetIntersectClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSetIntersectClauseTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeSelectAtomTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSelectAtomTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeSelectParensTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSelectParensTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeSetopClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSetopClauseTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeSetopTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSetopTypeTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeSetopUnionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSetopUnionTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeSetopExceptTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSetopExceptTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeSelectStatementTypeTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSelectStatementTypeTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeResultModifiersTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeResultModifiersTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeLimitOffsetTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLimitOffsetTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeLimitOffsetClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLimitOffsetClauseTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeOffsetLimitClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOffsetLimitClauseTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeOffsetFetchClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOffsetFetchClauseTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeFetchOnlyClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFetchOnlyClauseTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeTableStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTableStatementTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeOptionalParensSimpleSelectTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOptionalParensSimpleSelectTrampoline(PEGTransformer &transformer,
	                                                                         GeneratedTransformProcess &process);
	static void InitializeSimpleSelectParensTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSimpleSelectParensTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeSelectFromTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSelectFromTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeSelectFromClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSelectFromClauseTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeFromSelectClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFromSelectClauseTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeWithStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWithStatementTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeCTEBodyTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCTEBodyTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static void InitializeCTESelectBodyTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCTESelectBodyTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeCTEDMLBodyTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCTEDMLBodyTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeUsingKeyTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUsingKeyTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeMaterializedTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeMaterializedTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeSelectClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSelectClauseTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeTargetListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTargetListTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeColumnAliasesTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColumnAliasesTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeDistinctClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDistinctClauseTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeDistinctAllTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDistinctAllTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeDistinctOnTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDistinctOnTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeDistinctOnTargetsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDistinctOnTargetsTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeInnerTableRefTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInnerTableRefTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeTableSubqueryTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTableSubqueryTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeBaseTableRefTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBaseTableRefTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeTableAliasColonTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTableAliasColonTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeValuesRefTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeValuesRefTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeParensTableRefTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeParensTableRefTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeJoinOrPivotTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeJoinOrPivotTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeTablePivotClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTablePivotClauseTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeTablePivotClauseBodyTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTablePivotClauseBodyTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializePivotGroupByListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePivotGroupByListTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeTableUnpivotClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTableUnpivotClauseTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeTableUnpivotClauseBodyTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTableUnpivotClauseBodyTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializePivotHeaderTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePivotHeaderTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializePivotValueListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePivotValueListTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializePivotValueTargetTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePivotValueTargetTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializePivotEnumTargetTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePivotEnumTargetTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializePivotListTargetTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePivotListTargetTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeUnpivotValueListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUnpivotValueListTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializePivotTargetListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePivotTargetListTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeUnpivotTargetListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUnpivotTargetListTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeLateralTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLateralTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static void InitializeBaseTableNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBaseTableNameTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeUnqualifiedBaseTableNameTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUnqualifiedBaseTableNameTrampoline(PEGTransformer &transformer,
	                                                                       GeneratedTransformProcess &process);
	static void InitializeQualifiedTableNameTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeQualifiedTableNameTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeSchemaReservedTableTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSchemaReservedTableTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeCatalogReservedSchemaTableTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCatalogReservedSchemaTableTrampoline(PEGTransformer &transformer,
	                                                                         GeneratedTransformProcess &process);
	static void InitializeTableFunctionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTableFunctionTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeTableFunctionLateralOptTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTableFunctionLateralOptTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeTableFunctionAliasColonTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTableFunctionAliasColonTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeWithOrdinalityTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWithOrdinalityTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeQualifiedTableFunctionTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeQualifiedTableFunctionTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeTableFunctionArgumentsTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTableFunctionArgumentsTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeFunctionArgumentTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFunctionArgumentTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeNamedFunctionArgumentTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNamedFunctionArgumentTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializePositionalFunctionArgumentTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePositionalFunctionArgumentTrampoline(PEGTransformer &transformer,
	                                                                         GeneratedTransformProcess &process);
	static void InitializeNamedParameterTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNamedParameterTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeTableAliasTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTableAliasTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeTableAliasAsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTableAliasAsTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeTableAliasWithoutAsTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTableAliasWithoutAsTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeAtClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAtClauseTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeAtSpecifierTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAtSpecifierTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeAtUnitTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAtUnitTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static void InitializeVersionAtUnitTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeVersionAtUnitTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeTimestampAtUnitTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTimestampAtUnitTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeJoinClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeJoinClauseTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeNearestJoinClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNearestJoinClauseTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeNearestJoinAliasedTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNearestJoinAliasedTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeNearestJoinBareTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNearestJoinBareTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeNearestBareTableRefTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNearestBareTableRefTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeNearestValuesRefTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNearestValuesRefTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeNearestTableFunctionTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNearestTableFunctionTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeNearestTableSubqueryTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNearestTableSubqueryTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeNearestBaseTableRefTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNearestBaseTableRefTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeNearestParensTableRefTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNearestParensTableRefTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeApproxOrExactTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeApproxOrExactTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeNearestApproxTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNearestApproxTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeNearestExactTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNearestExactTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeDistanceOrSimilarityTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDistanceOrSimilarityTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeNearestDistanceTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNearestDistanceTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeNearestSimilarityTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNearestSimilarityTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeRegularJoinClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRegularJoinClauseTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeJoinByClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeJoinByClauseTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeAsofTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAsofTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static void InitializeJoinWithoutOnClauseTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeJoinWithoutOnClauseTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeJoinQualifierTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeJoinQualifierTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeOnClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOnClauseTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeUsingClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUsingClauseTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeJoinTypeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeJoinTypeTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeJoinPrefixTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeJoinPrefixTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeCrossJoinPrefixTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCrossJoinPrefixTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeNaturalJoinPrefixTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNaturalJoinPrefixTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializePositionalJoinPrefixTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizePositionalJoinPrefixTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeFullJoinTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFullJoinTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeLeftJoinTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLeftJoinTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeRightJoinTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRightJoinTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeSemiJoinTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSemiJoinTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeAntiJoinTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAntiJoinTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeInnerJoinTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeInnerJoinTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeFromClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFromClauseTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeWhereClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWhereClauseTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeGroupByClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGroupByClauseTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeHavingClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeHavingClauseTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeQualifyClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeQualifyClauseTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeSampleClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSampleClauseTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeWindowClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeWindowClauseTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeSampleEntryTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSampleEntryTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeSampleEntryCountTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSampleEntryCountTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeSampleEntryFunctionTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSampleEntryFunctionTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeSampleFunctionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSampleFunctionTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeSamplePropertiesTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSamplePropertiesTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeRepeatableSampleTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRepeatableSampleTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeSampleSeedTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSampleSeedTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeSampleCountTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSampleCountTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeSampleValueTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSampleValueTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeSampleUnitTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSampleUnitTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeSamplePercentageTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSamplePercentageTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeSampleRowsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSampleRowsTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeGroupByExpressionsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGroupByExpressionsTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeGroupByAllTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGroupByAllTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeGroupByListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGroupByListTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeGroupByExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGroupByExpressionTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeGroupByBaseExpressionTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGroupByBaseExpressionTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeEmptyGroupingItemTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeEmptyGroupingItemTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeCubeOrRollupClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCubeOrRollupClauseTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeCubeOrRollupTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCubeOrRollupTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeCubeKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCubeKeywordTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeRollupKeywordTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRollupKeywordTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeGroupingSetsClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGroupingSetsClauseTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeSubqueryReferenceTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSubqueryReferenceTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeOrderByExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOrderByExpressionTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeDescOrAscTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDescOrAscTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeDescendingOrderTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDescendingOrderTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeAscendingOrderTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAscendingOrderTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeNullsFirstOrLastTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNullsFirstOrLastTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeNullsFirstTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNullsFirstTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeNullsLastTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNullsLastTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeOrderByClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOrderByClauseTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeOrderByExpressionsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOrderByExpressionsTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeOrderByExpressionListTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOrderByExpressionListTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeOrderByAllTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOrderByAllTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeLimitClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLimitClauseTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeOffsetClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOffsetClauseTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeOffsetValueTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOffsetValueTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeLimitValueTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLimitValueTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeLimitAllTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLimitAllTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeLimitLiteralPercentTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLimitLiteralPercentTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeLimitExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLimitExpressionTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeFetchClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFetchClauseTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeFetchValueTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeFetchValueTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeAliasedExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeAliasedExpressionTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeColIdExpressionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeColIdExpressionTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeExpressionAsCollabelTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExpressionAsCollabelTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeExpressionOptIdentifierTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeExpressionOptIdentifierTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeValuesClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeValuesClauseTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeValuesExpressionsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeValuesExpressionsTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeSetStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSetStatementTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeSetAssignmentOrTimeZoneTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSetAssignmentOrTimeZoneTrampoline(PEGTransformer &transformer,
	                                                                      GeneratedTransformProcess &process);
	static void InitializeResetStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeResetStatementTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeSetSchemaTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSetSchemaTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeStandardAssignmentTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeStandardAssignmentTrampoline(PEGTransformer &transformer,
	                                                                 GeneratedTransformProcess &process);
	static void InitializeSetVariableOrSettingTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSetVariableOrSettingTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeSetTimeZoneTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSetTimeZoneTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeZoneValueTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeZoneValueTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeZoneLocalTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeZoneLocalTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeZoneDefaultTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeZoneDefaultTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeZoneStringLiteralTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeZoneStringLiteralTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeZoneIdentifierTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeZoneIdentifierTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeZoneIntervalWithIntervalTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeZoneIntervalWithIntervalTrampoline(PEGTransformer &transformer,
	                                                                       GeneratedTransformProcess &process);
	static void InitializeZoneIntervalWithPrecisionTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeZoneIntervalWithPrecisionTrampoline(PEGTransformer &transformer,
	                                                                        GeneratedTransformProcess &process);
	static void InitializeSetSettingTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSetSettingTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeSetVariableTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSetVariableTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeVariableScopeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeVariableScopeTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeSettingScopeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSettingScopeTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeLocalScopeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeLocalScopeTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeSessionScopeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSessionScopeTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeGlobalScopeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeGlobalScopeTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeSetAssignmentTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSetAssignmentTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeVariableListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeVariableListTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeTransactionStatementTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeTransactionStatementTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeBeginTransactionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBeginTransactionTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeRollbackTransactionTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeRollbackTransactionTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeCommitTransactionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCommitTransactionTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeReadOrWriteTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeReadOrWriteTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeReadOnlyOrReadWriteTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeReadOnlyOrReadWriteTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeReadOnlyTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeReadOnlyTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static void InitializeReadWriteTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeReadWriteTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeUpdateStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUpdateStatementTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeUpdateTargetTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUpdateTargetTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeBaseTableSetTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBaseTableSetTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeBaseTableAliasSetTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeBaseTableAliasSetTrampoline(PEGTransformer &transformer,
	                                                                GeneratedTransformProcess &process);
	static void InitializeUpdateAliasTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUpdateAliasTrampoline(PEGTransformer &transformer,
	                                                          GeneratedTransformProcess &process);
	static void InitializeUpdateSetClauseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUpdateSetClauseTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeUpdateSetTupleTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUpdateSetTupleTrampoline(PEGTransformer &transformer,
	                                                             GeneratedTransformProcess &process);
	static void InitializeUpdateSetElementListTrampoline(PEGTransformer &transformer,
	                                                     GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUpdateSetElementListTrampoline(PEGTransformer &transformer,
	                                                                   GeneratedTransformProcess &process);
	static void InitializeUpdateSetElementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUpdateSetElementTrampoline(PEGTransformer &transformer,
	                                                               GeneratedTransformProcess &process);
	static void InitializeUpdateSetColumnTargetTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUpdateSetColumnTargetTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeUseStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUseStatementTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeUseTargetTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUseTargetTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeSchemaNameAsUseTargetTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeSchemaNameAsUseTargetTrampoline(PEGTransformer &transformer,
	                                                                    GeneratedTransformProcess &process);
	static void InitializeCatalogNameAsUseTargetTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeCatalogNameAsUseTargetTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeUseTargetCatalogSchemaTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeUseTargetCatalogSchemaTrampoline(PEGTransformer &transformer,
	                                                                     GeneratedTransformProcess &process);
	static void InitializeDotIdentifierTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeDotIdentifierTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeVacuumStatementTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeVacuumStatementTrampoline(PEGTransformer &transformer,
	                                                              GeneratedTransformProcess &process);
	static void InitializeVacuumOptionsTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeVacuumOptionsTrampoline(PEGTransformer &transformer,
	                                                            GeneratedTransformProcess &process);
	static void InitializeVacuumParensOptionsTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeVacuumParensOptionsTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeVacuumLegacyOptionsTrampoline(PEGTransformer &transformer,
	                                                    GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeVacuumLegacyOptionsTrampoline(PEGTransformer &transformer,
	                                                                  GeneratedTransformProcess &process);
	static void InitializeVacuumOptionTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeVacuumOptionTrampoline(PEGTransformer &transformer,
	                                                           GeneratedTransformProcess &process);
	static void InitializeOptAnalyzeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOptAnalyzeTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeOptFullTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOptFullTrampoline(PEGTransformer &transformer,
	                                                      GeneratedTransformProcess &process);
	static void InitializeOptFreezeTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOptFreezeTrampoline(PEGTransformer &transformer,
	                                                        GeneratedTransformProcess &process);
	static void InitializeOptVerboseTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeOptVerboseTrampoline(PEGTransformer &transformer,
	                                                         GeneratedTransformProcess &process);
	static void InitializeNameListTrampoline(PEGTransformer &transformer, GeneratedTransformProcess &process);
	static transform_result_ptr FinalizeNameListTrampoline(PEGTransformer &transformer,
	                                                       GeneratedTransformProcess &process);
	static unique_ptr<SQLStatement> TransformAlterStatement(PEGTransformer &transformer,
	                                                        unique_ptr<AlterInfo> alter_options);
	static unique_ptr<AlterInfo> TransformAlterTableStmt(PEGTransformer &transformer, const optional<bool> &if_exists,
	                                                     unique_ptr<BaseTableRef> base_table_name,
	                                                     vector<unique_ptr<AlterTableInfo>> alter_table_options);
	static unique_ptr<AlterInfo> TransformAlterSchemaStmt(PEGTransformer &transformer, const optional<bool> &if_exists,
	                                                      const QualifiedName &qualified_name,
	                                                      unique_ptr<AlterTableInfo> rename_alter);
	static unique_ptr<AlterTableInfo> TransformAddConstraint(PEGTransformer &transformer,
	                                                         unique_ptr<Constraint> top_level_constraint);
	static unique_ptr<AlterTableInfo> TransformDropConstraint(PEGTransformer &transformer,
	                                                          const optional<bool> &if_exists,
	                                                          const Identifier &identifier,
	                                                          const optional<bool> &drop_behavior);
	static unique_ptr<AlterTableInfo> TransformAddColumn(PEGTransformer &transformer, const bool &has_result,
	                                                     const optional<bool> &if_not_exists,
	                                                     AddColumnEntry add_column_entry);
	static AddColumnEntry TransformAddColumnEntry(PEGTransformer &transformer, const vector<string> &dotted_identifier,
	                                              const optional<LogicalType> &type,
	                                              optional<GeneratedColumnDefinition> generated_column,
	                                              optional<vector<ColumnConstraintEntry>> column_constraint);
	static unique_ptr<AlterTableInfo> TransformDropColumn(PEGTransformer &transformer, const bool &has_result,
	                                                      const optional<bool> &if_exists,
	                                                      unique_ptr<ColumnRefExpression> nested_column_name,
	                                                      const optional<bool> &drop_behavior);
	static unique_ptr<AlterTableInfo> TransformAlterColumn(PEGTransformer &transformer, const bool &has_result,
	                                                       unique_ptr<ColumnRefExpression> nested_column_name,
	                                                       unique_ptr<AlterTableInfo> alter_column_entry);
	static unique_ptr<AlterTableInfo> TransformRenameColumn(PEGTransformer &transformer, const bool &has_result,
	                                                        unique_ptr<ColumnRefExpression> nested_column_name,
	                                                        const Identifier &identifier);
	static unique_ptr<ColumnRefExpression> TransformNestedColumnName(PEGTransformer &transformer,
	                                                                 const optional<vector<Identifier>> &identifier_dot,
	                                                                 const Identifier &column_name);
	static Identifier TransformIdentifierDot(PEGTransformer &transformer, const Identifier &identifier);
	static unique_ptr<AlterTableInfo> TransformRenameAlter(PEGTransformer &transformer, const Identifier &identifier);
	static unique_ptr<AlterTableInfo> TransformSetPartitionedBy(PEGTransformer &transformer,
	                                                            vector<unique_ptr<ParsedExpression>> expression);
	static unique_ptr<AlterTableInfo> TransformResetPartitionedBy(PEGTransformer &transformer);
	static unique_ptr<AlterTableInfo> TransformSetSortedBy(PEGTransformer &transformer,
	                                                       vector<OrderByNode> order_by_expressions);
	static unique_ptr<AlterTableInfo> TransformResetSortedBy(PEGTransformer &transformer);
	static unique_ptr<AlterTableInfo>
	TransformSetOptions(PEGTransformer &transformer,
	                    case_insensitive_map_t<unique_ptr<ParsedExpression>> rel_option_list);
	static unique_ptr<AlterTableInfo>
	TransformResetOptions(PEGTransformer &transformer,
	                      case_insensitive_map_t<unique_ptr<ParsedExpression>> rel_option_list);
	static unique_ptr<AlterTableInfo> TransformAddDefault(PEGTransformer &transformer,
	                                                      unique_ptr<ParsedExpression> expression);
	static unique_ptr<AlterTableInfo> TransformDropDefault(PEGTransformer &transformer);
	static unique_ptr<AlterTableInfo> TransformChangeNullability(PEGTransformer &transformer,
	                                                             const string &drop_or_set);
	static string TransformDropNullability(PEGTransformer &transformer);
	static string TransformSetNullability(PEGTransformer &transformer);
	static unique_ptr<AlterTableInfo> TransformAlterType(PEGTransformer &transformer, const bool &has_result,
	                                                     const optional<LogicalType> &type,
	                                                     optional<unique_ptr<ParsedExpression>> using_expression);
	static unique_ptr<ParsedExpression> TransformUsingExpression(PEGTransformer &transformer,
	                                                             unique_ptr<ParsedExpression> expression);
	static unique_ptr<AlterInfo> TransformAlterViewStmt(PEGTransformer &transformer, const optional<bool> &if_exists,
	                                                    unique_ptr<BaseTableRef> base_table_name,
	                                                    unique_ptr<AlterTableInfo> rename_alter);
	static unique_ptr<AlterInfo> TransformAlterSequenceStmt(PEGTransformer &transformer,
	                                                        const optional<bool> &if_exists,
	                                                        const QualifiedName &qualified_sequence_name,
	                                                        unique_ptr<AlterInfo> alter_sequence_options);
	static QualifiedName TransformQualifiedSequenceName(PEGTransformer &transformer,
	                                                    const optional<Identifier> &catalog_qualification,
	                                                    const optional<Identifier> &schema_qualification,
	                                                    const Identifier &sequence_name);
	static unique_ptr<AlterInfo> TransformRenameAlterSequenceOptions(PEGTransformer &transformer,
	                                                                 unique_ptr<AlterTableInfo> rename_alter);
	static unique_ptr<AlterInfo>
	TransformSetSequenceOption(PEGTransformer &transformer,
	                           vector<pair<string, unique_ptr<SequenceOption>>> sequence_option);
	static unique_ptr<AlterInfo> TransformAlterDatabaseStmt(PEGTransformer &transformer,
	                                                        const optional<bool> &if_exists,
	                                                        const Identifier &identifier,
	                                                        const Identifier &identifier_1);
	static unique_ptr<SQLStatement> TransformAnalyzeStatement(PEGTransformer &transformer,
	                                                          const Identifier &analyze_keyword,
	                                                          const optional<bool> &analyze_verbose,
	                                                          optional<AnalyzeTarget> analyze_target);
	static AnalyzeTarget TransformAnalyzeTarget(PEGTransformer &transformer, unique_ptr<BaseTableRef> base_table_name,
	                                            const optional<vector<string>> &name_list);
	static bool TransformAnalyzeVerbose(PEGTransformer &transformer);
	static unique_ptr<SQLStatement>
	TransformAttachStatement(PEGTransformer &transformer, const optional<bool> &or_replace,
	                         const optional<bool> &if_not_exists, const bool &has_result,
	                         unique_ptr<ParsedExpression> database_path, const optional<Identifier> &attach_alias,
	                         const optional<vector<GenericCopyOption>> &attach_options);
	static unique_ptr<ParsedExpression> TransformDatabasePath(PEGTransformer &transformer,
	                                                          unique_ptr<ParsedExpression> expression);
	static Identifier TransformAttachAlias(PEGTransformer &transformer, const Identifier &col_id);
	static vector<GenericCopyOption> TransformAttachOptions(PEGTransformer &transformer,
	                                                        const vector<GenericCopyOption> &generic_copy_option_list);
	static unique_ptr<SQLStatement> TransformCallStatement(PEGTransformer &transformer,
	                                                       const QualifiedName &qualified_table_function,
	                                                       vector<FunctionArgument> table_function_arguments);
	static unique_ptr<SQLStatement> TransformCheckpointStatement(PEGTransformer &transformer,
	                                                             const optional<bool> &checkpoint_force,
	                                                             const optional<Identifier> &catalog_name);
	static bool TransformCheckpointForce(PEGTransformer &transformer);
	static unique_ptr<SQLStatement> TransformCommentStatement(PEGTransformer &transformer,
	                                                          const CatalogType &comment_on_type,
	                                                          const vector<string> &dotted_identifier,
	                                                          const Value &comment_value);
	static CatalogType TransformCommentTable(PEGTransformer &transformer);
	static CatalogType TransformCommentSequence(PEGTransformer &transformer);
	static CatalogType TransformCommentFunction(PEGTransformer &transformer);
	static CatalogType TransformCommentMacroTable(PEGTransformer &transformer);
	static CatalogType TransformCommentMacro(PEGTransformer &transformer);
	static CatalogType TransformCommentView(PEGTransformer &transformer);
	static CatalogType TransformCommentDatabase(PEGTransformer &transformer);
	static CatalogType TransformCommentIndex(PEGTransformer &transformer);
	static CatalogType TransformCommentSchema(PEGTransformer &transformer);
	static CatalogType TransformCommentType(PEGTransformer &transformer);
	static CatalogType TransformCommentColumn(PEGTransformer &transformer);
	static Value TransformStringLiteralValue(PEGTransformer &transformer, const string &string_literal);
	static Identifier TransformAnalyzeKeyword(PEGTransformer &transformer);
	static unique_ptr<SQLStatement> TransformExpressionStatement(PEGTransformer &transformer,
	                                                             vector<unique_ptr<ParsedExpression>> expression_alias);
	static Identifier TransformConstraintName(PEGTransformer &transformer, const Identifier &col_id_or_string);
	static Identifier TransformCollationName(PEGTransformer &transformer, const Identifier &identifier);
	static unique_ptr<ParsedExpression> TransformNumberLiteral(PEGTransformer &transformer, ParseResult &parse_result);
	static string TransformStringLiteral(PEGTransformer &transformer, ParseResult &parse_result);
	static LogicalType TransformType(PEGTransformer &transformer, unique_ptr<ParsedExpression> type_variations,
	                                 const optional<vector<int64_t>> &array_bounds);
	static unique_ptr<ParsedExpression>
	TransformCharacterSimpleType(PEGTransformer &transformer,
	                             optional<vector<unique_ptr<ParsedExpression>>> type_modifiers);
	static unique_ptr<ParsedExpression>
	TransformQualifiedSimpleType(PEGTransformer &transformer, const QualifiedName &qualified_type_name,
	                             optional<vector<unique_ptr<ParsedExpression>>> type_modifiers);
	static unique_ptr<ParsedExpression>
	TransformIntervalWithRangeSpecifier(PEGTransformer &transformer,
	                                    const DatePartSpecifier &interval_to_interval_as_type);
	static unique_ptr<ParsedExpression> TransformIntervalWithSimpleSpecifier(PEGTransformer &transformer,
	                                                                         const DatePartSpecifier &interval);
	static unique_ptr<ParsedExpression> TransformIntervalWithoutSpecifier(PEGTransformer &transformer);
	static DatePartSpecifier TransformIntervalToIntervalAsType(PEGTransformer &transformer, ParseResult &parse_result);
	static DatePartSpecifier TransformYearKeyword(PEGTransformer &transformer);
	static DatePartSpecifier TransformMonthKeyword(PEGTransformer &transformer);
	static DatePartSpecifier TransformDayKeyword(PEGTransformer &transformer);
	static DatePartSpecifier TransformHourKeyword(PEGTransformer &transformer);
	static DatePartSpecifier TransformMinuteKeyword(PEGTransformer &transformer);
	static DatePartSpecifier TransformSecondKeyword(PEGTransformer &transformer);
	static DatePartSpecifier TransformMillisecondKeyword(PEGTransformer &transformer);
	static DatePartSpecifier TransformMicrosecondKeyword(PEGTransformer &transformer);
	static DatePartSpecifier TransformWeekKeyword(PEGTransformer &transformer);
	static DatePartSpecifier TransformQuarterKeyword(PEGTransformer &transformer);
	static DatePartSpecifier TransformDecadeKeyword(PEGTransformer &transformer);
	static DatePartSpecifier TransformCenturyKeyword(PEGTransformer &transformer);
	static DatePartSpecifier TransformMillenniumKeyword(PEGTransformer &transformer);
	static DatePartSpecifier TransformYearToMonth(PEGTransformer &transformer, const DatePartSpecifier &year_keyword,
	                                              const DatePartSpecifier &month_keyword);
	static DatePartSpecifier TransformDayToHour(PEGTransformer &transformer, const DatePartSpecifier &day_keyword,
	                                            const DatePartSpecifier &hour_keyword);
	static DatePartSpecifier TransformDayToMinute(PEGTransformer &transformer, const DatePartSpecifier &day_keyword,
	                                              const DatePartSpecifier &minute_keyword);
	static DatePartSpecifier TransformDayToSecond(PEGTransformer &transformer, const DatePartSpecifier &day_keyword,
	                                              const DatePartSpecifier &second_keyword);
	static DatePartSpecifier TransformHourToMinute(PEGTransformer &transformer, const DatePartSpecifier &hour_keyword,
	                                               const DatePartSpecifier &minute_keyword);
	static DatePartSpecifier TransformHourToSecond(PEGTransformer &transformer, const DatePartSpecifier &hour_keyword,
	                                               const DatePartSpecifier &second_keyword);
	static DatePartSpecifier TransformMinuteToSecond(PEGTransformer &transformer,
	                                                 const DatePartSpecifier &minute_keyword,
	                                                 const DatePartSpecifier &second_keyword);
	static unique_ptr<ParsedExpression> TransformBitType(PEGTransformer &transformer, const bool &has_result,
	                                                     optional<vector<unique_ptr<ParsedExpression>>> expression);
	static unique_ptr<ParsedExpression> TransformGeometryType(PEGTransformer &transformer,
	                                                          optional<unique_ptr<ParsedExpression>> expression);
	static unique_ptr<ParsedExpression> TransformVariantType(PEGTransformer &transformer);
	static unique_ptr<ParsedExpression> TransformSimpleNumericType(PEGTransformer &transformer, const string &child);
	static string TransformIntType(PEGTransformer &transformer);
	static string TransformIntegerType(PEGTransformer &transformer);
	static string TransformSmallintType(PEGTransformer &transformer);
	static string TransformBigintType(PEGTransformer &transformer);
	static string TransformRealType(PEGTransformer &transformer);
	static string TransformBooleanType(PEGTransformer &transformer);
	static string TransformDoubleType(PEGTransformer &transformer);
	static unique_ptr<ParsedExpression> TransformFloatType(PEGTransformer &transformer,
	                                                       optional<unique_ptr<ParsedExpression>> number_literal);
	static unique_ptr<ParsedExpression>
	TransformDecimalType(PEGTransformer &transformer, optional<vector<unique_ptr<ParsedExpression>>> type_modifiers);
	static unique_ptr<ParsedExpression> TransformDecType(PEGTransformer &transformer,
	                                                     optional<vector<unique_ptr<ParsedExpression>>> type_modifiers);
	static unique_ptr<ParsedExpression>
	TransformNumericModType(PEGTransformer &transformer, optional<vector<unique_ptr<ParsedExpression>>> type_modifiers);
	static QualifiedName TransformTypeNameAsQualifiedName(PEGTransformer &transformer, const Identifier &type_name);
	static QualifiedName TransformCatalogReservedSchemaTypeName(PEGTransformer &transformer,
	                                                            const Identifier &catalog_qualification,
	                                                            const vector<Identifier> &reserved_schema_qualification,
	                                                            const Identifier &reserved_type_name);
	static QualifiedName TransformSchemaReservedTypeName(PEGTransformer &transformer,
	                                                     const Identifier &schema_qualification,
	                                                     const Identifier &reserved_type_name);
	static vector<unique_ptr<ParsedExpression>>
	TransformTypeModifiers(PEGTransformer &transformer, optional<vector<unique_ptr<ParsedExpression>>> expression);
	static unique_ptr<ParsedExpression> TransformRowType(PEGTransformer &transformer,
	                                                     const optional<child_list_t<LogicalType>> &col_id_type_list);
	static unique_ptr<ParsedExpression> TransformSetofType(PEGTransformer &transformer, const LogicalType &type);
	static unique_ptr<ParsedExpression> TransformUnionType(PEGTransformer &transformer,
	                                                       const child_list_t<LogicalType> &col_id_type_list);
	static child_list_t<LogicalType> TransformColIdTypeList(PEGTransformer &transformer,
	                                                        const vector<pair<Identifier, LogicalType>> &col_id_type);
	static unique_ptr<ParsedExpression> TransformMapType(PEGTransformer &transformer,
	                                                     const optional<vector<LogicalType>> &type);
	static unique_ptr<ParsedExpression> TransformTupleType(PEGTransformer &transformer,
	                                                       const vector<LogicalType> &type);
	static pair<Identifier, LogicalType> TransformColIdType(PEGTransformer &transformer, const Identifier &col_id,
	                                                        const LogicalType &type);
	static int64_t TransformArrayKeyword(PEGTransformer &transformer);
	static int64_t TransformArrayKeywordWithBounds(PEGTransformer &transformer, const int64_t &square_brackets_array);
	static int64_t TransformSquareBracketsArray(PEGTransformer &transformer,
	                                            optional<unique_ptr<ParsedExpression>> expression);
	static unique_ptr<ParsedExpression> TransformTimeType(PEGTransformer &transformer,
	                                                      const LogicalTypeId &time_or_timestamp,
	                                                      optional<vector<unique_ptr<ParsedExpression>>> type_modifiers,
	                                                      const optional<bool> &time_zone);
	static LogicalTypeId TransformTimeTypeId(PEGTransformer &transformer);
	static LogicalTypeId TransformTimestampTypeId(PEGTransformer &transformer);
	static bool TransformTimeZone(PEGTransformer &transformer, const bool &with_or_without);
	static bool TransformWithRule(PEGTransformer &transformer);
	static bool TransformWithoutRule(PEGTransformer &transformer);
	static unique_ptr<SQLStatement> TransformConnectStatement(PEGTransformer &transformer,
	                                                          optional<unique_ptr<ConnectInfo>> session_target);
	static unique_ptr<SQLStatement> TransformDisconnectStatement(PEGTransformer &transformer);
	static unique_ptr<ConnectInfo> TransformLocalSessionTarget(PEGTransformer &transformer);
	static unique_ptr<ConnectInfo>
	TransformStringSessionTarget(PEGTransformer &transformer, const string &string_literal,
	                             const optional<vector<GenericCopyOption>> &generic_copy_option_list);
	static unique_ptr<ConnectInfo> TransformCatalogSessionTarget(PEGTransformer &transformer,
	                                                             const Identifier &catalog_name);
	static unique_ptr<SQLStatement> TransformCopyStatement(PEGTransformer &transformer,
	                                                       unique_ptr<SQLStatement> copy_variations);
	static unique_ptr<SQLStatement> TransformCopyTable(PEGTransformer &transformer,
	                                                   unique_ptr<BaseTableRef> base_table_name,
	                                                   const optional<vector<string>> &insert_column_list,
	                                                   const bool &from_or_to,
	                                                   unique_ptr<ParsedExpression> copy_file_name,
	                                                   const optional<vector<GenericCopyOption>> &copy_options);
	static bool TransformCopyFrom(PEGTransformer &transformer);
	static bool TransformCopyTo(PEGTransformer &transformer);
	static unique_ptr<SQLStatement> TransformCopySelect(PEGTransformer &transformer,
	                                                    unique_ptr<SelectStatement> select_statement_internal,
	                                                    unique_ptr<ParsedExpression> copy_file_name,
	                                                    const optional<vector<GenericCopyOption>> &copy_options);
	static unique_ptr<ParsedExpression> TransformCopyFileNameStringLiteral(PEGTransformer &transformer,
	                                                                       const string &string_literal);
	static unique_ptr<ParsedExpression> TransformCopyFileNameIdentifier(PEGTransformer &transformer,
	                                                                    const Identifier &identifier);
	static unique_ptr<ParsedExpression> TransformCopyFileNameIdentifierColId(PEGTransformer &transformer,
	                                                                         const Identifier &identifier_col_id);
	static Identifier TransformIdentifierColId(PEGTransformer &transformer, const Identifier &identifier,
	                                           const vector<Identifier> &copy_file_name_suffix);
	static vector<GenericCopyOption> TransformCopyOptions(PEGTransformer &transformer, const bool &has_result,
	                                                      const vector<GenericCopyOption> &copy_option_list);
	static vector<GenericCopyOption>
	TransformSpecializedOptionList(PEGTransformer &transformer, const GenericCopyOption &specialized_option,
	                               const optional<vector<GenericCopyOption>> &specialized_option_tail);
	static GenericCopyOption TransformSpecializedOptionTail(PEGTransformer &transformer, const bool &has_result,
	                                                        const GenericCopyOption &specialized_option);
	static GenericCopyOption TransformBinaryOption(PEGTransformer &transformer);
	static GenericCopyOption TransformFreezeOption(PEGTransformer &transformer);
	static GenericCopyOption TransformOidsOption(PEGTransformer &transformer);
	static GenericCopyOption TransformCsvOption(PEGTransformer &transformer);
	static GenericCopyOption TransformHeaderOption(PEGTransformer &transformer);
	static GenericCopyOption TransformNullAsOption(PEGTransformer &transformer, const bool &has_result,
	                                               const string &string_literal);
	static GenericCopyOption TransformDelimiterAsOption(PEGTransformer &transformer, const bool &has_result,
	                                                    const string &string_literal);
	static GenericCopyOption TransformQuoteAsOption(PEGTransformer &transformer, const bool &has_result,
	                                                const string &string_literal);
	static GenericCopyOption TransformEscapeAsOption(PEGTransformer &transformer, const bool &has_result,
	                                                 const string &string_literal);
	static GenericCopyOption TransformEncodingOption(PEGTransformer &transformer, const string &string_literal);
	static GenericCopyOption TransformForceQuoteOption(PEGTransformer &transformer, const optional<bool> &force_quote,
	                                                   const vector<string> &star_symbol_column_list);
	static bool TransformForceQuote(PEGTransformer &transformer);
	static GenericCopyOption TransformPartitionByOption(PEGTransformer &transformer,
	                                                    const vector<string> &partition_by_column_list);
	static vector<string> TransformStarPartitionByColumnList(PEGTransformer &transformer);
	static vector<string> TransformParenthesizedPartitionByColumnList(PEGTransformer &transformer,
	                                                                  const vector<string> &column_list);
	static vector<string> TransformSinglePartitionByColumnList(PEGTransformer &transformer, const Identifier &col_id);
	static GenericCopyOption TransformForceNullOption(PEGTransformer &transformer, const optional<bool> &force_not_null,
	                                                  const vector<string> &column_list);
	static bool TransformForceNotNull(PEGTransformer &transformer);
	static vector<GenericCopyOption>
	TransformCopyGenericOptionList(PEGTransformer &transformer, const vector<GenericCopyOption> &copy_generic_option);
	static GenericCopyOption TransformOrderByCopyOption(PEGTransformer &transformer,
	                                                    optional<GenericCopyOptionValue> generic_copy_option_value);
	static GenericCopyOption
	TransformPartitionedByCopyOption(PEGTransformer &transformer,
	                                 optional<GenericCopyOptionValue> generic_copy_option_value);
	static vector<GenericCopyOption>
	TransformGenericCopyOptionList(PEGTransformer &transformer, const vector<GenericCopyOption> &generic_copy_option);
	static GenericCopyOption TransformGenericCopyOption(PEGTransformer &transformer, const Identifier &copy_option_name,
	                                                    optional<GenericCopyOptionValue> generic_copy_option_value);
	static GenericCopyOptionValue
	TransformGenericCopyOptionOrderList(PEGTransformer &transformer,
	                                    vector<OrderByNode> generic_copy_option_parenthesized_expression_list);
	static GenericCopyOptionValue TransformGenericCopyOptionExpression(PEGTransformer &transformer,
	                                                                   unique_ptr<ParsedExpression> expression);
	static vector<OrderByNode>
	TransformGenericCopyOptionParenthesizedExpressionList(PEGTransformer &transformer,
	                                                      vector<OrderByNode> order_by_expression_list);
	static unique_ptr<SQLStatement> TransformCopyFromDatabaseWithFlag(PEGTransformer &transformer,
	                                                                  const Identifier &col_id,
	                                                                  const Identifier &col_id_1,
	                                                                  const CopyDatabaseType &copy_database_flag);
	static unique_ptr<SQLStatement> TransformCopyFromDatabaseWithoutFlag(PEGTransformer &transformer,
	                                                                     const Identifier &col_id,
	                                                                     const Identifier &col_id_1);
	static CopyDatabaseType TransformCopyDatabaseFlag(PEGTransformer &transformer,
	                                                  const CopyDatabaseType &schema_or_data);
	static CopyDatabaseType TransformCopySchema(PEGTransformer &transformer);
	static CopyDatabaseType TransformCopyData(PEGTransformer &transformer);
	static unique_ptr<CreateStatement>
	TransformCreateIndexStmt(PEGTransformer &transformer, const optional<bool> &unique_index,
	                         const optional<bool> &if_not_exists, const optional<Identifier> &index_name,
	                         unique_ptr<BaseTableRef> base_table_name,
	                         const optional<vector<string>> &insert_column_list, const optional<Identifier> &index_type,
	                         optional<vector<unique_ptr<ParsedExpression>>> index_element,
	                         optional<case_insensitive_map_t<unique_ptr<ParsedExpression>>> with_list,
	                         optional<unique_ptr<ParsedExpression>> where_clause);
	static case_insensitive_map_t<unique_ptr<ParsedExpression>>
	TransformWithList(PEGTransformer &transformer,
	                  case_insensitive_map_t<unique_ptr<ParsedExpression>> rel_option_or_oids);
	static case_insensitive_map_t<unique_ptr<ParsedExpression>>
	TransformRelOptionList(PEGTransformer &transformer,
	                       vector<pair<Identifier, unique_ptr<ParsedExpression>>> rel_option);
	static case_insensitive_map_t<unique_ptr<ParsedExpression>> TransformOids(PEGTransformer &transformer,
	                                                                          const bool &with_or_without_oids);
	static bool TransformWithOids(PEGTransformer &transformer);
	static bool TransformWithoutOids(PEGTransformer &transformer);
	static unique_ptr<ParsedExpression> TransformIndexElement(PEGTransformer &transformer,
	                                                          unique_ptr<ParsedExpression> expression,
	                                                          const optional<OrderType> &desc_or_asc,
	                                                          const optional<OrderByNullType> &nulls_first_or_last);
	static bool TransformUniqueIndex(PEGTransformer &transformer);
	static Identifier TransformIndexType(PEGTransformer &transformer, const Identifier &identifier);
	static pair<Identifier, unique_ptr<ParsedExpression>>
	TransformRelOption(PEGTransformer &transformer, const Identifier &rel_option_name,
	                   optional<unique_ptr<ParsedExpression>> rel_option_argument_opt);
	static Identifier TransformRelOptionName(PEGTransformer &transformer, const string &child);
	static string TransformDottedIdentifierString(PEGTransformer &transformer, const vector<string> &dotted_identifier);
	static unique_ptr<ParsedExpression> TransformRelOptionArgumentOpt(PEGTransformer &transformer,
	                                                                  unique_ptr<ParsedExpression> def_arg);
	static unique_ptr<ParsedExpression> TransformDefArgNull(PEGTransformer &transformer, const Value &null_literal);
	static unique_ptr<ParsedExpression> TransformDefArgKeyword(PEGTransformer &transformer,
	                                                           const string &reserved_keyword);
	static unique_ptr<ParsedExpression> TransformDefArgStringLiteral(PEGTransformer &transformer,
	                                                                 const string &string_literal);
	static unique_ptr<ParsedExpression> TransformNoneLiteral(PEGTransformer &transformer);
	static unique_ptr<CreateStatement> TransformCreateMacroStmt(PEGTransformer &transformer,
	                                                            const bool &macro_or_function,
	                                                            const optional<bool> &if_not_exists,
	                                                            const QualifiedName &qualified_name,
	                                                            vector<unique_ptr<MacroFunction>> macro_definition);
	static bool TransformMacroKeyword(PEGTransformer &transformer);
	static bool TransformFunctionKeyword(PEGTransformer &transformer);
	static unique_ptr<MacroFunction> TransformMacroDefinition(PEGTransformer &transformer,
	                                                          optional<vector<MacroParameter>> macro_parameters,
	                                                          unique_ptr<MacroFunction> macro_definition_body);
	static vector<MacroParameter> TransformMacroParameters(PEGTransformer &transformer,
	                                                       vector<MacroParameter> macro_parameter);
	static MacroParameter TransformSimpleParameter(PEGTransformer &transformer, const Identifier &type_func_name,
	                                               const optional<LogicalType> &type);
	static unique_ptr<MacroFunction> TransformScalarMacroDefinition(PEGTransformer &transformer,
	                                                                unique_ptr<ParsedExpression> expression);
	static unique_ptr<MacroFunction>
	TransformTableMacroDefinition(PEGTransformer &transformer, unique_ptr<SelectStatement> select_statement_internal);
	static unique_ptr<CreateStatement> TransformCreateSchemaStmt(PEGTransformer &transformer,
	                                                             const optional<bool> &if_not_exists,
	                                                             const QualifiedName &qualified_name);
	static unique_ptr<CreateStatement>
	TransformCreateSecretStmt(PEGTransformer &transformer, const optional<bool> &if_not_exists,
	                          const optional<Identifier> &secret_name,
	                          const optional<Identifier> &secret_storage_specifier,
	                          const vector<GenericCopyOption> &generic_copy_option_list);
	static Identifier TransformSecretStorageSpecifier(PEGTransformer &transformer, const Identifier &identifier);
	static Identifier TransformSecretName(PEGTransformer &transformer, const Identifier &col_id);
	static unique_ptr<CreateStatement>
	TransformCreateSequenceStmt(PEGTransformer &transformer, const optional<bool> &if_not_exists,
	                            const QualifiedName &qualified_name,
	                            optional<vector<pair<string, unique_ptr<SequenceOption>>>> sequence_option);
	static pair<string, unique_ptr<SequenceOption>> TransformSeqCycle(PEGTransformer &transformer);
	static pair<string, unique_ptr<SequenceOption>> TransformSeqNoCycle(PEGTransformer &transformer);
	static pair<string, unique_ptr<SequenceOption>> TransformSeqSetIncrement(PEGTransformer &transformer,
	                                                                         const bool &has_result,
	                                                                         unique_ptr<ParsedExpression> expression);
	static pair<string, unique_ptr<SequenceOption>> TransformSeqSetMinMax(PEGTransformer &transformer,
	                                                                      const string &seq_min_or_max,
	                                                                      unique_ptr<ParsedExpression> expression);
	static pair<string, unique_ptr<SequenceOption>> TransformSeqNoMinMax(PEGTransformer &transformer,
	                                                                     const string &seq_min_or_max);
	static pair<string, unique_ptr<SequenceOption>>
	TransformSeqStartWith(PEGTransformer &transformer, const bool &has_result, unique_ptr<ParsedExpression> expression);
	static pair<string, unique_ptr<SequenceOption>> TransformSeqOwnedBy(PEGTransformer &transformer,
	                                                                    const QualifiedName &qualified_name);
	static string TransformMinValue(PEGTransformer &transformer);
	static string TransformMaxValue(PEGTransformer &transformer);
	static unique_ptr<SQLStatement> TransformCreateStatement(PEGTransformer &transformer,
	                                                         const optional<bool> &or_replace,
	                                                         const optional<SecretPersistType> &temporary,
	                                                         unique_ptr<CreateStatement> create_statement_variation);
	static bool TransformOrReplace(PEGTransformer &transformer);
	static SecretPersistType TransformPersistent(PEGTransformer &transformer);
	static SecretPersistType TransformTempPersistent(PEGTransformer &transformer);
	static SecretPersistType TransformTemporaryPersistent(PEGTransformer &transformer);
	static unique_ptr<CreateStatement> TransformCreateTableStmt(PEGTransformer &transformer,
	                                                            const optional<bool> &if_not_exists,
	                                                            const QualifiedName &qualified_name,
	                                                            CreateTableDefinition create_table_definition,
	                                                            const optional<bool> &commit_action);
	static CreateTableDefinition
	TransformCreateTableAs(PEGTransformer &transformer, optional<ColumnList> identifier_list,
	                       optional<PartitionSortedOptions> partition_sorted_options,
	                       optional<case_insensitive_map_t<unique_ptr<ParsedExpression>>> with_list,
	                       unique_ptr<SQLStatement> statement, const optional<bool> &with_data);
	static PartitionSortedOptions
	TransformPartitionOptSortedOptions(PEGTransformer &transformer,
	                                   vector<unique_ptr<ParsedExpression>> partition_options,
	                                   optional<vector<unique_ptr<ParsedExpression>>> sorted_options);
	static PartitionSortedOptions
	TransformSortedOptPartitionOptions(PEGTransformer &transformer, vector<unique_ptr<ParsedExpression>> sorted_options,
	                                   optional<vector<unique_ptr<ParsedExpression>>> partition_options);
	static vector<unique_ptr<ParsedExpression>>
	TransformPartitionOptions(PEGTransformer &transformer, vector<unique_ptr<ParsedExpression>> expression);
	static vector<unique_ptr<ParsedExpression>> TransformSortedOptions(PEGTransformer &transformer,
	                                                                   vector<unique_ptr<ParsedExpression>> expression);
	static bool TransformWithDataOnly(PEGTransformer &transformer);
	static bool TransformWithNoData(PEGTransformer &transformer);
	static ColumnList TransformIdentifierList(PEGTransformer &transformer, const vector<Identifier> &identifier);
	static CreateTableDefinition
	TransformCreateColumnList(PEGTransformer &transformer, optional<ColumnElements> create_table_column_list,
	                          optional<PartitionSortedOptions> partition_sorted_options,
	                          optional<case_insensitive_map_t<unique_ptr<ParsedExpression>>> with_list);
	static bool TransformIfNotExists(PEGTransformer &transformer);
	static QualifiedName
	TransformSchemaReservedIdentifierOrStringLiteral(PEGTransformer &transformer,
	                                                 const Identifier &schema_qualification,
	                                                 const Identifier &reserved_identifier_or_string_literal);
	static QualifiedName
	TransformCatalogReservedSchemaIdentifier(PEGTransformer &transformer, const Identifier &catalog_qualification,
	                                         const vector<Identifier> &reserved_schema_qualification,
	                                         const Identifier &reserved_identifier_or_string_literal);
	static QualifiedName TransformIdentifierOrStringLiteral(PEGTransformer &transformer, const string &child);
	static Identifier TransformCatalogQualification(PEGTransformer &transformer, const Identifier &catalog_name);
	static Identifier TransformSchemaQualification(PEGTransformer &transformer, const Identifier &schema_name);
	static Identifier TransformReservedSchemaQualification(PEGTransformer &transformer,
	                                                       const Identifier &reserved_schema_name);
	static Identifier TransformTableQualification(PEGTransformer &transformer, const Identifier &table_name);
	static Identifier TransformReservedTableQualification(PEGTransformer &transformer,
	                                                      const Identifier &reserved_table_name);
	static ColumnElements TransformCreateTableColumnList(PEGTransformer &transformer,
	                                                     vector<CreateTableColumnElement> create_table_column_element);
	static CreateTableColumnElement TransformCreateTableColumnDefinition(PEGTransformer &transformer,
	                                                                     ConstraintColumnDefinition column_definition);
	static CreateTableColumnElement TransformCreateTableConstraint(PEGTransformer &transformer,
	                                                               unique_ptr<Constraint> top_level_constraint);
	static ConstraintColumnDefinition
	TransformColumnDefinition(PEGTransformer &transformer, const vector<string> &dotted_identifier,
	                          const optional<LogicalType> &type, optional<GeneratedColumnDefinition> generated_column,
	                          const bool &has_result, optional<vector<ColumnConstraintEntry>> column_constraint);
	static ColumnConstraintEntry TransformNotNullConstraint(PEGTransformer &transformer, const bool &child);
	static bool TransformNullConstraint(PEGTransformer &transformer);
	static bool TransformNotNullColumnConstraint(PEGTransformer &transformer);
	static ColumnConstraintEntry TransformUniqueConstraint(PEGTransformer &transformer);
	static ColumnConstraintEntry TransformPrimaryKeyConstraint(PEGTransformer &transformer);
	static ColumnConstraintEntry TransformDefaultValue(PEGTransformer &transformer,
	                                                   unique_ptr<ParsedExpression> column_default_expr);
	static ColumnConstraintEntry TransformCheckConstraint(PEGTransformer &transformer,
	                                                      unique_ptr<ParsedExpression> expression);
	static ColumnConstraintEntry TransformForeignKeyConstraint(PEGTransformer &transformer,
	                                                           unique_ptr<BaseTableRef> base_table_name,
	                                                           const optional<vector<string>> &column_list,
	                                                           const KeyActions &key_actions);
	static ColumnConstraintEntry TransformColumnCollation(PEGTransformer &transformer,
	                                                      const vector<string> &dotted_identifier);
	static ColumnConstraintEntry TransformColumnCompression(PEGTransformer &transformer,
	                                                        const Identifier &col_id_or_string);
	static KeyActions TransformKeyActions(PEGTransformer &transformer, const optional<string> &update_action,
	                                      const optional<string> &delete_action);
	static string TransformUpdateAction(PEGTransformer &transformer, const string &key_action);
	static string TransformDeleteAction(PEGTransformer &transformer, const string &key_action);
	static string TransformNoKeyAction(PEGTransformer &transformer);
	static string TransformRestrictKeyAction(PEGTransformer &transformer);
	static string TransformCascadeKeyAction(PEGTransformer &transformer);
	static string TransformSetNullKeyAction(PEGTransformer &transformer);
	static string TransformSetDefaultKeyAction(PEGTransformer &transformer);
	static unique_ptr<Constraint> TransformTopLevelConstraint(PEGTransformer &transformer, const bool &has_result,
	                                                          unique_ptr<Constraint> top_level_constraint_list);
	static unique_ptr<Constraint> TransformTopCheckConstraint(PEGTransformer &transformer,
	                                                          ColumnConstraintEntry check_constraint);
	static unique_ptr<Constraint> TransformTopPrimaryKeyConstraint(PEGTransformer &transformer,
	                                                               const vector<string> &column_id_list);
	static unique_ptr<Constraint> TransformTopUniqueConstraint(PEGTransformer &transformer,
	                                                           const vector<string> &column_id_list);
	static unique_ptr<Constraint> TransformTopForeignKeyConstraint(PEGTransformer &transformer,
	                                                               const vector<string> &column_id_list,
	                                                               ColumnConstraintEntry foreign_key_constraint);
	static vector<string> TransformColumnIdList(PEGTransformer &transformer, const vector<Identifier> &col_id);
	static vector<string> TransformDottedIdentifier(PEGTransformer &transformer, const Identifier &identifier,
	                                                const optional<vector<string>> &dot_col_label);
	static string TransformDotColLabel(PEGTransformer &transformer, const string &col_label);
	static Identifier TransformColLabelIdentifier(PEGTransformer &transformer, const string &col_label);
	static Identifier TransformStringLiteralIdentifier(PEGTransformer &transformer, const string &string_literal);
	static GeneratedColumnDefinition TransformGeneratedColumn(PEGTransformer &transformer, const bool &has_result,
	                                                          unique_ptr<ParsedExpression> expression,
	                                                          const optional<bool> &generated_column_type);
	static bool TransformCommitAction(PEGTransformer &transformer, const bool &preserve_or_delete);
	static bool TransformPreserveRows(PEGTransformer &transformer);
	static bool TransformDeleteRows(PEGTransformer &transformer);
	static bool TransformVirtualGeneratedColumn(PEGTransformer &transformer);
	static bool TransformStoredGeneratedColumn(PEGTransformer &transformer);
	static unique_ptr<CreateStatement>
	TransformCreateTriggerStmt(PEGTransformer &transformer, const optional<bool> &if_not_exists,
	                           const Identifier &trigger_name, const TriggerTiming &trigger_timing,
	                           const TriggerEventInfo &trigger_event, unique_ptr<BaseTableRef> base_table_name,
	                           const optional<TriggerTableReferencingInfo> &referencing_clause,
	                           const optional<TriggerForEach> &for_each_clause, unique_ptr<SQLStatement> trigger_body);
	static Identifier TransformTriggerName(PEGTransformer &transformer, const Identifier &identifier);
	static TriggerTableReferencingInfo
	TransformReferencingClause(PEGTransformer &transformer, const TriggerTableReferencingInfo &referencing_item,
	                           const optional<TriggerTableReferencingInfo> &referencing_item_1);
	static TriggerTableReferencingInfo TransformReferencingNewTableAs(PEGTransformer &transformer,
	                                                                  const Identifier &col_id);
	static TriggerTableReferencingInfo TransformReferencingOldTableAs(PEGTransformer &transformer,
	                                                                  const Identifier &col_id);
	static TriggerTiming TransformTriggerBefore(PEGTransformer &transformer);
	static TriggerTiming TransformTriggerAfter(PEGTransformer &transformer);
	static TriggerTiming TransformTriggerInsteadOf(PEGTransformer &transformer);
	static TriggerEventInfo TransformTriggerEventInsert(PEGTransformer &transformer);
	static TriggerEventInfo TransformTriggerEventDelete(PEGTransformer &transformer);
	static TriggerEventInfo TransformTriggerEventUpdate(PEGTransformer &transformer);
	static TriggerEventInfo TransformTriggerEventUpdateOf(PEGTransformer &transformer,
	                                                      const vector<string> &trigger_column_list);
	static vector<string> TransformTriggerColumnList(PEGTransformer &transformer, const vector<Identifier> &col_id);
	static TriggerForEach TransformForEachRow(PEGTransformer &transformer);
	static TriggerForEach TransformForEachStatement(PEGTransformer &transformer);
	static unique_ptr<CreateStatement> TransformCreateTypeStmt(PEGTransformer &transformer,
	                                                           const optional<bool> &if_not_exists,
	                                                           const QualifiedName &qualified_name,
	                                                           unique_ptr<CreateTypeInfo> create_type);
	static unique_ptr<CreateTypeInfo> TransformCreateTypeFromType(PEGTransformer &transformer, const LogicalType &type);
	static unique_ptr<CreateTypeInfo> TransformEnumSelectType(PEGTransformer &transformer,
	                                                          unique_ptr<SelectStatement> select_statement_internal);
	static unique_ptr<CreateTypeInfo> TransformEnumStringLiteralList(PEGTransformer &transformer,
	                                                                 const optional<vector<string>> &string_literal);
	static unique_ptr<CreateStatement>
	TransformCreateViewStmt(PEGTransformer &transformer, const optional<bool> &create_secure,
	                        const optional<bool> &create_recursive, const optional<bool> &if_not_exists,
	                        const QualifiedName &qualified_name, const optional<vector<string>> &insert_column_list,
	                        optional<case_insensitive_map_t<unique_ptr<ParsedExpression>>> with_list,
	                        unique_ptr<SelectStatement> select_statement_internal);
	static bool TransformCreateRecursive(PEGTransformer &transformer);
	static bool TransformCreateSecure(PEGTransformer &transformer);
	static unique_ptr<SQLStatement> TransformDeallocateStatement(PEGTransformer &transformer,
	                                                             const optional<bool> &deallocate_prepare,
	                                                             const Identifier &identifier);
	static bool TransformDeallocatePrepare(PEGTransformer &transformer);
	static unique_ptr<SQLStatement>
	TransformDeleteStatement(PEGTransformer &transformer, optional<CommonTableExpressionMap> with_clause,
	                         unique_ptr<BaseTableRef> target_opt_alias,
	                         optional<vector<unique_ptr<TableRef>>> delete_using_clause,
	                         optional<unique_ptr<ParsedExpression>> where_clause,
	                         optional<vector<unique_ptr<ParsedExpression>>> returning_clause);
	static unique_ptr<SQLStatement> TransformTruncateStatement(PEGTransformer &transformer, const bool &has_result,
	                                                           unique_ptr<BaseTableRef> base_table_name);
	static unique_ptr<BaseTableRef> TransformTargetOptAlias(PEGTransformer &transformer,
	                                                        unique_ptr<BaseTableRef> base_table_name,
	                                                        const bool &has_result, const optional<Identifier> &col_id);
	static vector<unique_ptr<TableRef>> TransformDeleteUsingClause(PEGTransformer &transformer,
	                                                               vector<unique_ptr<TableRef>> table_ref);
	static unique_ptr<SelectStatement> TransformDescribeStatement(PEGTransformer &transformer,
	                                                              unique_ptr<QueryNode> child);
	static unique_ptr<QueryNode> TransformShowDeprecatedSelect(PEGTransformer &transformer, const ShowType &show_rule,
	                                                           unique_ptr<SelectStatement> select_statement_internal);
	static unique_ptr<QueryNode> TransformDescribeSelect(PEGTransformer &transformer,
	                                                     const ShowType &describe_or_summarize,
	                                                     unique_ptr<SelectStatement> select_statement_internal);
	static unique_ptr<QueryNode> TransformShowAllTables(PEGTransformer &transformer, const ShowType &show_or_describe,
	                                                    const bool &has_result);
	static unique_ptr<QueryNode> TransformShowTables(PEGTransformer &transformer, const ShowType &show_or_describe,
	                                                 const QualifiedName &qualified_name);
	static unique_ptr<QueryNode> TransformShowByName(PEGTransformer &transformer, const ShowType &show_rule,
	                                                 optional<DescribeTarget> show_target);
	static unique_ptr<QueryNode> TransformDescribeByName(PEGTransformer &transformer,
	                                                     const ShowType &describe_or_summarize,
	                                                     optional<DescribeTarget> describe_target);
	static DescribeTarget TransformShowDeprecatedQualifiedTableName(PEGTransformer &transformer,
	                                                                unique_ptr<BaseTableRef> qualified_table_name);
	static DescribeTarget TransformShowSettingName(PEGTransformer &transformer, const Identifier &setting_name);
	static DescribeTarget TransformDescribeBaseTableName(PEGTransformer &transformer,
	                                                     unique_ptr<BaseTableRef> base_table_name);
	static DescribeTarget TransformDescribeStringLiteral(PEGTransformer &transformer, const string &string_literal);
	static ShowType TransformSummarize(PEGTransformer &transformer, const ShowType &summarize_rule);
	static ShowType TransformSummarizeRule(PEGTransformer &transformer);
	static ShowType TransformShowRule(PEGTransformer &transformer);
	static ShowType TransformDescribeLongRule(PEGTransformer &transformer);
	static ShowType TransformDescRule(PEGTransformer &transformer);
	static unique_ptr<SQLStatement> TransformDetachStatement(PEGTransformer &transformer, const bool &has_result,
	                                                         const optional<bool> &if_exists,
	                                                         const Identifier &catalog_name);
	static unique_ptr<SQLStatement> TransformDropStatement(PEGTransformer &transformer,
	                                                       unique_ptr<DropStatement> drop_entries,
	                                                       const optional<bool> &drop_behavior);
	static unique_ptr<DropStatement> TransformDropTrigger(PEGTransformer &transformer, const optional<bool> &if_exists,
	                                                      const Identifier &trigger_name,
	                                                      unique_ptr<BaseTableRef> base_table_name);
	static unique_ptr<DropStatement> TransformDropTable(PEGTransformer &transformer, const CatalogType &table_or_view,
	                                                    const optional<bool> &if_exists,
	                                                    vector<unique_ptr<BaseTableRef>> base_table_name);
	static unique_ptr<DropStatement> TransformDropTableFunction(PEGTransformer &transformer,
	                                                            const CatalogType &comment_macro_table,
	                                                            const optional<bool> &if_exists,
	                                                            const vector<Identifier> &table_function_name);
	static unique_ptr<DropStatement> TransformDropFunction(PEGTransformer &transformer, const bool &function_type_macro,
	                                                       const optional<bool> &if_exists,
	                                                       const vector<QualifiedName> &function_identifier);
	static unique_ptr<DropStatement> TransformDropSchema(PEGTransformer &transformer, const optional<bool> &if_exists,
	                                                     const vector<QualifiedName> &qualified_name);
	static unique_ptr<DropStatement> TransformDropIndex(PEGTransformer &transformer, const optional<bool> &if_exists,
	                                                    const vector<QualifiedName> &qualified_index_name);
	static QualifiedName TransformQualifiedIndexNameString(PEGTransformer &transformer, const Identifier &index_name);
	static QualifiedName TransformSchemaReservedIndex(PEGTransformer &transformer,
	                                                  const Identifier &schema_qualification,
	                                                  const Identifier &reserved_index_name);
	static QualifiedName TransformCatalogReservedSchemaIndex(PEGTransformer &transformer,
	                                                         const Identifier &catalog_qualification,
	                                                         const Identifier &reserved_schema_qualification,
	                                                         const Identifier &reserved_index_name);
	static unique_ptr<DropStatement> TransformDropSequence(PEGTransformer &transformer, const optional<bool> &if_exists,
	                                                       const vector<QualifiedName> &qualified_sequence_name);
	static unique_ptr<DropStatement> TransformDropCollation(PEGTransformer &transformer,
	                                                        const optional<bool> &if_exists,
	                                                        const vector<Identifier> &collation_name);
	static unique_ptr<DropStatement> TransformDropType(PEGTransformer &transformer, const optional<bool> &if_exists,
	                                                   const vector<QualifiedName> &qualified_type_name);
	static unique_ptr<DropStatement> TransformDropSecret(PEGTransformer &transformer,
	                                                     const optional<SecretPersistType> &temporary,
	                                                     const optional<bool> &if_exists, const Identifier &secret_name,
	                                                     const optional<Identifier> &drop_secret_storage);
	static CatalogType TransformMaterializedViewEntry(PEGTransformer &transformer);
	static bool TransformFunctionTypeMacroKeyword(PEGTransformer &transformer);
	static bool TransformFunctionTypeFunction(PEGTransformer &transformer);
	static bool TransformCascadeDropBehavior(PEGTransformer &transformer);
	static bool TransformRestrictDropBehavior(PEGTransformer &transformer);
	static bool TransformIfExists(PEGTransformer &transformer);
	static Identifier TransformDropSecretStorage(PEGTransformer &transformer, const Identifier &identifier);
	static unique_ptr<SQLStatement>
	TransformExecuteStatement(PEGTransformer &transformer, const Identifier &identifier,
	                          optional<vector<FunctionArgument>> table_function_arguments);
	static unique_ptr<SQLStatement>
	TransformExplainStatement(PEGTransformer &transformer, const optional<Identifier> &analyze_keyword,
	                          const optional<vector<GenericCopyOption>> &explain_option_list,
	                          unique_ptr<SQLStatement> explainable_statements);
	static vector<GenericCopyOption> TransformExplainOptionList(PEGTransformer &transformer,
	                                                            const vector<GenericCopyOption> &explain_option);
	static GenericCopyOption TransformExplainOption(PEGTransformer &transformer, const Identifier &explain_option_name,
	                                                optional<unique_ptr<ParsedExpression>> expression);
	static Identifier TransformExplainOptionName(PEGTransformer &transformer, ParseResult &choice_result);
	static unique_ptr<SQLStatement>
	TransformExplainSelectStatement(PEGTransformer &transformer, unique_ptr<SelectStatement> select_statement_internal);
	static unique_ptr<SQLStatement>
	TransformExportStatement(PEGTransformer &transformer, const optional<string> &export_source,
	                         const string &string_literal,
	                         const optional<vector<GenericCopyOption>> &generic_copy_option_list);
	static string TransformExportSource(PEGTransformer &transformer, const Identifier &catalog_name);
	static unique_ptr<SQLStatement> TransformImportStatement(PEGTransformer &transformer, const string &string_literal);
	static unique_ptr<ParsedExpression> TransformColumnReference(PEGTransformer &transformer,
	                                                             unique_ptr<ColumnRefExpression> child);
	static unique_ptr<ColumnRefExpression> TransformNestedSchemaTableColumnName(
	    PEGTransformer &transformer, const Identifier &catalog_qualification,
	    const Identifier &reserved_schema_qualification, const Identifier &reserved_schema_qualification_1,
	    const vector<Identifier> &reserved_schema_qualification_2, const Identifier &reserved_column_name);
	static unique_ptr<ColumnRefExpression>
	TransformCatalogReservedSchemaTableColumnName(PEGTransformer &transformer, const Identifier &catalog_qualification,
	                                              const Identifier &reserved_schema_qualification,
	                                              const Identifier &reserved_table_qualification,
	                                              const Identifier &reserved_column_name);
	static unique_ptr<ColumnRefExpression>
	TransformSchemaReservedTableColumnName(PEGTransformer &transformer, const Identifier &schema_qualification,
	                                       const Identifier &reserved_table_qualification,
	                                       const Identifier &reserved_column_name);
	static unique_ptr<ColumnRefExpression> TransformTableReservedColumnName(PEGTransformer &transformer,
	                                                                        const Identifier &table_qualification,
	                                                                        const Identifier &reserved_column_name);
	static unique_ptr<ParsedExpression>
	TransformFunctionExpression(PEGTransformer &transformer, const QualifiedName &function_identifier,
	                            MethodArguments function_expression_arguments,
	                            optional<vector<OrderByNode>> within_group_clause,
	                            optional<unique_ptr<ParsedExpression>> filter_clause, const bool &has_result,
	                            optional<unique_ptr<WindowExpression>> over_clause);
	static MethodArguments TransformFunctionExpressionArguments(PEGTransformer &transformer,
	                                                            MethodArguments function_expression_argument_list);
	static MethodArguments
	TransformFunctionExpressionArgumentList(PEGTransformer &transformer, const optional<bool> &distinct_or_all,
	                                        optional<vector<FunctionArgument>> function_argument_list,
	                                        optional<vector<OrderByNode>> order_by_clause,
	                                        const optional<bool> &ignore_or_respect_nulls);
	static vector<FunctionArgument> TransformFunctionArgumentList(PEGTransformer &transformer,
	                                                              vector<FunctionArgument> function_argument);
	static QualifiedName TransformFunctionNameAsQualifiedName(PEGTransformer &transformer,
	                                                          const Identifier &function_name);
	static QualifiedName
	TransformCatalogReservedSchemaFunctionName(PEGTransformer &transformer, const Identifier &catalog_qualification,
	                                           const optional<vector<Identifier>> &reserved_schema_qualification,
	                                           const Identifier &reserved_function_name);
	static QualifiedName TransformSchemaReservedFunctionName(PEGTransformer &transformer,
	                                                         const Identifier &schema_qualification,
	                                                         const Identifier &reserved_function_name);
	static bool TransformDistinctKeyword(PEGTransformer &transformer);
	static bool TransformAllKeyword(PEGTransformer &transformer);
	static vector<OrderByNode> TransformWithinGroupClause(PEGTransformer &transformer,
	                                                      vector<OrderByNode> order_by_clause);
	static unique_ptr<ParsedExpression> TransformFilterClause(PEGTransformer &transformer,
	                                                          unique_ptr<ParsedExpression> filter_clause_expression);
	static unique_ptr<ParsedExpression>
	TransformFilterClauseExpression(PEGTransformer &transformer, unique_ptr<ParsedExpression> filter_clause_contents);
	static unique_ptr<ParsedExpression> TransformFilterClauseContents(PEGTransformer &transformer,
	                                                                  const bool &has_result,
	                                                                  unique_ptr<ParsedExpression> expression);
	static bool TransformIgnoreNulls(PEGTransformer &transformer);
	static bool TransformRespectNulls(PEGTransformer &transformer);
	static unique_ptr<ParsedExpression>
	TransformParenthesisExpression(PEGTransformer &transformer,
	                               optional<vector<unique_ptr<ParsedExpression>>> expression);
	static unique_ptr<ParsedExpression> TransformLiteralExpression(PEGTransformer &transformer,
	                                                               ParseResult &choice_result);
	static unique_ptr<ParsedExpression> TransformConstantLiteral(PEGTransformer &transformer, const Value &child);
	static Value TransformNullLiteral(PEGTransformer &transformer);
	static Value TransformTrueLiteral(PEGTransformer &transformer);
	static Value TransformFalseLiteral(PEGTransformer &transformer);
	static unique_ptr<ParsedExpression>
	TransformCastExpression(PEGTransformer &transformer, const bool &cast_or_try_cast, CastArguments cast_arguments);
	static CastArguments TransformCastArguments(PEGTransformer &transformer, unique_ptr<ParsedExpression> expression,
	                                            const LogicalType &type);
	static bool TransformCastKeyword(PEGTransformer &transformer);
	static bool TransformTryCastKeyword(PEGTransformer &transformer);
	static string TransformColIdDot(PEGTransformer &transformer, const Identifier &col_id);
	static unique_ptr<ParsedExpression>
	TransformStarExpression(PEGTransformer &transformer, const optional<vector<string>> &star_qualifier_list,
	                        const optional<qualified_column_set_t> &exclude_list,
	                        optional<case_insensitive_map_t<unique_ptr<ParsedExpression>>> replace_list,
	                        const optional<qualified_column_map_t<string>> &rename_list);
	static vector<string> TransformStarQualifierList(PEGTransformer &transformer, const vector<string> &col_id_dot);
	static qualified_column_set_t TransformExcludeList(PEGTransformer &transformer,
	                                                   const qualified_column_set_t &exclude_names);
	static qualified_column_set_t TransformExcludeNameList(PEGTransformer &transformer,
	                                                       const vector<QualifiedColumnName> &exclude_name);
	static qualified_column_set_t TransformExcludeNameSingle(PEGTransformer &transformer,
	                                                         const QualifiedColumnName &exclude_name);
	static QualifiedColumnName TransformExcludeDottedName(PEGTransformer &transformer,
	                                                      const vector<string> &dotted_identifier);
	static QualifiedColumnName TransformExcludeColumnName(PEGTransformer &transformer,
	                                                      const Identifier &col_id_or_string);
	static case_insensitive_map_t<unique_ptr<ParsedExpression>>
	TransformReplaceList(PEGTransformer &transformer,
	                     case_insensitive_map_t<unique_ptr<ParsedExpression>> replace_entries);
	static case_insensitive_map_t<unique_ptr<ParsedExpression>>
	TransformReplaceEntrySingle(PEGTransformer &transformer, pair<string, unique_ptr<ParsedExpression>> replace_entry);
	static case_insensitive_map_t<unique_ptr<ParsedExpression>>
	TransformReplaceEntryList(PEGTransformer &transformer,
	                          vector<pair<string, unique_ptr<ParsedExpression>>> replace_entry);
	static pair<string, unique_ptr<ParsedExpression>>
	TransformReplaceEntry(PEGTransformer &transformer, unique_ptr<ParsedExpression> expression,
	                      unique_ptr<ParsedExpression> column_reference);
	static qualified_column_map_t<string> TransformRenameList(PEGTransformer &transformer,
	                                                          const qualified_column_map_t<string> &rename_entries);
	static qualified_column_map_t<string>
	TransformRenameEntryList(PEGTransformer &transformer,
	                         const vector<pair<QualifiedColumnName, string>> &rename_entry);
	static qualified_column_map_t<string>
	TransformSingleRenameEntry(PEGTransformer &transformer, const pair<QualifiedColumnName, string> &rename_entry);
	static pair<QualifiedColumnName, string> TransformRenameEntry(PEGTransformer &transformer,
	                                                              const QualifiedColumnName &exclude_name,
	                                                              const Identifier &identifier);
	static unique_ptr<ParsedExpression> TransformSubqueryExpression(PEGTransformer &transformer,
	                                                                const optional<bool> &subquery_not,
	                                                                const optional<bool> &subquery_exists,
	                                                                unique_ptr<TableRef> subquery_reference);
	static bool TransformSubqueryNot(PEGTransformer &transformer);
	static bool TransformSubqueryExists(PEGTransformer &transformer);
	static unique_ptr<ParsedExpression> TransformCaseExpression(PEGTransformer &transformer,
	                                                            optional<unique_ptr<ParsedExpression>> expression,
	                                                            vector<CaseCheck> case_when_then,
	                                                            optional<unique_ptr<ParsedExpression>> case_else);
	static CaseCheck TransformCaseWhenThen(PEGTransformer &transformer, unique_ptr<ParsedExpression> expression,
	                                       unique_ptr<ParsedExpression> expression_1);
	static unique_ptr<ParsedExpression> TransformCaseElse(PEGTransformer &transformer,
	                                                      unique_ptr<ParsedExpression> expression);
	static unique_ptr<ParsedExpression> TransformTypeLiteral(PEGTransformer &transformer, const LogicalType &type,
	                                                         const string &string_literal);
	static unique_ptr<ParsedExpression> TransformIntervalLiteral(PEGTransformer &transformer,
	                                                             unique_ptr<ParsedExpression> interval_parameter,
	                                                             const optional<DatePartSpecifier> &interval);
	static unique_ptr<ParsedExpression> TransformIntervalStringParameter(PEGTransformer &transformer,
	                                                                     const string &string_literal);
	static WindowFrame TransformFrameClause(PEGTransformer &transformer, const string &framing,
	                                        vector<WindowBoundaryExpression> frame_extent,
	                                        const optional<WindowExcludeMode> &window_exclude_clause);
	static string TransformRowsFraming(PEGTransformer &transformer);
	static string TransformRangeFraming(PEGTransformer &transformer);
	static string TransformGroupsFraming(PEGTransformer &transformer);
	static vector<WindowBoundaryExpression> TransformSingleFrameExtent(PEGTransformer &transformer,
	                                                                   WindowBoundaryExpression frame_bound);
	static vector<WindowBoundaryExpression> TransformBetweenFrameExtent(PEGTransformer &transformer,
	                                                                    WindowBoundaryExpression frame_bound,
	                                                                    WindowBoundaryExpression frame_bound_1);
	static WindowBoundaryExpression TransformFrameUnbounded(PEGTransformer &transformer,
	                                                        const bool &preceding_or_following);
	static WindowBoundaryExpression TransformFrameExpression(PEGTransformer &transformer,
	                                                         unique_ptr<ParsedExpression> expression,
	                                                         const bool &preceding_or_following);
	static WindowBoundaryExpression TransformFrameCurrentRow(PEGTransformer &transformer);
	static bool TransformPrecedingFrame(PEGTransformer &transformer);
	static bool TransformFollowingFrame(PEGTransformer &transformer);
	static WindowExcludeMode TransformWindowExcludeClause(PEGTransformer &transformer,
	                                                      const WindowExcludeMode &window_exclude_element);
	static WindowExcludeMode TransformExcludeCurrentRow(PEGTransformer &transformer);
	static WindowExcludeMode TransformExcludeGroup(PEGTransformer &transformer);
	static WindowExcludeMode TransformExcludeTies(PEGTransformer &transformer);
	static WindowExcludeMode TransformExcludeNoOthers(PEGTransformer &transformer);
	static unique_ptr<WindowExpression> TransformIdentifierWindowFrame(PEGTransformer &transformer,
	                                                                   const Identifier &identifier);
	static unique_ptr<WindowExpression> TransformParensIdentifier(PEGTransformer &transformer,
	                                                              const Identifier &identifier);
	static unique_ptr<WindowExpression>
	TransformWindowFrameNameContentsParens(PEGTransformer &transformer,
	                                       unique_ptr<WindowExpression> window_frame_name_contents);
	static unique_ptr<WindowExpression>
	TransformWindowFrameNameContents(PEGTransformer &transformer, const optional<Identifier> &base_window_name,
	                                 unique_ptr<WindowExpression> window_frame_contents);
	static unique_ptr<WindowExpression>
	TransformWindowFrameContentsParens(PEGTransformer &transformer, unique_ptr<WindowExpression> window_frame_contents);
	static unique_ptr<WindowExpression>
	TransformWindowFrameContents(PEGTransformer &transformer,
	                             optional<vector<unique_ptr<ParsedExpression>>> window_partition,
	                             optional<vector<OrderByNode>> order_by_clause, optional<WindowFrame> frame_clause);
	static Identifier TransformBaseWindowName(PEGTransformer &transformer, const Identifier &identifier);
	static vector<unique_ptr<ParsedExpression>>
	TransformWindowPartition(PEGTransformer &transformer, vector<unique_ptr<ParsedExpression>> expression);
	static unique_ptr<ParsedExpression>
	TransformArrayBoundedListExpression(PEGTransformer &transformer, const bool &has_result,
	                                    vector<unique_ptr<ParsedExpression>> bounded_list_expression);
	static unique_ptr<ParsedExpression>
	TransformArrayParensSelect(PEGTransformer &transformer, unique_ptr<SelectStatement> select_statement_internal);
	static vector<unique_ptr<ParsedExpression>>
	TransformBoundedListExpression(PEGTransformer &transformer,
	                               optional<vector<unique_ptr<ParsedExpression>>> expression);
	static unique_ptr<ParsedExpression> TransformStructExpression(PEGTransformer &transformer,
	                                                              optional<vector<FunctionArgument>> struct_field);
	static FunctionArgument TransformStructField(PEGTransformer &transformer, const Identifier &col_id_or_string,
	                                             unique_ptr<ParsedExpression> expression);
	static unique_ptr<ParsedExpression>
	TransformMapExpression(PEGTransformer &transformer, vector<unique_ptr<ParsedExpression>> map_struct_expression);
	static vector<unique_ptr<ParsedExpression>>
	TransformMapStructExpression(PEGTransformer &transformer,
	                             optional<vector<vector<unique_ptr<ParsedExpression>>>> map_struct_field);
	static vector<unique_ptr<ParsedExpression>> TransformMapStructField(PEGTransformer &transformer,
	                                                                    unique_ptr<ParsedExpression> expression,
	                                                                    unique_ptr<ParsedExpression> expression_1);
	static unique_ptr<ParsedExpression>
	TransformGroupingExpression(PEGTransformer &transformer, const bool &grouping_or_grouping_id,
	                            optional<vector<unique_ptr<ParsedExpression>>> expression);
	static bool TransformGroupingKeyword(PEGTransformer &transformer);
	static bool TransformGroupingIdKeyword(PEGTransformer &transformer);
	static unique_ptr<ParsedExpression>
	TransformQuestionMarkNumberedParameter(PEGTransformer &transformer, unique_ptr<ParsedExpression> number_literal);
	static unique_ptr<ParsedExpression> TransformAnonymousParameter(PEGTransformer &transformer);
	static unique_ptr<ParsedExpression> TransformNumberedParameter(PEGTransformer &transformer,
	                                                               unique_ptr<ParsedExpression> number_literal);
	static unique_ptr<ParsedExpression> TransformColLabelParameter(PEGTransformer &transformer,
	                                                               const string &col_label);
	static unique_ptr<ParsedExpression> TransformPositionalExpression(PEGTransformer &transformer,
	                                                                  unique_ptr<ParsedExpression> number_literal);
	static unique_ptr<ParsedExpression> TransformDefaultExpression(PEGTransformer &transformer);
	static unique_ptr<ParsedExpression>
	TransformListComprehensionExpression(PEGTransformer &transformer, unique_ptr<ParsedExpression> expression,
	                                     const vector<Identifier> &col_id_or_string,
	                                     unique_ptr<ParsedExpression> expression_1,
	                                     optional<unique_ptr<ParsedExpression>> list_comprehension_filter);
	static unique_ptr<ParsedExpression> TransformListComprehensionFilter(PEGTransformer &transformer,
	                                                                     unique_ptr<ParsedExpression> expression);
	static unique_ptr<ParsedExpression> TransformParensExpression(PEGTransformer &transformer,
	                                                              unique_ptr<ParsedExpression> expression);
	static unique_ptr<ParsedExpression> TransformColumnDefaultExpr(PEGTransformer &transformer,
	                                                               unique_ptr<ParsedExpression> col_def_or_expr);
	static unique_ptr<ParsedExpression>
	TransformLambdaArrowExpression(PEGTransformer &transformer, unique_ptr<ParsedExpression> logical_or_expression,
	                               optional<vector<unique_ptr<ParsedExpression>>> single_arrow_pair);
	static unique_ptr<ParsedExpression> TransformSingleArrowPair(PEGTransformer &transformer,
	                                                             unique_ptr<ParsedExpression> logical_or_expression);
	static unique_ptr<ParsedExpression>
	TransformLogicalOrExpression(PEGTransformer &transformer, unique_ptr<ParsedExpression> logical_and_expression,
	                             optional<vector<unique_ptr<ParsedExpression>>> logical_or_expression_tail);
	static unique_ptr<ParsedExpression>
	TransformLogicalOrExpressionTail(PEGTransformer &transformer, unique_ptr<ParsedExpression> logical_and_expression);
	static unique_ptr<ParsedExpression>
	TransformColDefOrExpr(PEGTransformer &transformer, unique_ptr<ParsedExpression> col_def_and_expr,
	                      optional<vector<unique_ptr<ParsedExpression>>> col_def_or_expression_tail);
	static unique_ptr<ParsedExpression> TransformColDefOrExpressionTail(PEGTransformer &transformer,
	                                                                    unique_ptr<ParsedExpression> col_def_and_expr);
	static unique_ptr<ParsedExpression>
	TransformLogicalAndExpression(PEGTransformer &transformer, unique_ptr<ParsedExpression> logical_not_expression,
	                              optional<vector<unique_ptr<ParsedExpression>>> logical_and_expression_tail);
	static unique_ptr<ParsedExpression>
	TransformLogicalAndExpressionTail(PEGTransformer &transformer, unique_ptr<ParsedExpression> logical_not_expression);
	static unique_ptr<ParsedExpression>
	TransformColDefAndExpr(PEGTransformer &transformer, unique_ptr<ParsedExpression> is_distinct_from_expression,
	                       optional<vector<unique_ptr<ParsedExpression>>> col_def_and_expression_tail);
	static unique_ptr<ParsedExpression>
	TransformColDefAndExpressionTail(PEGTransformer &transformer,
	                                 unique_ptr<ParsedExpression> is_distinct_from_expression);
	static unique_ptr<ParsedExpression> TransformLogicalNotExpression(PEGTransformer &transformer,
	                                                                  optional<vector<bool>> not_expression,
	                                                                  unique_ptr<ParsedExpression> is_expression);
	static vector<bool> TransformNotExpression(PEGTransformer &transformer, const vector<bool> &not_keyword);
	static bool TransformNotKeyword(PEGTransformer &transformer);
	static unique_ptr<ParsedExpression> TransformIsExpression(PEGTransformer &transformer,
	                                                          unique_ptr<ParsedExpression> is_distinct_from_expression,
	                                                          optional<vector<unique_ptr<ParsedExpression>>> is_test);
	static unique_ptr<ParsedExpression> TransformIsLiteral(PEGTransformer &transformer, const bool &has_result,
	                                                       const Value &is_literal_value);
	static Value TransformUnknownLiteral(PEGTransformer &transformer);
	static unique_ptr<ParsedExpression> TransformNotNullKeyword(PEGTransformer &transformer);
	static unique_ptr<ParsedExpression> TransformNotNullOperator(PEGTransformer &transformer);
	static unique_ptr<ParsedExpression> TransformIsNull(PEGTransformer &transformer,
	                                                    unique_ptr<ParsedExpression> is_null_operator);
	static unique_ptr<ParsedExpression> TransformIsNullOperator(PEGTransformer &transformer);
	static unique_ptr<ParsedExpression>
	TransformIsDistinctFromExpression(PEGTransformer &transformer, unique_ptr<ParsedExpression> comparison_expression,
	                                  optional<vector<IsDistinctFromTail>> is_distinct_from_tail);
	static IsDistinctFromTail TransformIsDistinctFromTail(PEGTransformer &transformer,
	                                                      const ExpressionType &is_distinct_from_op,
	                                                      unique_ptr<ParsedExpression> comparison_expression);
	static ExpressionType TransformIsDistinctFromOp(PEGTransformer &transformer, const bool &has_result);
	static unique_ptr<ParsedExpression>
	TransformComparisonExpression(PEGTransformer &transformer, unique_ptr<ParsedExpression> between_in_like_expression,
	                              optional<vector<ComparisonExpressionTail>> comparison_expression_tail);
	static ComparisonExpressionTail
	TransformComparisonExpressionTail(PEGTransformer &transformer, const ExpressionType &comparison_operator,
	                                  optional<vector<bool>> not_expression,
	                                  unique_ptr<ParsedExpression> between_in_like_expression);
	static ExpressionType TransformOperatorEqual(PEGTransformer &transformer);
	static ExpressionType TransformOperatorNotEqual(PEGTransformer &transformer);
	static ExpressionType TransformOperatorLessThan(PEGTransformer &transformer);
	static ExpressionType TransformOperatorGreaterThan(PEGTransformer &transformer);
	static ExpressionType TransformOperatorLessThanEquals(PEGTransformer &transformer);
	static ExpressionType TransformOperatorGreaterThanEquals(PEGTransformer &transformer);
	static unique_ptr<ParsedExpression>
	TransformBetweenInLikeExpression(PEGTransformer &transformer,
	                                 unique_ptr<ParsedExpression> other_operator_expression,
	                                 optional<BetweenInLikeOperator> between_in_like_op);
	static BetweenInLikeOperator TransformBetweenInLikeOp(PEGTransformer &transformer, const bool &has_result,
	                                                      unique_ptr<ParsedExpression> between_in_like_op_expression);
	static unique_ptr<ParsedExpression> TransformLikeClause(PEGTransformer &transformer, const string &like_variations,
	                                                        unique_ptr<ParsedExpression> other_operator_expression,
	                                                        optional<unique_ptr<ParsedExpression>> escape_clause);
	static unique_ptr<ParsedExpression> TransformEscapeClause(PEGTransformer &transformer,
	                                                          unique_ptr<ParsedExpression> comparison_expression);
	static string TransformLikeToken(PEGTransformer &transformer);
	static string TransformILikeToken(PEGTransformer &transformer);
	static string TransformGlobToken(PEGTransformer &transformer);
	static string TransformSimilarToToken(PEGTransformer &transformer);
	static string TransformRegexMatchToken(PEGTransformer &transformer);
	static string TransformRegexInsensitiveMatchToken(PEGTransformer &transformer);
	static string TransformNotILikeOp(PEGTransformer &transformer);
	static string TransformNotLikeOp(PEGTransformer &transformer);
	static string TransformNotRegexInsensitiveMatchOp(PEGTransformer &transformer);
	static string TransformNotSimilarToOp(PEGTransformer &transformer);
	static unique_ptr<ParsedExpression> TransformInClause(PEGTransformer &transformer,
	                                                      unique_ptr<ParsedExpression> in_expression);
	static unique_ptr<ParsedExpression>
	TransformInContainsExpression(PEGTransformer &transformer, unique_ptr<ParsedExpression> other_operator_expression);
	static unique_ptr<ParsedExpression> TransformInExpressionList(PEGTransformer &transformer,
	                                                              vector<unique_ptr<ParsedExpression>> expression);
	static unique_ptr<ParsedExpression>
	TransformInSelectStatement(PEGTransformer &transformer, unique_ptr<SelectStatement> select_statement_internal);
	static unique_ptr<ParsedExpression>
	TransformBetweenClause(PEGTransformer &transformer, unique_ptr<ParsedExpression> other_operator_expression,
	                       unique_ptr<ParsedExpression> other_operator_expression_1);
	static unique_ptr<ParsedExpression>
	TransformOtherOperatorExpression(PEGTransformer &transformer, unique_ptr<ParsedExpression> bitwise_expression,
	                                 optional<vector<OtherOperatorTail>> other_operator_tail);
	static OtherOperatorTail TransformOtherOperatorTail(PEGTransformer &transformer, ParsedOperator other_operator,
	                                                    unique_ptr<ParsedExpression> bitwise_expression);
	static ParsedOperator TransformAnyAllParsedOperator(PEGTransformer &transformer,
	                                                    const pair<string, bool> &any_all_operator);
	static ParsedOperator TransformNamedOtherOperator(PEGTransformer &transformer, const string &child);
	static pair<string, bool> TransformAnyAllOperator(PEGTransformer &transformer, const string &any_op,
	                                                  const bool &any_or_all);
	static bool TransformSubqueryAny(PEGTransformer &transformer);
	static bool TransformSubqueryAll(PEGTransformer &transformer);
	static string TransformQualifiedOperator(PEGTransformer &transformer, const string &qualified_operator_contents);
	static string TransformQualifiedOperatorContents(PEGTransformer &transformer,
	                                                 const optional<vector<string>> &col_id_dot, const string &any_op);
	static unique_ptr<ParsedExpression>
	TransformBitwiseExpression(PEGTransformer &transformer, unique_ptr<ParsedExpression> additive_expression,
	                           optional<vector<BinaryExpressionTail>> bitwise_expression_tail);
	static BinaryExpressionTail TransformBitwiseExpressionTail(PEGTransformer &transformer, const string &bit_operator,
	                                                           unique_ptr<ParsedExpression> additive_expression);
	static unique_ptr<ParsedExpression>
	TransformAdditiveExpression(PEGTransformer &transformer, unique_ptr<ParsedExpression> multiplicative_expression,
	                            optional<vector<BinaryExpressionTail>> additive_expression_tail);
	static BinaryExpressionTail TransformAdditiveExpressionTail(PEGTransformer &transformer, const string &term,
	                                                            unique_ptr<ParsedExpression> multiplicative_expression,
	                                                            optional_idx query_location);
	static unique_ptr<ParsedExpression>
	TransformMultiplicativeExpression(PEGTransformer &transformer,
	                                  unique_ptr<ParsedExpression> exponentiation_expression,
	                                  optional<vector<BinaryExpressionTail>> multiplicative_expression_tail);
	static BinaryExpressionTail
	TransformMultiplicativeExpressionTail(PEGTransformer &transformer, const string &factor,
	                                      unique_ptr<ParsedExpression> exponentiation_expression);
	static unique_ptr<ParsedExpression>
	TransformExponentiationExpression(PEGTransformer &transformer, unique_ptr<ParsedExpression> collate_expression,
	                                  optional<vector<BinaryExpressionTail>> exponentiation_expression_tail);
	static BinaryExpressionTail TransformExponentiationExpressionTail(PEGTransformer &transformer,
	                                                                  const string &exponent_operator,
	                                                                  unique_ptr<ParsedExpression> collate_expression);
	static unique_ptr<ParsedExpression>
	TransformCollateExpression(PEGTransformer &transformer, unique_ptr<ParsedExpression> at_time_zone_expression,
	                           optional<vector<unique_ptr<ParsedExpression>>> collate_expression_tail);
	static unique_ptr<ParsedExpression>
	TransformCollateExpressionTail(PEGTransformer &transformer, unique_ptr<ParsedExpression> at_time_zone_expression);
	static unique_ptr<ParsedExpression>
	TransformAtTimeZoneExpression(PEGTransformer &transformer, unique_ptr<ParsedExpression> prefix_expression,
	                              optional<vector<unique_ptr<ParsedExpression>>> at_time_zone_expression_tail);
	static unique_ptr<ParsedExpression>
	TransformAtTimeZoneExpressionTail(PEGTransformer &transformer, unique_ptr<ParsedExpression> prefix_expression);
	static unique_ptr<ParsedExpression>
	TransformBaseExpression(PEGTransformer &transformer, unique_ptr<ParsedExpression> single_expression,
	                        optional<vector<unique_ptr<ParsedExpression>>> indirection_list);
	static vector<unique_ptr<ParsedExpression>>
	TransformIndirectionList(PEGTransformer &transformer, vector<unique_ptr<ParsedExpression>> indirection);
	static unique_ptr<ParsedExpression> TransformCastOperator(PEGTransformer &transformer, const LogicalType &type);
	static unique_ptr<ParsedExpression> TransformDotMethodOperator(PEGTransformer &transformer,
	                                                               unique_ptr<ParsedExpression> method_expression);
	static unique_ptr<ParsedExpression> TransformDotColumnOperator(PEGTransformer &transformer,
	                                                               const string &col_label);
	static unique_ptr<ParsedExpression> TransformMethodExpression(PEGTransformer &transformer, const string &col_label,
	                                                              MethodArguments method_expression_arguments);
	static MethodArguments TransformMethodExpressionArguments(PEGTransformer &transformer,
	                                                          MethodArguments method_expression_argument_list);
	static MethodArguments
	TransformMethodExpressionArgumentList(PEGTransformer &transformer, const optional<bool> &distinct_or_all,
	                                      optional<vector<FunctionArgument>> method_function_arguments,
	                                      optional<vector<OrderByNode>> order_by_clause,
	                                      const optional<bool> &ignore_or_respect_nulls);
	static vector<FunctionArgument> TransformMethodFunctionArguments(PEGTransformer &transformer,
	                                                                 vector<FunctionArgument> function_argument);
	static unique_ptr<ParsedExpression> TransformSliceExpression(PEGTransformer &transformer,
	                                                             vector<unique_ptr<ParsedExpression>> slice_bound);
	static vector<unique_ptr<ParsedExpression>>
	TransformSliceBound(PEGTransformer &transformer, optional<unique_ptr<ParsedExpression>> expression,
	                    optional<unique_ptr<ParsedExpression>> end_slice_bound,
	                    optional<unique_ptr<ParsedExpression>> step_slice_bound);
	static unique_ptr<ParsedExpression> TransformEndSliceBound(PEGTransformer &transformer,
	                                                           optional<unique_ptr<ParsedExpression>> end_slice_value);
	static unique_ptr<ParsedExpression> TransformEndSliceMinus(PEGTransformer &transformer);
	static unique_ptr<ParsedExpression> TransformStepSliceBound(PEGTransformer &transformer,
	                                                            optional<unique_ptr<ParsedExpression>> expression);
	static unique_ptr<ParsedExpression> TransformPostfixOperator(PEGTransformer &transformer);
	static unique_ptr<ParsedExpression> TransformCoalesceExpression(PEGTransformer &transformer,
	                                                                vector<unique_ptr<ParsedExpression>> expression);
	static unique_ptr<ParsedExpression> TransformUnpackExpression(PEGTransformer &transformer,
	                                                              unique_ptr<ParsedExpression> expression);
	static unique_ptr<ParsedExpression> TransformTryExpression(PEGTransformer &transformer,
	                                                           unique_ptr<ParsedExpression> expression);
	static unique_ptr<ParsedExpression> TransformColumnsExpression(PEGTransformer &transformer, const bool &has_result,
	                                                               unique_ptr<ParsedExpression> expression);
	static unique_ptr<ParsedExpression>
	TransformExtractExpression(PEGTransformer &transformer, vector<unique_ptr<ParsedExpression>> extract_arguments);
	static vector<unique_ptr<ParsedExpression>> TransformExtractArguments(PEGTransformer &transformer,
	                                                                      unique_ptr<ParsedExpression> extract_argument,
	                                                                      unique_ptr<ParsedExpression> expression);
	static unique_ptr<ParsedExpression> TransformLambdaExpression(PEGTransformer &transformer,
	                                                              const vector<Identifier> &col_id_or_string,
	                                                              unique_ptr<ParsedExpression> expression);
	static unique_ptr<ParsedExpression>
	TransformNullIfExpression(PEGTransformer &transformer, vector<unique_ptr<ParsedExpression>> null_if_arguments);
	static vector<unique_ptr<ParsedExpression>> TransformNullIfArguments(PEGTransformer &transformer,
	                                                                     unique_ptr<ParsedExpression> expression,
	                                                                     unique_ptr<ParsedExpression> expression_1);
	static unique_ptr<ParsedExpression>
	TransformPositionExpression(PEGTransformer &transformer, vector<unique_ptr<ParsedExpression>> position_arguments);
	static vector<unique_ptr<ParsedExpression>>
	TransformPositionArguments(PEGTransformer &transformer, unique_ptr<ParsedExpression> other_operator_expression,
	                           unique_ptr<ParsedExpression> expression);
	static unique_ptr<ParsedExpression>
	TransformRowExpression(PEGTransformer &transformer, optional<vector<unique_ptr<ParsedExpression>>> expression);
	static unique_ptr<ParsedExpression>
	TransformSubstringExpression(PEGTransformer &transformer, vector<unique_ptr<ParsedExpression>> substring_arguments);
	static vector<unique_ptr<ParsedExpression>>
	TransformSubstringExpressionList(PEGTransformer &transformer, vector<unique_ptr<ParsedExpression>> expression);
	static vector<unique_ptr<ParsedExpression>>
	TransformSubstringParameters(PEGTransformer &transformer, unique_ptr<ParsedExpression> expression,
	                             vector<unique_ptr<ParsedExpression>> substring_from_for);
	static vector<unique_ptr<ParsedExpression>>
	TransformSubstringFromOptionalFor(PEGTransformer &transformer, unique_ptr<ParsedExpression> from_expression,
	                                  optional<unique_ptr<ParsedExpression>> for_expression);
	static vector<unique_ptr<ParsedExpression>> TransformSubstringFor(PEGTransformer &transformer,
	                                                                  unique_ptr<ParsedExpression> for_expression);
	static unique_ptr<ParsedExpression> TransformTrimExpression(PEGTransformer &transformer,
	                                                            TrimArguments trim_arguments);
	static TrimArguments TransformTrimArguments(PEGTransformer &transformer, const optional<string> &trim_direction,
	                                            optional<unique_ptr<ParsedExpression>> trim_source,
	                                            vector<unique_ptr<ParsedExpression>> expression);
	static string TransformTrimBoth(PEGTransformer &transformer);
	static string TransformTrimLeading(PEGTransformer &transformer);
	static string TransformTrimTrailing(PEGTransformer &transformer);
	static unique_ptr<ParsedExpression> TransformTrimSource(PEGTransformer &transformer,
	                                                        optional<unique_ptr<ParsedExpression>> expression);
	static unique_ptr<ParsedExpression>
	TransformOverlayExpression(PEGTransformer &transformer, vector<unique_ptr<ParsedExpression>> overlay_arguments);
	static vector<unique_ptr<ParsedExpression>>
	TransformOverlayParameters(PEGTransformer &transformer, unique_ptr<ParsedExpression> expression,
	                           unique_ptr<ParsedExpression> expression_1, unique_ptr<ParsedExpression> from_expression,
	                           optional<unique_ptr<ParsedExpression>> for_expression);
	static unique_ptr<ParsedExpression> TransformFromExpression(PEGTransformer &transformer,
	                                                            unique_ptr<ParsedExpression> expression);
	static unique_ptr<ParsedExpression> TransformForExpression(PEGTransformer &transformer,
	                                                           unique_ptr<ParsedExpression> expression);
	static vector<unique_ptr<ParsedExpression>>
	TransformOverlayExpressionList(PEGTransformer &transformer, vector<unique_ptr<ParsedExpression>> expression);
	static unique_ptr<ParsedExpression> TransformExtractDatePartArgument(PEGTransformer &transformer,
	                                                                     const DatePartSpecifier &extract_date_part);
	static unique_ptr<ParsedExpression> TransformExtractIdentifierArgument(PEGTransformer &transformer,
	                                                                       const Identifier &identifier);
	static unique_ptr<ParsedExpression> TransformExtractStringArgument(PEGTransformer &transformer,
	                                                                   const string &string_literal);
	static unique_ptr<SQLStatement>
	TransformCreateExternalResourceStmt(PEGTransformer &transformer, const string &string_literal,
	                                    const optional<Identifier> &attach_alias,
	                                    const optional<vector<GenericCopyOption>> &external_resource_creation_options);
	static unique_ptr<SQLStatement> TransformRegisterExternalResourceStmt(PEGTransformer &transformer,
	                                                                      const string &string_literal,
	                                                                      const optional<Identifier> &attach_alias,
	                                                                      unique_ptr<ParsedExpression> expression);
	static unique_ptr<SQLStatement> TransformDestroyExternalResourceStmt(PEGTransformer &transformer,
	                                                                     const Identifier &col_id);
	static unique_ptr<SQLStatement> TransformShowExternalResourcesStmt(PEGTransformer &transformer,
	                                                                   const optional<bool> &show_all_modifier);
	static bool TransformShowAllModifier(PEGTransformer &transformer);
	static unique_ptr<SQLStatement> TransformAttachToExternalResource(
	    PEGTransformer &transformer, unique_ptr<ExternalResourceOptions> external_resource_source,
	    const Identifier &attach_alias, const optional<vector<GenericCopyOption>> &attach_options);
	static unique_ptr<SQLStatement>
	TransformConnectToExternalResource(PEGTransformer &transformer,
	                                   unique_ptr<ExternalResourceOptions> external_resource_source,
	                                   const optional<vector<GenericCopyOption>> &attach_options);
	static unique_ptr<ExternalResourceOptions> TransformExternalResourceCreateClause(
	    PEGTransformer &transformer, const string &string_literal,
	    const optional<vector<GenericCopyOption>> &external_resource_creation_options);
	static unique_ptr<ExternalResourceOptions> TransformExternalResourceReferenceClause(PEGTransformer &transformer,
	                                                                                    const Identifier &col_id);
	static unique_ptr<SQLStatement>
	TransformInsertStatement(PEGTransformer &transformer, optional<CommonTableExpressionMap> with_clause,
	                         const optional<OnConflictAction> &or_action, unique_ptr<BaseTableRef> insert_target,
	                         const optional<InsertColumnOrder> &by_name_or_position,
	                         const optional<vector<string>> &insert_column_list, InsertValues insert_values,
	                         optional<unique_ptr<OnConflictInfo>> on_conflict_clause,
	                         optional<vector<unique_ptr<ParsedExpression>>> returning_clause);
	static OnConflictAction TransformInsertOrReplace(PEGTransformer &transformer);
	static OnConflictAction TransformInsertOrIgnore(PEGTransformer &transformer);
	static InsertColumnOrder TransformInsertByName(PEGTransformer &transformer);
	static InsertColumnOrder TransformInsertByPosition(PEGTransformer &transformer);
	static unique_ptr<BaseTableRef> TransformInsertTarget(PEGTransformer &transformer,
	                                                      unique_ptr<BaseTableRef> base_table_name,
	                                                      const optional<Identifier> &insert_alias);
	static Identifier TransformInsertAlias(PEGTransformer &transformer, const Identifier &identifier);
	static vector<string> TransformColumnList(PEGTransformer &transformer, const vector<Identifier> &col_id);
	static vector<string> TransformInsertColumnList(PEGTransformer &transformer, const vector<string> &column_list);
	static InsertValues TransformSelectInsertValues(PEGTransformer &transformer,
	                                                unique_ptr<SelectStatement> select_statement_internal);
	static InsertValues TransformDefaultValues(PEGTransformer &transformer);
	static unique_ptr<OnConflictInfo> TransformOnConflictClause(PEGTransformer &transformer,
	                                                            optional<OnConflictExpressionTarget> on_conflict_target,
	                                                            unique_ptr<OnConflictInfo> on_conflict_action);
	static OnConflictExpressionTarget
	TransformOnConflictExpressionTarget(PEGTransformer &transformer, const vector<string> &column_id_list,
	                                    optional<unique_ptr<ParsedExpression>> where_clause);
	static OnConflictExpressionTarget TransformOnConflictIndexTarget(PEGTransformer &transformer,
	                                                                 const Identifier &constraint_name);
	static unique_ptr<OnConflictInfo> TransformOnConflictUpdate(PEGTransformer &transformer,
	                                                            unique_ptr<UpdateSetInfo> update_set_clause,
	                                                            optional<unique_ptr<ParsedExpression>> where_clause);
	static unique_ptr<OnConflictInfo> TransformOnConflictNothing(PEGTransformer &transformer);
	static vector<unique_ptr<ParsedExpression>>
	TransformReturningClause(PEGTransformer &transformer, vector<unique_ptr<ParsedExpression>> target_list);
	static unique_ptr<SQLStatement> TransformLoadStatement(PEGTransformer &transformer,
	                                                       const Identifier &col_id_or_string,
	                                                       const optional<ExtensionRepositoryInfo> &from_source,
	                                                       const optional<Identifier> &extension_alias);
	static Identifier TransformExtensionAlias(PEGTransformer &transformer, const Identifier &identifier);
	static unique_ptr<SQLStatement> TransformInstallStatement(PEGTransformer &transformer, const bool &has_result,
	                                                          const optional<bool> &install_and_load,
	                                                          const QualifiedName &identifier_or_string_literal,
	                                                          const optional<ExtensionRepositoryInfo> &from_source,
	                                                          const optional<string> &version_number);
	static bool TransformInstallAndLoad(PEGTransformer &transformer);
	static unique_ptr<SQLStatement> TransformUpdateExtensionsStatement(PEGTransformer &transformer,
	                                                                   const optional<vector<Identifier>> &identifier);
	static ExtensionRepositoryInfo TransformFromSourceIdentifier(PEGTransformer &transformer,
	                                                             const Identifier &identifier);
	static ExtensionRepositoryInfo TransformFromSourceString(PEGTransformer &transformer, const string &string_literal);
	static string TransformVersionNumber(PEGTransformer &transformer,
	                                     const QualifiedName &identifier_or_string_literal);
	static unique_ptr<SQLStatement>
	TransformCreateExtensionRepositoryStmt(PEGTransformer &transformer, const optional<bool> &or_replace,
	                                       const optional<bool> &if_not_exists, const Identifier &col_id_or_string,
	                                       const string &repository_prefix,
	                                       const optional<vector<string>> &repository_public_key);
	static string TransformRepositoryPrefix(PEGTransformer &transformer, const bool &has_result,
	                                        const string &string_literal);
	static vector<string> TransformRepositoryPublicKey(PEGTransformer &transformer, const bool &has_result,
	                                                   const vector<string> &string_literal);
	static unique_ptr<SQLStatement> TransformDropExtensionRepositoryStmt(PEGTransformer &transformer,
	                                                                     const optional<bool> &if_exists,
	                                                                     const Identifier &col_id_or_string);
	static unique_ptr<SQLStatement>
	TransformMergeIntoStatement(PEGTransformer &transformer, optional<CommonTableExpressionMap> with_clause,
	                            unique_ptr<BaseTableRef> target_opt_alias, unique_ptr<TableRef> merge_into_using_clause,
	                            JoinQualifier join_qualifier,
	                            vector<pair<MergeActionCondition, unique_ptr<MergeIntoAction>>> merge_match,
	                            optional<vector<unique_ptr<ParsedExpression>>> returning_clause);
	static unique_ptr<TableRef> TransformMergeIntoUsingClause(PEGTransformer &transformer,
	                                                          unique_ptr<TableRef> table_ref);
	static pair<MergeActionCondition, unique_ptr<MergeIntoAction>>
	TransformMatchedClause(PEGTransformer &transformer, optional<unique_ptr<ParsedExpression>> and_expression,
	                       unique_ptr<MergeIntoAction> matched_clause_action);
	static unique_ptr<MergeIntoAction>
	TransformUpdateMatchClause(PEGTransformer &transformer, optional<unique_ptr<MergeIntoAction>> update_match_info);
	static unique_ptr<MergeIntoAction> TransformUpdateMatchSetAction(PEGTransformer &transformer,
	                                                                 unique_ptr<UpdateSetInfo> update_match_set_clause);
	static unique_ptr<MergeIntoAction> TransformUpdateByNameOrPosition(PEGTransformer &transformer,
	                                                                   const InsertColumnOrder &by_name_or_position);
	static unique_ptr<MergeIntoAction> TransformDeleteMatchClause(PEGTransformer &transformer);
	static unique_ptr<MergeIntoAction>
	TransformInsertMatchClause(PEGTransformer &transformer, optional<unique_ptr<MergeIntoAction>> insert_match_info);
	static unique_ptr<MergeIntoAction> TransformInsertDefaultValues(PEGTransformer &transformer);
	static unique_ptr<MergeIntoAction>
	TransformInsertByNameOrPosition(PEGTransformer &transformer, const optional<InsertColumnOrder> &by_name_or_position,
	                                const bool &has_result);
	static unique_ptr<MergeIntoAction> TransformInsertValuesList(PEGTransformer &transformer,
	                                                             const optional<vector<string>> &insert_column_list,
	                                                             vector<unique_ptr<ParsedExpression>> expression);
	static unique_ptr<MergeIntoAction> TransformDoNothingMatchClause(PEGTransformer &transformer);
	static unique_ptr<MergeIntoAction> TransformErrorMatchClause(PEGTransformer &transformer,
	                                                             optional<unique_ptr<ParsedExpression>> expression);
	static unique_ptr<ParsedExpression> TransformAndExpression(PEGTransformer &transformer,
	                                                           unique_ptr<ParsedExpression> expression);
	static pair<MergeActionCondition, unique_ptr<MergeIntoAction>>
	TransformNotMatchedClause(PEGTransformer &transformer, const optional<MergeActionCondition> &by_source_or_target,
	                          optional<unique_ptr<ParsedExpression>> and_expression,
	                          unique_ptr<MergeIntoAction> matched_clause_action);
	static MergeActionCondition TransformBySource(PEGTransformer &transformer);
	static MergeActionCondition TransformByTarget(PEGTransformer &transformer);
	static vector<PivotColumn> TransformPivotOn(PEGTransformer &transformer, vector<PivotColumn> pivot_column_list);
	static vector<unique_ptr<ParsedExpression>> TransformPivotUsing(PEGTransformer &transformer,
	                                                                vector<unique_ptr<ParsedExpression>> target_list);
	static vector<PivotColumn> TransformPivotColumnList(PEGTransformer &transformer,
	                                                    vector<PivotColumn> pivot_column_entry);
	static PivotColumn TransformPivotColumnExpression(PEGTransformer &transformer,
	                                                  unique_ptr<ParsedExpression> expression);
	static PivotColumn TransformPivotColumnSubquery(PEGTransformer &transformer,
	                                                unique_ptr<ParsedExpression> base_expression,
	                                                unique_ptr<SelectStatement> select_statement_internal);
	static UnpivotNameValues TransformIntoNameValues(PEGTransformer &transformer, const Identifier &col_id_or_string,
	                                                 const vector<string> &optional_parens_name_list);
	static vector<string> TransformParenthesizedNameList(PEGTransformer &transformer,
	                                                     const vector<Identifier> &col_id_or_string);
	static vector<string> TransformBareNameList(PEGTransformer &transformer,
	                                            const vector<Identifier> &col_id_or_string);
	static bool TransformIncludeNulls(PEGTransformer &transformer);
	static bool TransformExcludeNulls(PEGTransformer &transformer);
	static vector<string> TransformUnpivotHeaderSingle(PEGTransformer &transformer, const Identifier &col_id_or_string);
	static vector<string> TransformUnpivotHeaderList(PEGTransformer &transformer,
	                                                 const vector<Identifier> &col_id_or_string);
	static unique_ptr<SQLStatement> TransformPragmaStatement(PEGTransformer &transformer,
	                                                         unique_ptr<SQLStatement> pragma_assign_or_function);
	static unique_ptr<SQLStatement> TransformPragmaAssign(PEGTransformer &transformer, const Identifier &setting_name,
	                                                      vector<unique_ptr<ParsedExpression>> variable_list);
	static unique_ptr<SQLStatement>
	TransformPragmaFunction(PEGTransformer &transformer, const Identifier &pragma_name,
	                        optional<vector<unique_ptr<ParsedExpression>>> pragma_parameters);
	static vector<unique_ptr<ParsedExpression>>
	TransformPragmaParameters(PEGTransformer &transformer, vector<unique_ptr<ParsedExpression>> expression);
	static unique_ptr<SQLStatement> TransformPrepareStatement(PEGTransformer &transformer, const Identifier &identifier,
	                                                          const optional<vector<LogicalType>> &type_list,
	                                                          unique_ptr<SQLStatement> statement);
	static vector<LogicalType> TransformTypeList(PEGTransformer &transformer, const vector<LogicalType> &type);
	static unique_ptr<SQLStatement> TransformSelectStatement(PEGTransformer &transformer,
	                                                         unique_ptr<SelectStatement> select_statement_internal);
	static unique_ptr<SelectStatement> TransformSelectSetOpChain(
	    PEGTransformer &transformer, unique_ptr<SelectStatement> intersect_chain,
	    optional<vector<pair<unique_ptr<SetOperationNode>, unique_ptr<SelectStatement>>>> select_set_op_chain_tail);
	static pair<unique_ptr<SetOperationNode>, unique_ptr<SelectStatement>>
	TransformSelectSetOpChainTail(PEGTransformer &transformer, unique_ptr<SetOperationNode> setop_clause,
	                              unique_ptr<SelectStatement> intersect_chain);
	static unique_ptr<SelectStatement> TransformIntersectChain(
	    PEGTransformer &transformer, unique_ptr<SelectStatement> select_atom,
	    optional<vector<pair<unique_ptr<SetOperationNode>, unique_ptr<SelectStatement>>>> intersect_chain_tail);
	static pair<unique_ptr<SetOperationNode>, unique_ptr<SelectStatement>>
	TransformIntersectChainTail(PEGTransformer &transformer, unique_ptr<SetOperationNode> set_intersect_clause,
	                            unique_ptr<SelectStatement> select_atom);
	static unique_ptr<SetOperationNode> TransformSetIntersectClause(PEGTransformer &transformer,
	                                                                const optional<bool> &distinct_or_all);
	static unique_ptr<SelectStatement> TransformSelectParens(PEGTransformer &transformer,
	                                                         unique_ptr<SelectStatement> select_statement_internal);
	static unique_ptr<SetOperationNode> TransformSetopClause(PEGTransformer &transformer,
	                                                         const SetOperationType &setop_type,
	                                                         const optional<bool> &distinct_or_all,
	                                                         const bool &has_result);
	static SetOperationType TransformSetopUnion(PEGTransformer &transformer);
	static SetOperationType TransformSetopExcept(PEGTransformer &transformer);
	static vector<unique_ptr<ResultModifier>>
	TransformResultModifiers(PEGTransformer &transformer, optional<vector<OrderByNode>> order_by_clause,
	                         optional<unique_ptr<ResultModifier>> limit_offset);
	static unique_ptr<ResultModifier> TransformLimitOffsetClause(PEGTransformer &transformer,
	                                                             LimitPercentResult limit_clause,
	                                                             optional<LimitPercentResult> offset_clause);
	static unique_ptr<ResultModifier> TransformOffsetLimitClause(PEGTransformer &transformer,
	                                                             LimitPercentResult offset_clause,
	                                                             optional<LimitPercentResult> limit_clause);
	static unique_ptr<ResultModifier> TransformOffsetFetchClause(PEGTransformer &transformer,
	                                                             LimitPercentResult offset_clause,
	                                                             LimitPercentResult fetch_clause);
	static unique_ptr<ResultModifier> TransformFetchOnlyClause(PEGTransformer &transformer,
	                                                           LimitPercentResult fetch_clause);
	static unique_ptr<SelectStatement> TransformTableStatement(PEGTransformer &transformer,
	                                                           unique_ptr<BaseTableRef> base_table_name);
	static unique_ptr<SelectStatement> TransformSimpleSelectParens(PEGTransformer &transformer,
	                                                               unique_ptr<SelectStatement> simple_select);
	static unique_ptr<SelectNode> TransformSelectFromClause(PEGTransformer &transformer,
	                                                        unique_ptr<SelectNode> select_clause,
	                                                        optional<unique_ptr<TableRef>> from_clause);
	static unique_ptr<SelectNode> TransformFromSelectClause(PEGTransformer &transformer,
	                                                        unique_ptr<TableRef> from_clause,
	                                                        optional<unique_ptr<SelectNode>> select_clause);
	static pair<Identifier, unique_ptr<CommonTableExpressionInfo>>
	TransformWithStatement(PEGTransformer &transformer, const Identifier &col_id_or_string,
	                       const optional<vector<string>> &insert_column_list,
	                       optional<vector<unique_ptr<ParsedExpression>>> using_key, const optional<bool> &materialized,
	                       unique_ptr<TableRef> cte_body);
	static unique_ptr<TableRef> TransformCTESelectBody(PEGTransformer &transformer,
	                                                   unique_ptr<SelectStatement> select_statement_internal);
	static unique_ptr<TableRef> TransformCTEDMLBody(PEGTransformer &transformer, unique_ptr<SQLStatement> statement);
	static vector<unique_ptr<ParsedExpression>> TransformUsingKey(PEGTransformer &transformer,
	                                                              vector<unique_ptr<ParsedExpression>> target_list);
	static bool TransformMaterialized(PEGTransformer &transformer, const bool &has_result);
	static unique_ptr<SelectNode> TransformSelectClause(PEGTransformer &transformer,
	                                                    optional<DistinctClause> distinct_clause,
	                                                    optional<vector<unique_ptr<ParsedExpression>>> target_list);
	static vector<unique_ptr<ParsedExpression>>
	TransformTargetList(PEGTransformer &transformer, vector<unique_ptr<ParsedExpression>> aliased_expression);
	static vector<string> TransformColumnAliases(PEGTransformer &transformer,
	                                             const vector<Identifier> &col_id_or_string);
	static DistinctClause TransformDistinctAll(PEGTransformer &transformer);
	static DistinctClause TransformDistinctOn(PEGTransformer &transformer,
	                                          optional<vector<unique_ptr<ParsedExpression>>> distinct_on_targets);
	static vector<unique_ptr<ParsedExpression>>
	TransformDistinctOnTargets(PEGTransformer &transformer, vector<unique_ptr<ParsedExpression>> expression);
	static unique_ptr<TableRef> TransformTableSubquery(PEGTransformer &transformer,
	                                                   const optional<Identifier> &table_alias_colon,
	                                                   const optional<bool> &lateral,
	                                                   unique_ptr<TableRef> subquery_reference,
	                                                   const optional<TableAlias> &table_alias);
	static unique_ptr<TableRef>
	TransformBaseTableRef(PEGTransformer &transformer, const optional<Identifier> &table_alias_colon,
	                      unique_ptr<BaseTableRef> base_table_name, const optional<TableAlias> &table_alias,
	                      optional<unique_ptr<AtClause>> at_clause, optional<unique_ptr<SampleOptions>> sample_clause);
	static Identifier TransformTableAliasColon(PEGTransformer &transformer, const Identifier &col_id_or_string);
	static unique_ptr<TableRef> TransformValuesRef(PEGTransformer &transformer,
	                                               const optional<Identifier> &table_alias_colon,
	                                               unique_ptr<SelectStatement> values_clause,
	                                               const optional<TableAlias> &table_alias);
	static unique_ptr<TableRef> TransformParensTableRef(PEGTransformer &transformer,
	                                                    const optional<Identifier> &table_alias_colon,
	                                                    unique_ptr<TableRef> table_ref,
	                                                    const optional<TableAlias> &table_alias,
	                                                    optional<unique_ptr<SampleOptions>> sample_clause);
	static unique_ptr<TableRef> TransformTablePivotClause(PEGTransformer &transformer,
	                                                      unique_ptr<TableRef> table_pivot_clause_body,
	                                                      const optional<TableAlias> &table_alias);
	static unique_ptr<TableRef> TransformTablePivotClauseBody(PEGTransformer &transformer,
	                                                          vector<unique_ptr<ParsedExpression>> target_list,
	                                                          vector<PivotColumn> pivot_value_list,
	                                                          const optional<vector<string>> &pivot_group_by_list);
	static vector<string> TransformPivotGroupByList(PEGTransformer &transformer,
	                                                const vector<string> &optional_parens_name_list);
	static unique_ptr<TableRef> TransformTableUnpivotClause(PEGTransformer &transformer,
	                                                        const optional<bool> &include_or_exclude_nulls,
	                                                        unique_ptr<TableRef> table_unpivot_clause_body,
	                                                        const optional<TableAlias> &table_alias);
	static unique_ptr<TableRef> TransformTableUnpivotClauseBody(PEGTransformer &transformer,
	                                                            const vector<string> &unpivot_header,
	                                                            vector<PivotColumn> unpivot_value_list);
	static unique_ptr<ParsedExpression> TransformPivotHeader(PEGTransformer &transformer,
	                                                         unique_ptr<ParsedExpression> base_expression);
	static PivotColumn TransformPivotValueList(PEGTransformer &transformer, unique_ptr<ParsedExpression> pivot_header,
	                                           PivotColumn pivot_value_target);
	static PivotColumn TransformPivotEnumTarget(PEGTransformer &transformer, const Identifier &identifier);
	static PivotColumn TransformPivotListTarget(PEGTransformer &transformer,
	                                            vector<PivotColumnEntry> pivot_target_list);
	static PivotColumn TransformUnpivotValueList(PEGTransformer &transformer, const vector<string> &unpivot_header,
	                                             vector<PivotColumnEntry> unpivot_target_list);
	static vector<PivotColumnEntry> TransformPivotTargetList(PEGTransformer &transformer,
	                                                         vector<unique_ptr<ParsedExpression>> target_list);
	static vector<PivotColumnEntry> TransformUnpivotTargetList(PEGTransformer &transformer,
	                                                           vector<unique_ptr<ParsedExpression>> target_list);
	static bool TransformLateral(PEGTransformer &transformer);
	static unique_ptr<BaseTableRef> TransformUnqualifiedBaseTableName(PEGTransformer &transformer,
	                                                                  const Identifier &table_name);
	static unique_ptr<BaseTableRef> TransformSchemaReservedTable(PEGTransformer &transformer,
	                                                             const Identifier &schema_qualification,
	                                                             const Identifier &reserved_table_name);
	static unique_ptr<BaseTableRef>
	TransformCatalogReservedSchemaTable(PEGTransformer &transformer, const Identifier &catalog_qualification,
	                                    const vector<Identifier> &reserved_schema_qualification,
	                                    const Identifier &reserved_table_name);
	static unique_ptr<TableRef> TransformTableFunctionLateralOpt(PEGTransformer &transformer,
	                                                             const optional<bool> &lateral,
	                                                             const QualifiedName &qualified_table_function,
	                                                             vector<FunctionArgument> table_function_arguments,
	                                                             const optional<bool> &with_ordinality,
	                                                             const optional<TableAlias> &table_alias);
	static unique_ptr<TableRef> TransformTableFunctionAliasColon(PEGTransformer &transformer,
	                                                             const Identifier &table_alias_colon,
	                                                             const QualifiedName &qualified_table_function,
	                                                             vector<FunctionArgument> table_function_arguments,
	                                                             const optional<bool> &with_ordinality,
	                                                             optional<unique_ptr<SampleOptions>> sample_clause);
	static bool TransformWithOrdinality(PEGTransformer &transformer);
	static QualifiedName TransformQualifiedTableFunction(PEGTransformer &transformer,
	                                                     const optional<Identifier> &catalog_qualification,
	                                                     const optional<vector<Identifier>> &schema_qualification,
	                                                     const Identifier &table_function_name);
	static vector<FunctionArgument>
	TransformTableFunctionArguments(PEGTransformer &transformer, optional<vector<FunctionArgument>> function_argument);
	static FunctionArgument TransformNamedFunctionArgument(PEGTransformer &transformer, MacroParameter named_parameter);
	static FunctionArgument TransformPositionalFunctionArgument(PEGTransformer &transformer,
	                                                            unique_ptr<ParsedExpression> expression);
	static MacroParameter TransformNamedParameter(PEGTransformer &transformer, const Identifier &type_func_name,
	                                              const optional<LogicalType> &type,
	                                              unique_ptr<ParsedExpression> expression);
	static TableAlias TransformTableAliasAs(PEGTransformer &transformer,
	                                        const QualifiedName &identifier_or_string_literal,
	                                        const optional<vector<string>> &column_aliases);
	static TableAlias TransformTableAliasWithoutAs(PEGTransformer &transformer, const Identifier &identifier,
	                                               const optional<vector<string>> &column_aliases);
	static unique_ptr<AtClause> TransformAtClause(PEGTransformer &transformer, unique_ptr<AtClause> at_specifier);
	static unique_ptr<AtClause> TransformAtSpecifier(PEGTransformer &transformer, const string &at_unit,
	                                                 unique_ptr<ParsedExpression> expression);
	static string TransformVersionAtUnit(PEGTransformer &transformer);
	static string TransformTimestampAtUnit(PEGTransformer &transformer);
	static unique_ptr<TableRef>
	TransformNearestJoinAliased(PEGTransformer &transformer, const optional<JoinType> &join_type,
	                            unique_ptr<TableRef> table_ref, const optional<bool> &approx_or_exact,
	                            optional<unique_ptr<ParsedExpression>> number_literal,
	                            const OrderType &distance_or_similarity, unique_ptr<ParsedExpression> expression);
	static unique_ptr<TableRef>
	TransformNearestJoinBare(PEGTransformer &transformer, const optional<JoinType> &join_type,
	                         unique_ptr<TableRef> nearest_bare_table_ref, const optional<bool> &approx_or_exact,
	                         optional<unique_ptr<ParsedExpression>> number_literal,
	                         const OrderType &distance_or_similarity, unique_ptr<ParsedExpression> expression);
	static unique_ptr<TableRef> TransformNearestValuesRef(PEGTransformer &transformer,
	                                                      unique_ptr<SelectStatement> values_clause);
	static unique_ptr<TableRef> TransformNearestTableFunction(PEGTransformer &transformer,
	                                                          const optional<bool> &lateral,
	                                                          const QualifiedName &qualified_table_function,
	                                                          vector<FunctionArgument> table_function_arguments,
	                                                          const optional<bool> &with_ordinality);
	static unique_ptr<TableRef> TransformNearestTableSubquery(PEGTransformer &transformer,
	                                                          const optional<bool> &lateral,
	                                                          unique_ptr<TableRef> subquery_reference);
	static unique_ptr<TableRef> TransformNearestBaseTableRef(PEGTransformer &transformer,
	                                                         unique_ptr<BaseTableRef> base_table_name,
	                                                         optional<unique_ptr<AtClause>> at_clause,
	                                                         optional<unique_ptr<SampleOptions>> sample_clause);
	static unique_ptr<TableRef> TransformNearestParensTableRef(PEGTransformer &transformer,
	                                                           unique_ptr<TableRef> table_ref,
	                                                           optional<unique_ptr<SampleOptions>> sample_clause);
	static bool TransformNearestApprox(PEGTransformer &transformer);
	static bool TransformNearestExact(PEGTransformer &transformer);
	static OrderType TransformNearestDistance(PEGTransformer &transformer);
	static OrderType TransformNearestSimilarity(PEGTransformer &transformer);
	static unique_ptr<TableRef> TransformRegularJoinClause(PEGTransformer &transformer, const optional<bool> &asof,
	                                                       const optional<JoinType> &join_type,
	                                                       unique_ptr<TableRef> table_ref,
	                                                       JoinQualifier join_qualifier);
	static unique_ptr<TableRef> TransformJoinByClause(PEGTransformer &transformer, const string &col_label,
	                                                  unique_ptr<TableRef> table_ref, JoinQualifier join_qualifier);
	static bool TransformAsof(PEGTransformer &transformer);
	static unique_ptr<TableRef> TransformJoinWithoutOnClause(PEGTransformer &transformer, const JoinPrefix &join_prefix,
	                                                         unique_ptr<TableRef> inner_table_ref);
	static JoinQualifier TransformOnClause(PEGTransformer &transformer, unique_ptr<ParsedExpression> expression);
	static JoinQualifier TransformUsingClause(PEGTransformer &transformer, const vector<Identifier> &column_name);
	static JoinPrefix TransformCrossJoinPrefix(PEGTransformer &transformer);
	static JoinPrefix TransformNaturalJoinPrefix(PEGTransformer &transformer, const optional<JoinType> &join_type);
	static JoinPrefix TransformPositionalJoinPrefix(PEGTransformer &transformer);
	static JoinType TransformFullJoin(PEGTransformer &transformer, const bool &has_result);
	static JoinType TransformLeftJoin(PEGTransformer &transformer, const bool &has_result);
	static JoinType TransformRightJoin(PEGTransformer &transformer, const bool &has_result);
	static JoinType TransformSemiJoin(PEGTransformer &transformer);
	static JoinType TransformAntiJoin(PEGTransformer &transformer);
	static JoinType TransformInnerJoin(PEGTransformer &transformer);
	static unique_ptr<TableRef> TransformFromClause(PEGTransformer &transformer,
	                                                vector<unique_ptr<TableRef>> table_ref);
	static unique_ptr<ParsedExpression> TransformWhereClause(PEGTransformer &transformer,
	                                                         unique_ptr<ParsedExpression> expression);
	static GroupByNode TransformGroupByClause(PEGTransformer &transformer, GroupByNode group_by_expressions);
	static unique_ptr<ParsedExpression> TransformHavingClause(PEGTransformer &transformer,
	                                                          unique_ptr<ParsedExpression> expression);
	static unique_ptr<ParsedExpression> TransformQualifyClause(PEGTransformer &transformer,
	                                                           unique_ptr<ParsedExpression> expression);
	static unique_ptr<SampleOptions> TransformSampleClause(PEGTransformer &transformer,
	                                                       unique_ptr<SampleOptions> sample_entry);
	static vector<unique_ptr<ParsedExpression>>
	TransformWindowClause(PEGTransformer &transformer, vector<unique_ptr<ParsedExpression>> window_definition);
	static unique_ptr<SampleOptions>
	TransformSampleEntryCount(PEGTransformer &transformer, unique_ptr<SampleOptions> sample_count,
	                          const optional<pair<SampleMethod, optional_idx>> &sample_properties);
	static unique_ptr<SampleOptions> TransformSampleEntryFunction(PEGTransformer &transformer,
	                                                              const optional<SampleMethod> &sample_function,
	                                                              unique_ptr<SampleOptions> sample_count,
	                                                              const optional<optional_idx> &repeatable_sample);
	static SampleMethod TransformSampleFunction(PEGTransformer &transformer, const Identifier &col_id);
	static pair<SampleMethod, optional_idx> TransformSampleProperties(PEGTransformer &transformer,
	                                                                  const Identifier &col_id,
	                                                                  const optional<optional_idx> &sample_seed);
	static optional_idx TransformRepeatableSample(PEGTransformer &transformer, const optional_idx &sample_seed);
	static optional_idx TransformSampleSeed(PEGTransformer &transformer, unique_ptr<ParsedExpression> number_literal);
	static unique_ptr<SampleOptions> TransformSampleCount(PEGTransformer &transformer,
	                                                      unique_ptr<ParsedExpression> sample_value,
	                                                      const optional<bool> &sample_unit);
	static bool TransformSamplePercentage(PEGTransformer &transformer);
	static bool TransformSampleRows(PEGTransformer &transformer);
	static GroupByNode TransformGroupByAll(PEGTransformer &transformer);
	static GroupByNode TransformGroupByList(PEGTransformer &transformer,
	                                        vector<GroupByExpressionInfo> group_by_expression);
	static GroupByExpressionInfo TransformGroupByBaseExpression(PEGTransformer &transformer,
	                                                            unique_ptr<ParsedExpression> expression);
	static GroupByExpressionInfo TransformEmptyGroupingItem(PEGTransformer &transformer);
	static GroupByExpressionInfo TransformCubeOrRollupClause(PEGTransformer &transformer, const string &cube_or_rollup,
	                                                         optional<vector<unique_ptr<ParsedExpression>>> expression);
	static string TransformCubeKeyword(PEGTransformer &transformer);
	static string TransformRollupKeyword(PEGTransformer &transformer);
	static GroupByExpressionInfo TransformGroupingSetsClause(PEGTransformer &transformer,
	                                                         vector<GroupByExpressionInfo> group_by_expression);
	static unique_ptr<TableRef> TransformSubqueryReference(PEGTransformer &transformer,
	                                                       unique_ptr<SelectStatement> select_statement_internal);
	static OrderByNode TransformOrderByExpression(PEGTransformer &transformer, unique_ptr<ParsedExpression> expression,
	                                              const optional<OrderType> &desc_or_asc,
	                                              const optional<OrderByNullType> &nulls_first_or_last);
	static OrderType TransformDescendingOrder(PEGTransformer &transformer);
	static OrderType TransformAscendingOrder(PEGTransformer &transformer);
	static OrderByNullType TransformNullsFirst(PEGTransformer &transformer);
	static OrderByNullType TransformNullsLast(PEGTransformer &transformer);
	static vector<OrderByNode> TransformOrderByClause(PEGTransformer &transformer,
	                                                  vector<OrderByNode> order_by_expressions);
	static vector<OrderByNode> TransformOrderByExpressionList(PEGTransformer &transformer,
	                                                          vector<OrderByNode> order_by_expression);
	static vector<OrderByNode> TransformOrderByAll(PEGTransformer &transformer, const optional<OrderType> &desc_or_asc,
	                                               const optional<OrderByNullType> &nulls_first_or_last);
	static LimitPercentResult TransformLimitClause(PEGTransformer &transformer, LimitPercentResult limit_value);
	static LimitPercentResult TransformOffsetClause(PEGTransformer &transformer, LimitPercentResult offset_value);
	static LimitPercentResult TransformOffsetValue(PEGTransformer &transformer, unique_ptr<ParsedExpression> expression,
	                                               const bool &has_result);
	static LimitPercentResult TransformLimitAll(PEGTransformer &transformer);
	static LimitPercentResult TransformLimitLiteralPercent(PEGTransformer &transformer,
	                                                       unique_ptr<ParsedExpression> number_literal);
	static LimitPercentResult TransformLimitExpression(PEGTransformer &transformer,
	                                                   unique_ptr<ParsedExpression> expression, const bool &has_result);
	static LimitPercentResult TransformFetchValue(PEGTransformer &transformer, unique_ptr<ParsedExpression> expression);
	static unique_ptr<ParsedExpression> TransformColIdExpression(PEGTransformer &transformer, const Identifier &col_id,
	                                                             unique_ptr<ParsedExpression> expression);
	static unique_ptr<ParsedExpression> TransformExpressionAsCollabel(PEGTransformer &transformer,
	                                                                  unique_ptr<ParsedExpression> expression,
	                                                                  const Identifier &col_label_or_string);
	static unique_ptr<ParsedExpression> TransformExpressionOptIdentifier(PEGTransformer &transformer,
	                                                                     unique_ptr<ParsedExpression> expression,
	                                                                     const optional<Identifier> &identifier);
	static unique_ptr<SelectStatement>
	TransformValuesClause(PEGTransformer &transformer, vector<vector<unique_ptr<ParsedExpression>>> values_expressions);
	static vector<unique_ptr<ParsedExpression>>
	TransformValuesExpressions(PEGTransformer &transformer, vector<unique_ptr<ParsedExpression>> expression);
	static unique_ptr<SQLStatement> TransformSetStatement(PEGTransformer &transformer,
	                                                      unique_ptr<SetStatement> set_assignment_or_time_zone);
	static unique_ptr<SQLStatement> TransformResetStatement(PEGTransformer &transformer,
	                                                        const SettingInfo &set_variable_or_setting);
	static unique_ptr<SetStatement> TransformSetSchema(PEGTransformer &transformer, const string &string_literal);
	static unique_ptr<SetStatement> TransformStandardAssignment(PEGTransformer &transformer,
	                                                            const SettingInfo &set_variable_or_setting,
	                                                            vector<unique_ptr<ParsedExpression>> set_assignment);
	static unique_ptr<SetStatement> TransformSetTimeZone(PEGTransformer &transformer,
	                                                     unique_ptr<ParsedExpression> zone_value);
	static unique_ptr<ParsedExpression> TransformZoneLocal(PEGTransformer &transformer);
	static unique_ptr<ParsedExpression> TransformZoneDefault(PEGTransformer &transformer);
	static unique_ptr<ParsedExpression> TransformZoneStringLiteral(PEGTransformer &transformer,
	                                                               const string &string_literal);
	static unique_ptr<ParsedExpression> TransformZoneIdentifier(PEGTransformer &transformer,
	                                                            const Identifier &identifier);
	static unique_ptr<ParsedExpression> TransformZoneIntervalWithInterval(PEGTransformer &transformer,
	                                                                      const string &string_literal,
	                                                                      const optional<DatePartSpecifier> &interval);
	static unique_ptr<ParsedExpression> TransformZoneIntervalWithPrecision(PEGTransformer &transformer,
	                                                                       unique_ptr<ParsedExpression> number_literal,
	                                                                       const string &string_literal);
	static SettingInfo TransformSetSetting(PEGTransformer &transformer, const optional<SetScope> &setting_scope,
	                                       const Identifier &setting_name);
	static SettingInfo TransformSetVariable(PEGTransformer &transformer, const SetScope &variable_scope,
	                                        const Identifier &identifier);
	static SetScope TransformVariableScope(PEGTransformer &transformer);
	static SetScope TransformLocalScope(PEGTransformer &transformer);
	static SetScope TransformSessionScope(PEGTransformer &transformer);
	static SetScope TransformGlobalScope(PEGTransformer &transformer);
	static vector<unique_ptr<ParsedExpression>>
	TransformSetAssignment(PEGTransformer &transformer, vector<unique_ptr<ParsedExpression>> variable_list);
	static vector<unique_ptr<ParsedExpression>> TransformVariableList(PEGTransformer &transformer,
	                                                                  vector<unique_ptr<ParsedExpression>> expression);
	static unique_ptr<SQLStatement> TransformBeginTransaction(PEGTransformer &transformer, const bool &has_result,
	                                                          const optional<TransactionModifierType> &read_or_write);
	static unique_ptr<SQLStatement> TransformRollbackTransaction(PEGTransformer &transformer, const bool &has_result);
	static unique_ptr<SQLStatement> TransformCommitTransaction(PEGTransformer &transformer, const bool &has_result);
	static TransactionModifierType TransformReadOrWrite(PEGTransformer &transformer,
	                                                    const TransactionModifierType &read_only_or_read_write);
	static TransactionModifierType TransformReadOnly(PEGTransformer &transformer);
	static TransactionModifierType TransformReadWrite(PEGTransformer &transformer);
	static unique_ptr<SQLStatement>
	TransformUpdateStatement(PEGTransformer &transformer, optional<CommonTableExpressionMap> with_clause,
	                         unique_ptr<TableRef> update_target, unique_ptr<UpdateSetInfo> update_set_clause,
	                         optional<unique_ptr<TableRef>> from_clause,
	                         optional<unique_ptr<ParsedExpression>> where_clause,
	                         optional<vector<unique_ptr<ParsedExpression>>> returning_clause);
	static unique_ptr<TableRef> TransformBaseTableSet(PEGTransformer &transformer,
	                                                  unique_ptr<BaseTableRef> base_table_name);
	static unique_ptr<TableRef> TransformBaseTableAliasSet(PEGTransformer &transformer,
	                                                       unique_ptr<BaseTableRef> base_table_name,
	                                                       const optional<Identifier> &update_alias);
	static Identifier TransformUpdateAlias(PEGTransformer &transformer, const bool &has_result,
	                                       const Identifier &col_id);
	static unique_ptr<UpdateSetInfo> TransformUpdateSetTuple(PEGTransformer &transformer,
	                                                         const vector<Identifier> &column_name,
	                                                         unique_ptr<ParsedExpression> expression);
	static unique_ptr<UpdateSetInfo>
	TransformUpdateSetElementList(PEGTransformer &transformer,
	                              vector<pair<string, unique_ptr<ParsedExpression>>> update_set_element);
	static pair<string, unique_ptr<ParsedExpression>>
	TransformUpdateSetElement(PEGTransformer &transformer, const string &update_set_column_target,
	                          unique_ptr<ParsedExpression> expression);
	static string TransformUpdateSetColumnTarget(PEGTransformer &transformer, const Identifier &column_name,
	                                             const optional<vector<Identifier>> &dot_identifier);
	static unique_ptr<SQLStatement> TransformUseStatement(PEGTransformer &transformer, const QualifiedName &use_target);
	static QualifiedName TransformSchemaNameAsUseTarget(PEGTransformer &transformer, const Identifier &schema_name);
	static QualifiedName TransformCatalogNameAsUseTarget(PEGTransformer &transformer, const Identifier &catalog_name);
	static QualifiedName TransformUseTargetCatalogSchema(PEGTransformer &transformer, const Identifier &catalog_name,
	                                                     const Identifier &reserved_schema_name,
	                                                     const optional<vector<Identifier>> &dot_identifier);
	static Identifier TransformDotIdentifier(PEGTransformer &transformer, const Identifier &identifier);
	static unique_ptr<SQLStatement> TransformVacuumStatement(PEGTransformer &transformer,
	                                                         const optional<VacuumOptions> &vacuum_options,
	                                                         optional<AnalyzeTarget> analyze_target);
	static VacuumOptions TransformVacuumParensOptions(PEGTransformer &transformer, const vector<string> &vacuum_option);
	static VacuumOptions TransformVacuumLegacyOptions(PEGTransformer &transformer, const optional<string> &opt_full,
	                                                  const optional<string> &opt_freeze,
	                                                  const optional<string> &opt_verbose,
	                                                  const optional<string> &opt_analyze);
	static string TransformOptAnalyze(PEGTransformer &transformer, const Identifier &analyze_keyword);
	static string TransformOptFull(PEGTransformer &transformer);
	static string TransformOptFreeze(PEGTransformer &transformer);
	static string TransformOptVerbose(PEGTransformer &transformer);
	static vector<string> TransformNameList(PEGTransformer &transformer, const vector<Identifier> &col_id);
	//===--------------------------------------------------------------------===//
	// END GENERATED TRAMPOLINE RULES
	//===--------------------------------------------------------------------===//

	PEGTransformerFactory(const PEGTransformerFactory &) = delete;

	static const case_insensitive_map_t<const TransformFrameOps *> &GeneratedTransformFrameOps();

	static string ExtractFormat(const string &file_path);

	static string TransformIdentifierOrKeyword(PEGTransformer &transformer, ParseResult &parse_result);

private:
	explicit PEGTransformerFactory(ParsedGrammar &grammar_p);
	ParsedGrammar &grammar;
};

} // namespace duckdb
