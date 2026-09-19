#pragma once

#include "duckdb/parser/peg/matcher.hpp"

namespace duckdb {
class ListMatcher;

class RepeatMatcher : public Matcher {
public:
	static constexpr MatcherType TYPE = MatcherType::REPEAT;

public:
	explicit RepeatMatcher(Matcher &element_p) : Matcher(TYPE), element(element_p) {
	}

	DUCKDB_API arena_ptr<MatchProcess> StartMatch(MatchState &state) const override;

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
	Matcher &GetChildMatcher() {
		return element;
	}

	//! An element of the form `Atom X` is matched in the repeat's own frame, which builds the list result the
	//! element would have built. See RepeatMatchProcess.
	void SetFusedElement(const ListMatcher &element_p) {
		fused_element = element_p;
	}
	optional_ptr<const ListMatcher> GetFusedElement() const {
		return fused_element;
	}

private:
	Matcher &element;
	optional_ptr<const ListMatcher> fused_element;
};

} // namespace duckdb
