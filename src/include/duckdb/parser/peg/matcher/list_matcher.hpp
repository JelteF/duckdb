#pragma once

#include "duckdb/parser/peg/matcher.hpp"
#include "duckdb/parser/peg/matcher/choice_matcher.hpp"
#include "duckdb/parser/peg/matcher/precedence_hierarchy.hpp"

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
			result += matcher.get().GetPrintName();
		}
		return "(" + result + ")";
	}

	//! Matched in the choice's own frame rather than in one of its own. See MatcherFactory::FuseSingleChoiceRules.
	void SetFusedChoice(const ChoiceMatcher &choice) {
		fused_choice = choice;
	}
	optional_ptr<const ChoiceMatcher> GetFusedChoice() const {
		return fused_choice;
	}

	//! Matched as a level of the operator precedence hierarchy rather than as a plain list. See PrecedenceHierarchy.
	void SetPrecedenceLevel(const PrecedenceHierarchy &hierarchy_p, idx_t level_p) {
		hierarchy = hierarchy_p;
		level = level_p;
	}
	optional_ptr<const PrecedenceHierarchy> GetHierarchy() const {
		return hierarchy;
	}
	idx_t GetPrecedenceLevel() const {
		return level;
	}

public:
	vector<reference<Matcher>> matchers;
	//! If true, this matcher will not contribute autocomplete suggestions (used for rules like ExpressionStatement)
	bool suppress_suggestions = false;

private:
	optional_ptr<const ChoiceMatcher> fused_choice;
	optional_ptr<const PrecedenceHierarchy> hierarchy;
	idx_t level = 0;
};

} // namespace duckdb
