#pragma once

#include "duckdb/parser/peg/matcher.hpp"
#include "duckdb/common/string.hpp"

namespace duckdb {
class ListMatcher;

class ChoiceMatcher : public Matcher {
public:
	static constexpr MatcherType TYPE = MatcherType::CHOICE;

public:
	ChoiceMatcher() : Matcher(TYPE) {
	}
	explicit ChoiceMatcher(vector<reference<Matcher>> &&matchers_p) : Matcher(TYPE), matchers(std::move(matchers_p)) {
	}

	DUCKDB_API arena_ptr<MatchProcess> StartMatch(MatchState &state) const override;
	//! Start the choice in the frame of the rule whose whole body it is, so that it builds the list result that
	//! rule's own frame would have built. See ListMatcher::SetFusedChoice.
	DUCKDB_API virtual arena_ptr<MatchProcess> StartFusedMatch(MatchState &state, const ListMatcher &wrapper) const;

	SuggestionType AddSuggestionInternal(MatchState &state) const override {
		for (auto &child_matcher : matchers) {
			child_matcher.get().AddSuggestion(state);
		}
		return SuggestionType::MANDATORY;
	}

	string ToString() const override {
		string result = "";
		for (auto &matcher : matchers) {
			if (!result.empty()) {
				result += " / ";
			}
			result += matcher.get().GetPrintName();
		}
		return result;
	}

public:
	vector<reference<Matcher>> matchers;
};

} // namespace duckdb
