#include "duckdb/parser/peg/matcher_factory.hpp"
#include "duckdb/parser/peg/peg_parser.hpp"
#include "duckdb/parser/peg/matcher/list.hpp"
#include "duckdb/parser/peg/compiled_grammar.hpp"
#include "duckdb/parser/peg/matcher/literal_choice_matcher.hpp"
#include "duckdb/parser/peg/matcher/optional_matcher.hpp"
#include "duckdb/parser/peg/matcher/precedence_ladder.hpp"
#include "duckdb/parser/peg/matcher/repeat_matcher.hpp"

namespace duckdb {

class CompiledKeywordMatcher final : public KeywordMatcher {
public:
	CompiledKeywordMatcher(const string &keyword, const KeywordInfo &info, const PEGKeywordHelper &helper)
	    : KeywordMatcher(keyword, info, helper) {
	}

	optional_idx GetDispatchLiteral(const GrammarLiteralTable &table) const override {
		if (literal_table.get() != &table || !literal_info.LiteralId()) {
			return optional_idx();
		}
		return optional_idx(literal_info.LiteralId());
	}
};

namespace {

//! A collapsible rule of the form `X <- Y`, `X <- Y Tail*`, `X <- Y Tail?` or `X <- Prefix? Y` is a link of the
//! operator precedence ladder: its own contribution is a single optional affix and it forwards its operand
//! otherwise. Returns false when the matcher has another shape, which ends the chain.
bool DescribeLadderLevel(ListMatcher &matcher, PrecedenceLevel &level) {
	if (!matcher.IsCollapsible() || !matcher.GetRule()) {
		return false;
	}
	auto &children = matcher.matchers;
	if (children.size() == 1) {
		// a rule whose body is one ordered choice is matched by the choice itself, which builds the rule's result;
		// it forwards a chosen alternative rather than an operand, so it is not a level of the ladder
		if (children[0].get().Type() == MatcherType::CHOICE) {
			return false;
		}
		level.shape = PrecedenceShape::ALIAS;
		level.operand = children[0].get();
		return true;
	}
	if (children.size() != 2) {
		return false;
	}
	auto &first = children[0].get();
	auto &second = children[1].get();
	if (first.Type() != MatcherType::OPTIONAL && second.Type() == MatcherType::OPTIONAL) {
		auto &affix = second.Cast<OptionalMatcher>().GetChildMatcher();
		// `Tail*` is an optional around a repeat, `Tail?` an optional around the tail itself
		if (affix.Type() == MatcherType::REPEAT) {
			level.shape = PrecedenceShape::SUFFIX_REPEAT;
			level.affix = affix.Cast<RepeatMatcher>().GetChildMatcher();
		} else {
			level.shape = PrecedenceShape::SUFFIX_OPTIONAL;
			level.affix = affix;
		}
		level.operand = first;
		return true;
	}
	if (first.Type() == MatcherType::OPTIONAL && second.Type() != MatcherType::OPTIONAL) {
		level.shape = PrecedenceShape::PREFIX_OPTIONAL;
		level.affix = first.Cast<OptionalMatcher>().GetChildMatcher();
		level.operand = second;
		return true;
	}
	return false;
}

} // namespace

void MatcherFactory::MatcherConstructionState::Register(string_t rule_name) {
	unconstructed.insert(rule_name);
}

void MatcherFactory::MatcherConstructionState::Schedule(string_t rule_name) {
	if (unconstructed.count(rule_name) && scheduled.insert(rule_name).second) {
		pending.push(rule_name);
	}
}

bool MatcherFactory::MatcherConstructionState::Begin(string_t rule_name) {
	return unconstructed.erase(rule_name);
}

bool MatcherFactory::MatcherConstructionState::HasScheduled() const {
	return !pending.empty();
}

string_t MatcherFactory::MatcherConstructionState::TakeNext() {
	auto rule_name = pending.front();
	pending.pop();
	scheduled.erase(rule_name);
	return rule_name;
}

Matcher &MatcherFactory::CreateMatcher(const PEGExpression &expression, const string_map_t<idx_t> &parameter_map,
                                       vector<reference<Matcher>> &parameters) {
	switch (expression.type) {
	case PEGExpression::Type::LITERAL:
		return Keyword(expression.text.GetString());
	case PEGExpression::Type::REFERENCE: {
		auto parameter = parameter_map.find(expression.text);
		if (parameter != parameter_map.end()) {
			return parameters[parameter->second].get();
		}
		auto matcher = matchers.find(expression.text);
		if (matcher != matchers.end()) {
			construction_state.Schedule(expression.text);
			return matcher->second.get();
		}
		return CreateMatcher(expression.text);
	}
	case PEGExpression::Type::FUNCTION_CALL: {
		if (expression.children.size() != 1) {
			throw InternalException("Function call '%s' expected a single argument", expression.text.GetString());
		}
		vector<reference<Matcher>> function_parameters;
		function_parameters.push_back(CreateMatcher(expression.children[0], parameter_map, parameters));
		return CreateMatcher(expression.text, function_parameters);
	}
	case PEGExpression::Type::SEQUENCE: {
		vector<reference<Matcher>> children;
		for (auto &child : expression.children) {
			children.push_back(CreateMatcher(child, parameter_map, parameters));
		}
		return List(std::move(children));
	}
	case PEGExpression::Type::CHOICE: {
		vector<reference<Matcher>> children;
		for (auto &child : expression.children) {
			children.push_back(CreateMatcher(child, parameter_map, parameters));
		}
		return Choice(std::move(children));
	}
	case PEGExpression::Type::OPTIONAL:
	case PEGExpression::Type::REPEAT:
	case PEGExpression::Type::OPTIONAL_REPEAT: {
		if (expression.children.size() != 1) {
			throw InternalException("PEG postfix expression expected a single child");
		}
		auto &child = CreateMatcher(expression.children[0], parameter_map, parameters);
		if (expression.type == PEGExpression::Type::OPTIONAL) {
			return Optional(child);
		}
		auto &repeat = Repeat(child);
		if (expression.type == PEGExpression::Type::OPTIONAL_REPEAT) {
			return Optional(repeat);
		}
		return repeat;
	}
	case PEGExpression::Type::REGEX:
		throw InternalException("REGEX operator not supported in PEG grammar");
	default:
		throw InternalException("Unrecognized PEG expression type");
	}
}

optional_ptr<const CompiledGrammarRule> MatcherFactory::GetRule(const string &rule_name) const {
	auto entry = rules.find(rule_name);
	if (entry == rules.end()) {
		return nullptr;
	}
	return *entry->second;
}

Matcher &MatcherFactory::CreateMatcher(string_t rule_name, vector<reference<Matcher>> &parameters) {
	bool is_function_call = !parameters.empty();
	auto matcher_entry = matchers.find(rule_name);
	if (!is_function_call) {
		if (matcher_entry == matchers.end()) {
			throw InvalidConfigurationException("Recipe references rule %s, which doesn't exist in the grammar",
			                                    rule_name.GetString());
		}
		if (!construction_state.Begin(rule_name)) {
			//! Already constructed, return the cached matcher
			return matcher_entry->second.get();
		}
	} else {
		matcher_entry = matchers.end();
	}
	// Named matchers are registered before any bodies are constructed so recursive references can resolve immediately.
	auto &matcher = is_function_call ? List() : matcher_entry->second.get().Cast<ListMatcher>();

	// fill the matcher from the given set of rules
	// look up the rule
	auto entry = grammar.rules.find(rule_name.GetString());
	if (entry == grammar.rules.end()) {
		throw InvalidConfigurationException("Failed to create matcher for rule %s - rule is missing",
		                                    rule_name.GetString());
	}
	auto &rule = entry->second->recipe;
	if (rule.parameters.size() > 1) {
		throw InvalidConfigurationException("Only functions with a single parameter are supported");
	}
	if (parameters.size() != rule.parameters.size()) {
		throw InvalidConfigurationException("Parameter count mismatch (rule %s expected %d parameters but got %d)",
		                                    rule_name.GetString(), rule.parameters.size(), parameters.size());
	}
	auto &expression_matcher = CreateMatcher(rule.expression, rule.parameters, parameters);
	if (rule.expression.type == PEGExpression::Type::SEQUENCE) {
		matcher.matchers = std::move(expression_matcher.Cast<ListMatcher>().matchers);
	} else {
		matcher.matchers.push_back(expression_matcher);
	}

	auto rule_name_str = rule_name.GetString();
	auto rule_p = GetRule(rule_name_str);
	if (!rule_p) {
		throw InvalidInputException("Failed to compile rule '%s', no registered data exists for it", rule_name_str);
	}
	auto &compiled_rule = *rule_p;

	matcher.SetRule(compiled_rule);
	if (packrat_memoized_rules.count(rule_name)) {
		matcher.SetPackratMemoized();
	}
	if (collapsible_rules.count(rule_name)) {
		matcher.SetCollapsible();
	}
	if (no_suggestion_rules.count(rule_name)) {
		matcher.Cast<ListMatcher>().suppress_suggestions = true;
	}
	return matcher;
}

void MatcherFactory::AddKeywordOverride(const char *name, KeywordInfo info) {
	keyword_overrides.insert(make_pair(name, info));
}

void MatcherFactory::AddRuleOverride(const char *name, unique_ptr<Matcher> &&matcher_p) {
	auto &matcher = allocator.Allocate(std::move(matcher_p));
	if (packrat_memoized_rules.count(name)) {
		matcher.SetPackratMemoized();
	}
	if (grammar.GetRule(name)) {
		auto rule_p = GetRule(name);
		if (!rule_p) {
			throw InvalidInputException("No registered data exists for rule '%s', failed to set RuleOverride", name);
		}
		auto &rule = *rule_p;
		matcher.SetRule(rule);
	}
	matchers.emplace(name, reference<Matcher>(matcher));
}

void MatcherFactory::AddPackratMemoizedRule(const char *name) {
	packrat_memoized_rules.insert(name);
}

void MatcherFactory::AddCollapsibleRule(const char *name) {
	collapsible_rules.insert(name);
}

void MatcherFactory::SuppressSuggestions(const char *name) {
	no_suggestion_rules.insert(name);
}

MatcherFactory::MatcherFactory(MatcherAllocator &allocator, const ParsedGrammar &grammar_p,
                               const compiled_rules_map_t &rules, const PEGKeywordHelper &keyword_helper_p,
                               terminal_rule_overrides_t terminal_rule_overrides_p)
    : allocator(allocator), grammar(grammar_p), rules(rules), keyword_helper(keyword_helper_p),
      terminal_rule_overrides(std::move(terminal_rule_overrides_p)) {
}

//! Index the levels by the tokens their affix can start with, so that walking the ladder outwards costs one lookup
//! instead of a start set probe per level.
static void BuildLadderLevelMasks(PrecedenceLadder &ladder) {
	for (idx_t level = 0; level < ladder.levels.size(); level++) {
		auto &entry = ladder.levels[level];
		if (!entry.affix) {
			continue;
		}
		if (entry.IsPrefix()) {
			ladder.prefix_levels.push_back(level);
			continue;
		}
		auto level_bit = uint32_t(1) << level;
		auto start_set = entry.affix->GetStartSet();
		if (!start_set || start_set->any || !start_set->predicate_leaders.empty()) {
			ladder.predicate_levels |= level_bit;
		}
		if (!start_set) {
			continue;
		}
		if (start_set->literal_table) {
			ladder.literal_table = start_set->literal_table;
		}
		for (auto literal_id : start_set->literal_ids) {
			if (literal_id >= ladder.literal_levels.size()) {
				ladder.literal_levels.resize(literal_id + 1, 0);
			}
			ladder.literal_levels[literal_id] |= level_bit;
		}
	}
}

void MatcherFactory::IndexStartSets() {
	for (auto &ladder : ladders) {
		BuildLadderLevelMasks(ladder.get());
	}
}

void MatcherFactory::FuseSingleChoiceRules() {
	for (auto &entry : matchers) {
		auto &matcher = entry.second.get();
		if (matcher.Type() != MatcherType::LIST) {
			continue;
		}
		auto &list = matcher.Cast<ListMatcher>();
		// a rule that is only an ordered choice does not need a frame of its own to wrap the choice's result
		if (list.suppress_suggestions || list.GetLadder() || list.matchers.size() != 1) {
			continue;
		}
		auto &child = list.matchers[0].get();
		if (child.Type() != MatcherType::CHOICE) {
			continue;
		}
		list.SetFusedChoice(child.Cast<ChoiceMatcher>());
	}
}

void MatcherAllocator::FuseRepeatElements() {
	for (auto &entry : matchers) {
		auto &matcher = *entry;
		if (matcher.Type() != MatcherType::REPEAT) {
			continue;
		}
		auto &repeat = matcher.Cast<RepeatMatcher>();
		auto &element = repeat.GetChildMatcher();
		if (element.Type() != MatcherType::LIST) {
			continue;
		}
		auto &list = element.Cast<ListMatcher>();
		// `(Atom X)` only: the repeat matches the atom itself and asks for X, so the element needs no frame. Anything
		// the element's own frame would have done besides that - a rule, a name, a ladder, memoization, hidden
		// suggestions - keeps it.
		if (list.matchers.size() != 2 || list.GetRule() || list.HasName() || list.GetLadder() ||
		    list.GetFusedChoice() || list.suppress_suggestions || list.IsPackratMemoized() || list.IsCollapsible()) {
			continue;
		}
		auto &lead = list.matchers[0].get();
		if (!lead.IsAtomic() || lead.IsPackratMemoized()) {
			continue;
		}
		repeat.SetFusedElement(list);
	}
}

void MatcherFactory::BuildPrecedenceLadder(const string &root_rule) {
	auto entry = matchers.find(root_rule);
	if (entry == matchers.end() || entry->second.get().Type() != MatcherType::LIST) {
		return;
	}
	auto ladder = make_uniq<PrecedenceLadder>();
	vector<reference<ListMatcher>> chain;
	auto current = optional_ptr<ListMatcher>(&entry->second.get().Cast<ListMatcher>());
	while (current) {
		PrecedenceLevel level;
		if (!DescribeLadderLevel(*current, level)) {
			break;
		}
		level.rule = current->GetRule();
		ladder->levels.push_back(level);
		chain.push_back(*current);
		auto &operand = *level.operand;
		current = operand.Type() == MatcherType::LIST ? optional_ptr<ListMatcher>(&operand.Cast<ListMatcher>())
		                                              : optional_ptr<ListMatcher>();
	}
	if (ladder->levels.size() < 2 || ladder->levels.size() > PrecedenceLadder::MAX_LEVELS) {
		return;
	}
	ladder->leaf = ladder->levels.back().operand;

	auto &stored = allocator.AddLadder(std::move(ladder));
	ladders.push_back(stored);
	for (idx_t level = 0; level < chain.size(); level++) {
		chain[level].get().SetPrecedenceLevel(stored, level);
	}
}

Matcher &MatcherFactory::CreateRootMatcher(const string &root_rule) {
	// keyword overrides
	AddKeywordOverride("TABLE", KeywordInfo(1, ' '));
	AddKeywordOverride(".", KeywordInfo(0, '\0'));
	AddKeywordOverride("(", KeywordInfo(0, '\0'));
	// matcher rule flags, from packrat_memoized_rules and collapsible_rules in scripts/parser/grammar_types.yml
	//===--------------------------------------------------------------------===//
	// START GENERATED MATCHER RULE FLAGS
	//===--------------------------------------------------------------------===//
	// Rules whose transformer returns their operand unchanged when the rest of the rule
	// matched nothing; the matcher hands out that operand's parse result instead of its own
	AddCollapsibleRule("Expression");
	AddCollapsibleRule("LambdaArrowExpression");
	AddCollapsibleRule("LogicalOrExpression");
	AddCollapsibleRule("LogicalAndExpression");
	AddCollapsibleRule("LogicalNotExpression");
	AddCollapsibleRule("IsExpression");
	AddCollapsibleRule("IsDistinctFromExpression");
	AddCollapsibleRule("ComparisonExpression");
	AddCollapsibleRule("BetweenInLikeExpression");
	AddCollapsibleRule("OtherOperatorExpression");
	AddCollapsibleRule("BitwiseExpression");
	AddCollapsibleRule("AdditiveExpression");
	AddCollapsibleRule("MultiplicativeExpression");
	AddCollapsibleRule("ExponentiationExpression");
	AddCollapsibleRule("CollateExpression");
	AddCollapsibleRule("AtTimeZoneExpression");
	AddCollapsibleRule("PrefixExpression");
	AddCollapsibleRule("BaseExpression");
	AddCollapsibleRule("AlterOptions");
	AddCollapsibleRule("AlterTableOptions");
	AddCollapsibleRule("AlterColumnEntry");
	AddCollapsibleRule("AddOrDropDefault");
	AddCollapsibleRule("AlterSequenceOptions");
	AddCollapsibleRule("ExpressionAlias");
	AddCollapsibleRule("TypeVariations");
	AddCollapsibleRule("SimpleType");
	AddCollapsibleRule("IntervalType");
	AddCollapsibleRule("IntervalInterval");
	AddCollapsibleRule("IntervalWithSpecifier");
	AddCollapsibleRule("NumericType");
	AddCollapsibleRule("DecimalNumericType");
	AddCollapsibleRule("SessionTarget");
	AddCollapsibleRule("CopyVariations");
	AddCollapsibleRule("CopyFileName");
	AddCollapsibleRule("CopyFileNameExpression");
	AddCollapsibleRule("GenericCopyOptionValue");
	AddCollapsibleRule("CopyFromDatabase");
	AddCollapsibleRule("RelOptionOrOids");
	AddCollapsibleRule("DefArg");
	AddCollapsibleRule("MacroDefinitionBody");
	AddCollapsibleRule("MacroParameter");
	AddCollapsibleRule("SequenceOption");
	AddCollapsibleRule("SeqSetCycle");
	AddCollapsibleRule("CreateStatementVariation");
	AddCollapsibleRule("CreateTableDefinition");
	AddCollapsibleRule("PartitionSortedOptions");
	AddCollapsibleRule("CreateTableColumnElement");
	AddCollapsibleRule("ColumnConstraint");
	AddCollapsibleRule("TopLevelConstraintList");
	AddCollapsibleRule("TriggerBody");
	AddCollapsibleRule("CreateType");
	AddCollapsibleRule("DescribeTarget");
	AddCollapsibleRule("DropEntries");
	AddCollapsibleRule("ExplainableStatements");
	AddCollapsibleRule("ReplaceEntries");
	AddCollapsibleRule("IntervalParameter");
	AddCollapsibleRule("FrameExtent");
	AddCollapsibleRule("FrameBound");
	AddCollapsibleRule("WindowFrame");
	AddCollapsibleRule("WindowFrameDefinition");
	AddCollapsibleRule("ListExpression");
	AddCollapsibleRule("Parameter");
	AddCollapsibleRule("SingleExpression");
	AddCollapsibleRule("IsTest");
	AddCollapsibleRule("NotNull");
	AddCollapsibleRule("BetweenInLikeOpExpression");
	AddCollapsibleRule("InExpression");
	AddCollapsibleRule("OtherOperator");
	AddCollapsibleRule("Indirection");
	AddCollapsibleRule("DotOperator");
	AddCollapsibleRule("EndSliceValue");
	AddCollapsibleRule("SpecialFunctionExpression");
	AddCollapsibleRule("SubstringArguments");
	AddCollapsibleRule("SubstringFromFor");
	AddCollapsibleRule("OverlayArguments");
	AddCollapsibleRule("ExtractArgument");
	AddCollapsibleRule("ExternalResourceStatement");
	AddCollapsibleRule("ExternalResourceSource");
	AddCollapsibleRule("InsertValues");
	AddCollapsibleRule("OnConflictTarget");
	AddCollapsibleRule("OnConflictAction");
	AddCollapsibleRule("ExtensionRepositoryStatement");
	AddCollapsibleRule("MergeMatch");
	AddCollapsibleRule("MatchedClauseAction");
	AddCollapsibleRule("UpdateMatchInfo");
	AddCollapsibleRule("InsertMatchInfo");
	AddCollapsibleRule("UpdateMatchSetClause");
	AddCollapsibleRule("PivotColumnEntry");
	AddCollapsibleRule("PragmaAssignOrFunction");
	AddCollapsibleRule("SelectAtom");
	AddCollapsibleRule("SelectStatementType");
	AddCollapsibleRule("LimitOffset");
	AddCollapsibleRule("OptionalParensSimpleSelect");
	AddCollapsibleRule("SelectFrom");
	AddCollapsibleRule("CTEBody");
	AddCollapsibleRule("DistinctClause");
	AddCollapsibleRule("InnerTableRef");
	AddCollapsibleRule("JoinOrPivot");
	AddCollapsibleRule("PivotValueTarget");
	AddCollapsibleRule("BaseTableName");
	AddCollapsibleRule("QualifiedTableName");
	AddCollapsibleRule("TableFunction");
	AddCollapsibleRule("FunctionArgument");
	AddCollapsibleRule("JoinClause");
	AddCollapsibleRule("NearestJoinClause");
	AddCollapsibleRule("NearestBareTableRef");
	AddCollapsibleRule("JoinQualifier");
	AddCollapsibleRule("SampleEntry");
	AddCollapsibleRule("SampleValue");
	AddCollapsibleRule("GroupByExpressions");
	AddCollapsibleRule("GroupByExpression");
	AddCollapsibleRule("OrderByExpressions");
	AddCollapsibleRule("LimitValue");
	AddCollapsibleRule("FetchClause");
	AddCollapsibleRule("AliasedExpression");
	AddCollapsibleRule("SetAssignmentOrTimeZone");
	AddCollapsibleRule("ZoneValue");
	AddCollapsibleRule("TransactionStatement");
	AddCollapsibleRule("UpdateTarget");
	AddCollapsibleRule("UpdateSetClause");

	AddPackratMemoizedRule("Expression");
	AddPackratMemoizedRule("LambdaArrowExpression");
	AddPackratMemoizedRule("LogicalOrExpression");
	AddPackratMemoizedRule("LogicalAndExpression");
	AddPackratMemoizedRule("LogicalNotExpression");
	AddPackratMemoizedRule("IsExpression");
	AddPackratMemoizedRule("ComparisonExpression");
	AddPackratMemoizedRule("BitwiseExpression");
	AddPackratMemoizedRule("AdditiveExpression");
	AddPackratMemoizedRule("MultiplicativeExpression");
	AddPackratMemoizedRule("ExponentiationExpression");
	AddPackratMemoizedRule("PrefixExpression");
	AddPackratMemoizedRule("CollateExpression");
	AddPackratMemoizedRule("AtTimeZoneExpression");
	AddPackratMemoizedRule("SingleExpression");
	AddPackratMemoizedRule("BaseExpression");
	AddPackratMemoizedRule("ParensExpression");
	AddPackratMemoizedRule("ParenthesisExpression");
	AddPackratMemoizedRule("Identifier");
	AddPackratMemoizedRule("ColId");
	AddPackratMemoizedRule("ColumnReference");
	AddPackratMemoizedRule("FunctionExpression");
	//===--------------------------------------------------------------------===//
	// END GENERATED MATCHER RULE FLAGS
	//===--------------------------------------------------------------------===//

	for (auto &entry : terminal_rule_overrides) {
		AddRuleOverride(entry.first.c_str(), std::move(entry.second));
	}

	// suppress suggestions for catch-all rules that would pollute statement-level autocomplete
	SuppressSuggestions("ExpressionStatement");
	// Using SHOW to describe a table/query is deprecated - parse these forms but do not autocomplete them after SHOW
	// (only setting names and the special SHOW forms are offered). DESCRIBE/SUMMARIZE still complete tables and
	// queries.
	SuppressSuggestions("ShowDeprecatedQualifiedTableName");
	SuppressSuggestions("ShowDeprecatedSelect");

	// Register all named rules before constructing any children. Grammar changes can introduce cycles through
	// parameterized rules, so registering only the current recursive path is insufficient.
	for (auto &entry : grammar.rules) {
		if (!entry.second->recipe.parameters.empty()) {
			//! Parameterized rule, can't cache
			continue;
		}
		if (matchers.count(entry.first)) {
			//! Pre-made rule, doesn't get built by the matcher factory
			continue;
		}
		auto &matcher = List();
		auto rule_name = string_t(entry.second->name);
		matchers.emplace(rule_name, reference<Matcher>(matcher));
		construction_state.Register(rule_name);
	}

	// Populate the reachable rules without recursively constructing referenced bodies. The queue grows as references
	// are encountered and therefore also handles cycles that pass through parameterized rules.
	CreateMatcher(root_rule);
	while (construction_state.HasScheduled()) {
		CreateMatcher(construction_state.TakeNext());
	}
	BuildPrecedenceLadder("Expression");
	FuseSingleChoiceRules();
	allocator.FuseRepeatElements();
	return GetMatcher(root_rule);
}

unique_ptr<KeywordMatcher> MatcherFactory::CreateKeyword(const string &keyword, const KeywordInfo &info) const {
	return make_uniq<CompiledKeywordMatcher>(keyword, info, keyword_helper);
}

unique_ptr<ListMatcher> MatcherFactory::CreateList() const {
	return make_uniq<ListMatcher>();
}

unique_ptr<ChoiceMatcher> MatcherFactory::CreateChoice(vector<reference<Matcher>> &&matchers) const {
	auto &table = keyword_helper.GetLiteralTable();
	if (matchers.size() > 1) {
		unordered_map<uint32_t, idx_t> literal_children;
		for (idx_t i = 0; i < matchers.size(); i++) {
			auto &matcher = matchers[i].get();
			if (matcher.Type() != MatcherType::KEYWORD) {
				return make_uniq<ChoiceMatcher>(std::move(matchers));
			}
			auto literal = matcher.Cast<KeywordMatcher>().GetDispatchLiteral(table);
			if (!literal.IsValid()) {
				return make_uniq<ChoiceMatcher>(std::move(matchers));
			}
			// Preserve the first alternative when spellings share an ID.
			literal_children.emplace(static_cast<uint32_t>(literal.GetIndex()), i);
		}
		return make_uniq<LiteralChoiceMatcher>(std::move(matchers), table, std::move(literal_children));
	}
	return make_uniq<ChoiceMatcher>(std::move(matchers));
}

unique_ptr<OptionalMatcher> MatcherFactory::CreateOptional(Matcher &matcher) const {
	return make_uniq<OptionalMatcher>(matcher);
}

unique_ptr<RepeatMatcher> MatcherFactory::CreateRepeat(Matcher &matcher) const {
	return make_uniq<RepeatMatcher>(matcher);
}

KeywordMatcher &MatcherFactory::Keyword(const string &keyword) const {
	auto it = keywords.find(keyword);
	if (it != keywords.end()) {
		return it->second;
	}

	optional<KeywordInfo> info;
	auto entry = keyword_overrides.find(keyword);
	if (entry != keyword_overrides.end()) {
		info.emplace(entry->second);
	} else {
		info.emplace(0, ' ');
	}
	auto &result = allocator.Allocate(CreateKeyword(keyword, *info)).Cast<KeywordMatcher>();
	keywords.emplace(keyword, result);
	return result;
}

ListMatcher &MatcherFactory::List() const {
	return allocator.Allocate(CreateList()).Cast<ListMatcher>();
}

ListMatcher &MatcherFactory::List(vector<reference<Matcher>> matchers) const {
	auto result = CreateList();
	result->matchers = std::move(matchers);
	return allocator.Allocate(std::move(result)).Cast<ListMatcher>();
}

ChoiceMatcher &MatcherFactory::Choice(vector<reference<Matcher>> &&matchers) const {
	return allocator.Allocate(CreateChoice(std::move(matchers))).Cast<ChoiceMatcher>();
}

OptionalMatcher &MatcherFactory::Optional(Matcher &matcher) const {
	return allocator.Allocate(CreateOptional(matcher)).Cast<OptionalMatcher>();
}

RepeatMatcher &MatcherFactory::Repeat(Matcher &matcher) const {
	return allocator.Allocate(CreateRepeat(matcher)).Cast<RepeatMatcher>();
}

Matcher &MatcherFactory::GetMatcher(const string &rule_name) {
	auto entry = matchers.find(rule_name);
	if (entry == matchers.end()) {
		throw InternalException("Matcher for rule '%s' has not been built", rule_name);
	}
	return entry->second.get();
}

Matcher &MatcherFactory::CreateMatcher(string_t rule_name) {
	vector<reference<Matcher>> parameters;
	return CreateMatcher(rule_name, parameters);
}

} // namespace duckdb
