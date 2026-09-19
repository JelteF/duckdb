#include "duckdb/parser/peg/tokenizer/highlight_tokenizer.hpp"

namespace duckdb {

HighlightTokenizerBehavior::HighlightTokenizerBehavior(const string &sql, vector<MatcherToken> &tokens)
    : TokenizerBehavior(sql, tokens) {
}

void HighlightTokenizerBehavior::PushToken(idx_t start, idx_t end, TokenType type, bool unterminated) {
	if (start >= end) {
		return;
	}
	tokens.emplace_back(sql.data() + start, end - start, start, type, unterminated);
}

void HighlightTokenizerBehavior::OnStatementEnd(idx_t pos) {
	tokens.emplace_back(";", 1, pos, TokenType::TERMINATOR);
}
} // namespace duckdb
