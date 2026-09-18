#pragma once

#include "duckdb/parser/peg/matcher.hpp"
#include "duckdb/parser/peg/matcher/optional_matcher.hpp"

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
	//! The first element that is not an optional decides. A leading optional can start the list itself, or be
	//! skipped and let the next element decide; other nullable shapes answer true, which keeps this conservative.
	bool CanStartWith(MatchState &state, idx_t depth) const override {
		if (depth >= MAX_START_CHECK_DEPTH) {
			return true;
		}
		for (auto &element : matchers) {
			auto &child = element.get();
			if (child.Type() == MatcherType::OPTIONAL) {
				if (child.Cast<OptionalMatcher>().GetChildMatcher().CanStartWith(state, depth + 1)) {
					return true;
				}
				continue;
			}
			return child.CanStartWith(state, depth + 1);
		}
		return true;
	}

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

public:
	vector<reference<Matcher>> matchers;
	//! If true, this matcher will not contribute autocomplete suggestions (used for rules like ExpressionStatement)
	bool suppress_suggestions = false;
};

} // namespace duckdb
