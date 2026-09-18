#pragma once

#include "duckdb/parser/peg/matcher.hpp"

namespace duckdb {

class RepeatMatcher : public Matcher {
public:
	static constexpr MatcherType TYPE = MatcherType::REPEAT;

public:
	explicit RepeatMatcher(Matcher &element_p) : Matcher(TYPE), element(element_p) {
	}

	DUCKDB_API arena_ptr<MatchProcess> StartMatch(MatchState &state) const override;
	bool CanStartWith(MatchState &state, idx_t depth) const override {
		return element.CanStartWith(state, depth + 1);
	}

	SuggestionType AddSuggestionInternal(MatchState &state) const override {
		element.AddSuggestion(state);
		return SuggestionType::MANDATORY;
	}

	string ToString() const override {
		return element.GetName() + "*";
	}
	const Matcher &GetChildMatcher() const {
		return element;
	}

private:
	Matcher &element;
};

} // namespace duckdb
