//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/peg/matcher_token.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/string.hpp"
#include "duckdb/parser/peg/token_type.hpp"
#include "duckdb/parser/peg/grammar_literal_table.hpp"

#include <string_view>

namespace duckdb {

//! A token of a tokenized query. Its text is a view of the query rather than a copy of it, so the query has to
//! outlive every token taken from it. Everything that holds tokens past the parse it came from already respects that:
//! Parser::ParseQuery keeps the normalized query in scope for as long as it parses, ParseIterator declares its `sql`
//! before the iterator that views it, and the two things that hand tokens out - Parser::Tokenize and
//! TokenIterator::RemainingTokens - copy the text into a SimplifiedToken or SimpleToken first.
struct MatcherToken {
	MatcherToken(const char *text_p, idx_t length_p, idx_t offset_p, TokenType type_p, bool unterminated_p = false)
	    : type(type_p), text(text_p, length_p), offset(offset_p), length(length_p), unterminated(unterminated_p) {
	}

	TokenType type;
	std::string_view text;
	idx_t offset = 0;
	idx_t length = 0;
	bool unterminated = false;
	bool preceded_by_newline = false;
	bool preceded_by_block_comment = false;

	LiteralInfo GetLiteralInfo(const GrammarLiteralTable &table) {
		if (literal_table_id != table.CacheId()) {
			literal_info = table.Lookup(text);
			literal_table_id = table.CacheId();
		}
		return literal_info;
	}

	void ResetLiteralInfo() {
		literal_table_id = 0;
	}

private:
	LiteralInfo literal_info;
	uint64_t literal_table_id = 0;
};

} // namespace duckdb
