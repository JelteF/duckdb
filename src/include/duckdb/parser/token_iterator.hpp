//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/token_iterator.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/optional_ptr.hpp"
#include "duckdb/common/unique_ptr.hpp"
#include "duckdb/common/vector.hpp"
#include "duckdb/parser/peg/matcher_token.hpp"

namespace duckdb {
struct SimpleToken;

//! Iterates over an already-tokenized query. A root iterator can own its tokens; child iterators
//! reference the same tokens and carry an independent position for speculative parsing.
class TokenIterator {
public:
	DUCKDB_API explicit TokenIterator(unique_ptr<vector<MatcherToken>> owned_tokens);
	DUCKDB_API explicit TokenIterator(vector<MatcherToken> &tokens);
	//! Inline: a child MatchState is copied for nearly every matcher frame
	TokenIterator(const TokenIterator &other) : tokens(other.tokens), position(other.position) {
	}
	DUCKDB_API TokenIterator(TokenIterator &&other) noexcept;
	TokenIterator &operator=(const TokenIterator &) = delete;
	TokenIterator &operator=(TokenIterator &&) = delete;

	DUCKDB_API bool HasMoreStatements() const;
	DUCKDB_API idx_t EndOffset() const;

	//! Inline: the matcher reads and rewinds the position for every child it tries
	idx_t Position() const {
		return position;
	}
	idx_t Size() const {
		return tokens.size();
	}
	bool AtEnd() const {
		auto current = Current();
		return !current || current->type == TokenType::END_OF_INPUT;
	}

	//! Inline: this is called on every matcher step, an out-of-line call here is measurable
	optional_ptr<const MatcherToken> Current() const {
		if (position >= tokens.size()) {
			return nullptr;
		}
		return tokens[position];
	}
	//! True when the stream was tokenized for auto-completion, which puts the cursor token at the end
	bool HasAutocompleteCursor() const {
		return !tokens.empty() && tokens.back().type == TokenType::END_OF_INPUT_AUTOCOMPLETE;
	}
	LiteralInfo CurrentLiteralInfo(const GrammarLiteralTable &table) {
		if (position >= tokens.size()) {
			return LiteralInfo();
		}
		return tokens[position].GetLiteralInfo(table);
	}
	DUCKDB_API const MatcherToken &Previous() const;
	DUCKDB_API const MatcherToken &GetToken(idx_t index) const;

	DUCKDB_API void Advance(idx_t count = 1);
	void SetPosition(idx_t position_p) {
		if (position_p > tokens.size()) {
			ThrowPositionOutOfRange(position_p);
		}
		position = position_p;
	}
	void SetPosition(const TokenIterator &other) {
		if (&tokens != &other.tokens) {
			ThrowForeignTokens();
		}
		SetPosition(other.position);
	}
	DUCKDB_API void SetPreviousTokenType(TokenType type);

	DUCKDB_API vector<SimpleToken> RemainingTokens() const;
	DUCKDB_API string ToString() const;

private:
	[[noreturn]] DUCKDB_API void ThrowPositionOutOfRange(idx_t position_p) const;
	[[noreturn]] DUCKDB_API void ThrowForeignTokens() const;
	unique_ptr<vector<MatcherToken>> owned_tokens;
	vector<MatcherToken> &tokens;
	idx_t position = 0;
};

} // namespace duckdb
