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
#include "duckdb/common/bit_utils.hpp"
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
struct PrecedenceLadder;

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

	//! An optional that matched nothing carries no information at all: no rule, no name and no source location. One
	//! instance per match run therefore stands in for all of them.
	optional_ptr<ParseResult> EmptyOptionalResult();

private:
	optional_ptr<ParseResult> empty_optional;
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
//! Produced by a MatchProcess::Resume call, controlling the next step in the execution. Kept to a trivially copyable
//! pair of pointers and a result: one of these is returned for every step of every frame.
class MatchStep {
public:
	static MatchStep Child(MatchInput input) {
		return MatchStep(&input.matcher, &input.state, MatcherResult::Failure());
	}
	static MatchStep Complete(MatcherResult result) {
		return MatchStep(nullptr, nullptr, result);
	}

	bool HasChild() const {
		return child_matcher != nullptr;
	}
	MatchInput GetChild() const {
		D_ASSERT(child_matcher && child_state);
		return MatchInput {*child_matcher, *child_state};
	}
	MatcherResult GetResult() const {
		D_ASSERT(!child_matcher);
		return result;
	}

private:
	MatchStep(const Matcher *child_matcher_p, MatchState *child_state_p, MatcherResult result_p)
	    : child_matcher(child_matcher_p), child_state(child_state_p), result(result_p) {
	}

private:
	const Matcher *child_matcher;
	MatchState *child_state;
	MatcherResult result;
};

class MatchProcess {
public:
	virtual ~MatchProcess() = default;

	//! Resume matching, with the result of the previously requested child or nullptr on the first call.
	virtual MatchStep Resume(const MatcherResult *child_result) = 0;
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
	//! One bit per literal id that can start the matcher. The grammar numbers its literals densely, so an id indexes
	//! this directly: the answer is exact and takes one test, where a sorted list took a search and a bloom filter
	//! to keep the common miss cheap. The words live in MatcherAllocator::start_set_bits, one buffer for the whole
	//! grammar, so reaching them is a load from a contiguous region rather than a chase into a node of its own.
	const uint64_t *literal_words = nullptr;
	uint32_t literal_word_count = 0;
	//! Atomic matchers with a token predicate (identifiers, operators) that can start the matcher. Like the bitmap,
	//! these live in one buffer for the whole grammar rather than a vector per set.
	const reference<const Matcher> *leaders = nullptr;
	uint32_t leader_count = 0;

	bool HasLiteral(uint16_t literal_id) const {
		auto word = static_cast<idx_t>(literal_id) / 64;
		if (word >= literal_word_count) {
			return false;
		}
		return (literal_words[word] & (uint64_t(1) << (literal_id % 64))) != 0;
	}

	template <class FUNC>
	void ForEachLiteral(FUNC &&callback) const {
		for (idx_t word = 0; word < literal_word_count; word++) {
			auto bits = literal_words[word];
			while (bits) {
				auto bit = CountZeros<uint64_t>::Trailing(bits);
				callback(static_cast<uint16_t>(word * 64 + bit));
				bits &= bits - 1;
			}
		}
	}
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
	//! Inline: asked for nearly every child the matcher considers, and answered from the start set without a call
	bool MayMatchHere(MatchState &state) const {
		auto token = state.token_iterator.Current();
		if (!token) {
			return true;
		}
		// never prune at the auto-complete cursor, where the failing children are what produce the suggestions
		if (state.token_iterator.HasAutocompleteCursor() && token->type == TokenType::END_OF_INPUT_AUTOCOMPLETE) {
			return true;
		}
		auto set = start_set.get();
		if (!set) {
			return CanStartWith(state, 0);
		}
		if (set->any) {
			return true;
		}
		if (set->literal_word_count != 0) {
			auto literal_id = state.token_iterator.CurrentLiteralInfo(*set->literal_table).LiteralId();
			if (literal_id && set->HasLiteral(literal_id)) {
				return true;
			}
		}
		if (set->leader_count == 0) {
			return false;
		}
		return MatchesPredicateLeader(state, *set);
	}
	//! The uncommon half of MayMatchHere: a matcher that identifiers or operators can start
	bool MatchesPredicateLeader(MatchState &state, const MatcherStartSet &set) const;
	//! Token predicate for atomic matchers, consulted through the start sets; composite matchers never override it
	virtual bool CanStartWith(MatchState &state, idx_t depth) const {
		return true;
	}
	bool IsNullable() const {
		return nullable;
	}
	//! The tokens this matcher can start with, or null before MatcherAllocator::ComputeStartSets ran
	optional_ptr<const MatcherStartSet> GetStartSet() const {
		return start_set;
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
	//! The stored name, which lives as long as the grammar and can therefore be referenced by a parse result
	const string &GetNameRef() const {
		return name;
	}
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
	//! See MatcherFactory::AddCollapsibleRule
	void SetCollapsible() {
		collapsible = true;
	}
	bool IsCollapsible() const {
		return collapsible;
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
	bool collapsible = false;
	optional_ptr<const CompiledGrammarRule> rule;
	//! See MatcherStartSet; null until MatcherAllocator::ComputeStartSets ran (MayMatchHere then falls back to the
	//! matcher's own CanStartWith)
	optional_ptr<const MatcherStartSet> start_set;
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
	//! Take ownership of a precedence ladder, which lives as long as the matchers that refer to it
	PrecedenceLadder &AddLadder(unique_ptr<PrecedenceLadder> ladder);
	//! Fuse the element of every repeat that is written as `(Atom X)*` into the repeat itself
	void FuseRepeatElements();

private:
	vector<unique_ptr<Matcher>> matchers;
	vector<unique_ptr<PrecedenceLadder>> ladders;
	//! Every matcher's start set, and every start set's literal bitmap, each in one contiguous buffer. The matcher
	//! graph is walked at random, so what costs is the chase into a scattered node, not the bytes.
	vector<MatcherStartSet> start_sets;
	vector<uint64_t> start_set_bits;
	vector<reference<const Matcher>> start_set_leaders;
};

//! Owns the parse results of one match run. Results are carved out of an arena instead of being allocated one by
//! one: a parse creates a result per matched rule and freeing them all at the end is the only lifetime needed.
class ParseResultAllocator {
public:
	ParseResultAllocator();
	~ParseResultAllocator();

	template <class RESULT, class... ARGS>
	optional_ptr<ParseResult> Make(ARGS &&... args) {
		static_assert(std::is_base_of<ParseResult, RESULT>::value, "Expected a parse result");
		auto result = arena.Make<RESULT>(std::forward<ARGS>(args)...);
		if (RESULT::NEEDS_DESTRUCTOR) {
			parse_results.emplace_back(result);
		}
		return optional_ptr<ParseResult>(result);
	}

	//! Copy a collected set of children into the arena, where it lives as long as the results it belongs to
	ParseResultChildren MakeChildren(const reference<ParseResult> *children, idx_t count) {
		if (count == 0) {
			return ParseResultChildren();
		}
		arena.AlignNext();
		auto target =
		    reinterpret_cast<reference<ParseResult> *>(arena.Allocate(count * sizeof(reference<ParseResult>)));
		memcpy(static_cast<void *>(target), static_cast<const void *>(children),
		       count * sizeof(reference<ParseResult>));
		return ParseResultChildren(target, count);
	}

private:
	ArenaAllocator arena;
	//! Only tracked to run the destructors; the memory itself belongs to the arena
	vector<arena_ptr<ParseResult>> parse_results;
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
	auto result = context.allocator.Make<RESULT>(std::forward<ARGS>(args)...);
	if (rule) {
		result->SetRule(*rule);
		if (!result->HasName()) {
			result->SetName(rule->name);
		}
	}
	return MatcherResult::Success(result);
}

} // namespace duckdb
