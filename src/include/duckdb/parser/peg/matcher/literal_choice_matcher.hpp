#pragma once

#include "duckdb/parser/peg/matcher/choice_matcher.hpp"

namespace duckdb {

class LiteralChoiceMatcher final : public ChoiceMatcher {
public:
	LiteralChoiceMatcher(vector<reference<Matcher>> &&matchers, const GrammarLiteralTable &table_p,
	                     unordered_map<uint32_t, idx_t> &&literal_children_p)
	    : ChoiceMatcher(std::move(matchers)), table(table_p), literal_children(std::move(literal_children_p)) {
	}

	arena_ptr<MatchProcess> StartMatch(MatchState &state) const override;
	arena_ptr<MatchProcess> StartFusedMatch(MatchState &state, const ListMatcher &wrapper) const override;

	//! The single alternative the current token can select, or the alternative count when none can
	idx_t DispatchIndex(MatchState &state) const;

private:
	const GrammarLiteralTable &table;
	unordered_map<uint32_t, idx_t> literal_children;
};

} // namespace duckdb
