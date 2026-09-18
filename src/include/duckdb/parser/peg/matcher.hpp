//===----------------------------------------------------------------------===//
//                         DuckDB
//
// matcher.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/arena_containers/arena_ptr.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/common/identifier.hpp"
#include "duckdb/common/vector.hpp"
#include "duckdb/common/reference_map.hpp"
#include "duckdb/common/enums/identifier_case_mode.hpp"
#include "duckdb/parser/parser_extension.hpp"
#include "duckdb/parser/peg/keyword_helper.hpp"
#include "duckdb/parser/token_iterator.hpp"
#include "duckdb/parser/peg/parser_packrat.hpp"
#include "duckdb/parser/peg/tokenizer/tokenizer.hpp"
#include "duckdb/parser/peg/parsed_grammar.hpp"
#include "duckdb/parser/peg/transformer/parse_result.hpp"
#include "duckdb/storage/arena_allocator.hpp"

namespace duckdb {
class ClientContext;
class PEGTransformerFactory;
class ParseResultAllocator;
class Matcher;
class MatcherAllocator;
class MatchProcess;

enum class SuggestionState : uint8_t {
	SUGGEST_KEYWORD,
	SUGGEST_CATALOG_NAME,
	SUGGEST_SCHEMA_NAME,
	SUGGEST_TABLE_NAME,
	SUGGEST_TYPE_NAME,
	SUGGEST_COLUMN_NAME,
	SUGGEST_FILE_NAME,
	SUGGEST_DIRECTORY,
	SUGGEST_VARIABLE,
	SUGGEST_SCALAR_FUNCTION_NAME,
	SUGGEST_TABLE_FUNCTION_NAME,
	SUGGEST_PRAGMA_NAME,
	SUGGEST_SETTING_NAME,
	SUGGEST_RESERVED_VARIABLE
};

enum class CandidateType { KEYWORD, IDENTIFIER, LITERAL };

struct AutoCompleteCandidate {
	// NOLINTNEXTLINE: allow implicit conversion from string
	AutoCompleteCandidate(string candidate_p, SuggestionState suggestion_type, int32_t score_bonus = 0,
	                      CandidateType candidate_type = CandidateType::IDENTIFIER)
	    : candidate(std::move(candidate_p)), suggestion_type(suggestion_type), score_bonus(score_bonus),
	      candidate_type(candidate_type) {
	}
	// NOLINTNEXTLINE: allow implicit conversion from const char*
	AutoCompleteCandidate(const char *candidate_p, SuggestionState suggestion_type, int32_t score_bonus = 0,
	                      CandidateType candidate_type = CandidateType::IDENTIFIER)
	    : AutoCompleteCandidate(string(candidate_p), suggestion_type, score_bonus, candidate_type) {
	}
	// NOLINTNEXTLINE: allow implicit conversion from Identifier
	AutoCompleteCandidate(const Identifier &candidate_p, SuggestionState suggestion_type, int32_t score_bonus = 0,
	                      CandidateType candidate_type = CandidateType::IDENTIFIER)
	    : AutoCompleteCandidate(candidate_p.GetIdentifierName(), suggestion_type, score_bonus, candidate_type) {
	}

	string candidate;
	//! Type being suggested
	SuggestionState suggestion_type;
	//! The higher the score bonus, the more likely this candidate will be chosen
	int32_t score_bonus;
	//! The type of candidate we are suggesting - this modifies how we handle quoting/case sensitivity
	CandidateType candidate_type;
	//! Extra char to push at the back
	char extra_char = '\0';
	//! Suggestion position
	idx_t suggestion_pos = 0;
	//! The final score
	optional_idx score;
};

struct AutoCompleteSuggestion {
	AutoCompleteSuggestion(string text_p, idx_t pos, string type_p, idx_t score, char extra_char_p)
	    : text(std::move(text_p)), pos(pos), type(std::move(type_p)), score(score), extra_char(extra_char_p) {
	}

	string text;
	idx_t pos;
	string type;
	idx_t score;
	char extra_char;
};

enum class SuggestionType { OPTIONAL, MANDATORY };

enum class MatchMode : uint8_t { BUILD_PARSE_RESULT, RECOGNIZE_ONLY };

class MatcherResult {
public:
	static MatcherResult Success(optional_ptr<ParseResult> parse_result = nullptr) {
		return MatcherResult(true, parse_result);
	}

	static MatcherResult Failure() {
		return MatcherResult(false, nullptr);
	}

	bool IsSuccess() const {
		return success;
	}

	bool HasParseResult() const {
		return parse_result != nullptr;
	}

	optional_ptr<ParseResult> GetParseResult() const {
		return parse_result;
	}

private:
	MatcherResult(bool success_p, optional_ptr<ParseResult> parse_result_p)
	    : success(success_p), parse_result(parse_result_p) {
	}

private:
	bool success;
	optional_ptr<ParseResult> parse_result;
};

struct MatcherSuggestion {
	// NOLINTNEXTLINE: allow implicit conversion from auto-complete candidate
	MatcherSuggestion(AutoCompleteCandidate keyword_p) : keyword(std::move(keyword_p)), type(keyword.suggestion_type) {
	}
	// NOLINTNEXTLINE: allow implicit conversion from suggestion state
	MatcherSuggestion(SuggestionState type, char extra_char = '\0')
	    : keyword("", type), type(type), extra_char(extra_char) {
	}

	//! Literal suggestion
	AutoCompleteCandidate keyword;
	SuggestionState type;
	char extra_char = '\0';
};

struct MatchContext {
	MatchContext(vector<MatcherSuggestion> &suggestions_p, ParseResultAllocator &allocator_p,
	             ArenaAllocator &process_allocator_p, idx_t &max_token_index_p,
	             MatchMode mode_p = MatchMode::BUILD_PARSE_RESULT,
	             IdentifierCaseMode identifier_case_mode_p = IdentifierCaseMode::PRESERVE_CASE,
	             ParserPackratCache *packrat_cache_p = nullptr)
	    : suggestions(suggestions_p), allocator(allocator_p), process_allocator(process_allocator_p),
	      max_token_index(max_token_index_p), identifier_case_mode(identifier_case_mode_p),
	      packrat_cache(packrat_cache_p), mode(mode_p) {
	}

	vector<MatcherSuggestion> &suggestions;
	ParseResultAllocator &allocator;
	ArenaAllocator &process_allocator;
	idx_t &max_token_index;
	IdentifierCaseMode identifier_case_mode;
	ParserPackratCache *packrat_cache;
	MatchMode mode;
};

struct MatchState {
	MatchState(TokenIterator &token_iterator_p, MatchContext &context_p)
	    : token_iterator(token_iterator_p), context(context_p) {
	}
	MatchState(const MatchState &state)
	    : token_iterator(state.token_iterator), context(state.context), rule(state.rule) {
	}

	TokenIterator token_iterator;
	MatchContext &context;
	unique_ptr<reference_set_t<const Matcher>> added_suggestions;
	optional_ptr<const CompiledGrammarRule> rule;

	bool BuildParseResult() const {
		return context.mode == MatchMode::BUILD_PARSE_RESULT;
	}

	template <class RESULT, class... ARGS>
	MatcherResult AllocateParseResult(ARGS &&... args);

	template <class PROCESS, class... ARGS>
	arena_ptr<MatchProcess> Make(ARGS &&... args);

	void UpdateMaxTokenIndex() {
		if (token_iterator.Position() > context.max_token_index) {
			context.max_token_index = token_iterator.Position();
		}
	}

	idx_t GetMaxTokenIndex() const {
		return context.max_token_index;
	}

	//! Fold a non-quoted identifier in-place according to the configured case mode
	void FoldIdentifier(string &text) const {
		switch (context.identifier_case_mode) {
		case IdentifierCaseMode::LOWERCASE:
			text = StringUtil::Lower(text);
			break;
		case IdentifierCaseMode::UPPERCASE:
			text = StringUtil::Upper(text);
			break;
		default:
			break;
		}
	}

	void AddSuggestion(MatcherSuggestion suggestion);
};

//! Input to start a Matcher execution
struct MatchInput {
	const Matcher &matcher;
	MatchState &state;
};

//! Essentially a std::variant<MatchInput, MatcherResult>
//! Produced by a MatchProcess::Resume call, controlling the next step in the execution
class MatchStep {
public:
	static MatchStep Child(MatchInput input);
	static MatchStep Complete(MatcherResult result);

	optional<MatchInput> GetChild();
	MatcherResult GetResult() const;

private:
	MatchStep(optional<MatchInput> child_p, optional<MatcherResult> result_p)
	    : child(std::move(child_p)), result(result_p) {
	}

private:
	optional<MatchInput> child;
	optional<MatcherResult> result;
};

class MatchProcess {
public:
	virtual ~MatchProcess() = default;

	//! Resume matching, optionally with the result of the previously requested child.
	virtual MatchStep Resume(optional<MatcherResult> child_result) = 0;
};

enum class MatcherType {
	KEYWORD,
	LIST,
	OPTIONAL,
	CHOICE,
	REPEAT,
	VARIABLE,
	STRING_LITERAL,
	NUMBER_LITERAL,
	OPERATOR,
	END_OF_INPUT,
	CUSTOM
};

//! Which tokens a matcher can start with, computed once per grammar by MatcherAllocator::ComputeStartSets. Used by
//! Matcher::MayMatchHere to skip matchers that cannot match at the current token without pushing a frame for them.
struct MatcherStartSet {
	//! Anything may start this matcher (custom or untyped atomic matchers, or a cycle in the grammar): never prune
	bool any = false;
	optional_ptr<const GrammarLiteralTable> literal_table;
	//! Sorted literal ids of the keywords that can start the matcher
	vector<uint16_t> literal_ids;
	//! Atomic matchers with a token predicate (identifiers, operators) that can start the matcher
	vector<reference<const Matcher>> predicate_leaders;
};

class Matcher {
public:
	explicit Matcher(MatcherType type = MatcherType::CUSTOM) : type(type) {
	}
	virtual ~Matcher() = default;

	//! Match and construct the parse result
	MatcherResult MatchParseResult(MatchState &state) const;
	//! Create matcher-local state with state.Make<PROCESS>() for either execution driver.
	virtual arena_ptr<MatchProcess> StartMatch(MatchState &state) const = 0;
	virtual bool IsAtomic() const {
		return false;
	}
	//! Cheap, conservative pre-check used to skip matchers that cannot possibly match at the current token, without
	//! pushing a frame for them. Returns false only when a match is certainly impossible; anything unsure (custom
	//! matchers, nullable children, deep nesting) answers true. Never prunes at the autocomplete cursor, where the
	//! failing children are what produce the suggestions.
	bool MayMatchHere(MatchState &state) const;
	//! Token predicate for atomic matchers, consulted through the start sets; composite matchers never override it
	virtual bool CanStartWith(MatchState &state, idx_t depth) const {
		return true;
	}
	bool IsNullable() const {
		return nullable;
	}
	virtual SuggestionType AddSuggestion(MatchState &state) const;
	virtual SuggestionType AddSuggestionInternal(MatchState &state) const = 0;
	virtual string ToString() const = 0;
	void Print() const;

	MatcherType Type() const {
		return type;
	}
	void SetName(string name_p) {
		name = std::move(name_p);
	}
	void SetRule(const CompiledGrammarRule &rule_p) {
		rule = rule_p;
		name = rule_p.name;
	}
	optional_ptr<const CompiledGrammarRule> GetRule() const {
		return rule;
	}
	bool HasName() const {
		return !name.empty();
	}
	string GetName() const;
	optional_idx GetPackratId() const {
		return packrat_id;
	}
	//! Mark the matcher as memoized. The id is a dense index over the memoized matchers of a grammar and is used
	//! to index directly into the packrat cache.
	void SetPackratMemoized(idx_t packrat_id_p) {
		packrat_memoized = true;
		packrat_id = optional_idx(packrat_id_p);
	}
	bool IsPackratMemoized() const {
		return packrat_memoized;
	}

public:
	template <class TARGET>
	TARGET &Cast() {
		if (type != TARGET::TYPE) {
			throw InternalException("Failed to cast matcher to type - matcher type mismatch");
		}
		return reinterpret_cast<TARGET &>(*this);
	}

	template <class TARGET>
	const TARGET &Cast() const {
		if (type != TARGET::TYPE) {
			throw InternalException("Failed to cast matcher to type - matcher type mismatch");
		}
		return reinterpret_cast<const TARGET &>(*this);
	}

protected:
	friend class MatcherAllocator;
	MatcherType type;
	string name;
	optional_idx packrat_id;
	bool packrat_memoized = false;
	optional_ptr<const CompiledGrammarRule> rule;
	//! See MatcherStartSet; null until MatcherAllocator::ComputeStartSets ran (MayMatchHere then falls back to the
	//! matcher's own CanStartWith)
	unique_ptr<MatcherStartSet> start_set;
	bool nullable = false;
};

class AtomicMatcher : public Matcher {
public:
	explicit AtomicMatcher(MatcherType type) : Matcher(type) {
	}

	bool IsAtomic() const final {
		return true;
	}
	DUCKDB_API arena_ptr<MatchProcess> StartMatch(MatchState &state) const final;
	virtual MatcherResult MatchAtomic(MatchState &state) const = 0;
};

class KeywordInfo {
public:
	KeywordInfo() {
	}
	explicit KeywordInfo(int32_t score_bonus, char extra_char = ' ')
	    : score_bonus(score_bonus), extra_char(extra_char) {
	}

public:
	int32_t score_bonus = 0;
	char extra_char = '\0';
};

class MatcherAllocator {
public:
	Matcher &Allocate(unique_ptr<Matcher> matcher);
	//! Compute MatcherStartSet for every allocated matcher. Called once the matcher graph of a grammar is complete.
	void ComputeStartSets();

private:
	vector<unique_ptr<Matcher>> matchers;
};

class ParseResultAllocator {
public:
	optional_ptr<ParseResult> Allocate(unique_ptr<ParseResult> parse_result);

private:
	vector<unique_ptr<ParseResult>> parse_results;
};

template <class PROCESS, class... ARGS>
arena_ptr<MatchProcess> MatchState::Make(ARGS &&... args) {
	static_assert(std::is_base_of<MatchProcess, PROCESS>::value, "Expected a matcher process");
	return arena_ptr<MatchProcess>(context.process_allocator.Make<PROCESS>(std::forward<ARGS>(args)...));
}

template <class RESULT, class... ARGS>
MatcherResult MatchState::AllocateParseResult(ARGS &&... args) {
	if (!BuildParseResult()) {
		return MatcherResult::Success();
	}
	auto result = context.allocator.Allocate(make_uniq<RESULT>(std::forward<ARGS>(args)...));
	if (rule) {
		result->SetRule(*rule);
		result->name = rule->name;
	}
	return MatcherResult::Success(result);
}

} // namespace duckdb
