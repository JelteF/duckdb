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
	bool CanStartWith(MatchState &state, idx_t depth) const override {
		auto literal = state.token_iterator.CurrentLiteralInfo(table);
		return literal_children.find(literal.LiteralId()) != literal_children.end();
	}

private:
	const GrammarLiteralTable &table;
	unordered_map<uint32_t, idx_t> literal_children;
};

} // namespace duckdb
