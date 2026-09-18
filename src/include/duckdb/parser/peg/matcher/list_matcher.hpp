#pragma once

#include "duckdb/parser/peg/matcher.hpp"
#include "duckdb/parser/peg/matcher/precedence_ladder.hpp"
#include "duckdb/parser/peg/matcher/choice_matcher.hpp"

namespace duckdb {

class ListMatcher : public Matcher {
public:
	static constexpr MatcherType TYPE = MatcherType::LIST;

public:
	ListMatcher() : Matcher(TYPE) {
	}
	explicit ListMatcher(vector<reference<Matcher>> matchers_p) : Matcher(TYPE), matchers(std::move(matchers_p)) {
	}

	DUCKDB_API arena_ptr<MatchProcess> StartMatch(MatchState &state) const override;

	SuggestionType AddSuggestionInternal(MatchState &state) const override {
		if (suppress_suggestions) {
			return SuggestionType::OPTIONAL;
		}
		for (auto &matcher : matchers) {
			auto suggestion_result = matcher.get().AddSuggestion(state);
			if (suggestion_result == SuggestionType::MANDATORY) {
				// we must match this suggestion before continuing
				return SuggestionType::MANDATORY;
			}
		}
		// all child suggestions were optional - the entire list is optional
		return SuggestionType::OPTIONAL;
	}

	string ToString() const override {
		string result = "";
		for (auto &matcher : matchers) {
			if (!result.empty()) {
				result += " ";
			}
			result += matcher.get().GetName();
		}
		return "(" + result + ")";
	}

	//! Matched as a level of the operator precedence ladder rather than as a plain list. See PrecedenceLadder.
	void SetPrecedenceLevel(const PrecedenceLadder &ladder_p, idx_t level_p) {
		ladder = ladder_p;
		level = level_p;
	}
	optional_ptr<const PrecedenceLadder> GetLadder() const {
		return ladder;
	}

	//! A rule whose whole body is one ordered choice is matched in the choice's own frame, which builds the list
	//! result this matcher would have built. See ChoiceMatchProcess.
	void SetFusedChoice(const ChoiceMatcher &choice) {
		fused_choice = choice;
	}
	optional_ptr<const ChoiceMatcher> GetFusedChoice() const {
		return fused_choice;
	}
	idx_t GetPrecedenceLevel() const {
		return level;
	}

public:
	vector<reference<Matcher>> matchers;
	//! If true, this matcher will not contribute autocomplete suggestions (used for rules like ExpressionStatement)
	bool suppress_suggestions = false;

private:
	optional_ptr<const PrecedenceLadder> ladder;
	optional_ptr<const ChoiceMatcher> fused_choice;
	idx_t level = 0;
};

} // namespace duckdb
