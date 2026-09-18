//===----------------------------------------------------------------------===//
//                         DuckDB
//
// parser_packrat.hpp
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/common.hpp"
#include "duckdb/common/optional_ptr.hpp"
#include "duckdb/common/vector.hpp"
#include "duckdb/common/helper.hpp"
#include "duckdb/common/numeric_utils.hpp"
#include "duckdb/common/helper.hpp"

namespace duckdb {
class Matcher;
class ParseResult;

struct ParserPackratEntry {
	bool success = false;
	idx_t token_index_after = 0;
	idx_t max_token_index_seen = 0;
	optional_ptr<ParseResult> result;
};

//! Memoization table for the memoized grammar rules, indexed by (matcher, token position).
//! This is a flat array per matcher rather than a hash map: lookups happen on nearly every rule entry, and the key
//! space (a handful of memoized matchers times the token count) is small and dense enough that a direct index is
//! both faster and, for the matchers that are actually hit, not larger than the hash nodes would be.
class ParserPackratCache {
public:
	ParserPackratCache(idx_t base_token_index, idx_t token_count, idx_t matcher_count);
	~ParserPackratCache();

	optional_ptr<const ParserPackratEntry> Lookup(const Matcher &matcher, idx_t token_index) const;
	void Store(const Matcher &matcher, idx_t token_index, ParserPackratEntry entry);

private:
	static constexpr idx_t INITIAL_ROW_CAPACITY = 1024;

	//! Rows are indexed from the statement's first token: one cache serves one statement, and a statement is
	//! usually a small part of the query it sits in.
	idx_t RowIndex(idx_t token_index) const {
		return token_index < base_token_index ? token_count : token_index - base_token_index;
	}

	//! The entries of one memoized matcher. The entries themselves are raw storage: an entry is only read once
	//! `present` says it was written, so constructing a slot per token of the statement - which for a long
	//! statement is the largest single cost of memoizing at all - is skipped. A row also starts small and doubles,
	//! because a statement is usually far shorter than the query it sits in.
	struct PackratRow {
		unsafe_unique_array<data_t> entries;
		unsafe_unique_array<uint8_t> present;
		idx_t capacity = 0;

		ParserPackratEntry *Entries() const {
			return reinterpret_cast<ParserPackratEntry *>(entries.get());
		}
		void Grow(idx_t required, idx_t maximum);
	};

private:
	vector<PackratRow> rows;
	idx_t base_token_index;
	idx_t token_count;
};

} // namespace duckdb
