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
#include "duckdb/storage/arena_allocator.hpp"

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
	ParserPackratCache(ArenaAllocator &arena, idx_t token_count, idx_t matcher_count);
	~ParserPackratCache();

	optional_ptr<const ParserPackratEntry> Lookup(const Matcher &matcher, idx_t token_index) const;
	void Store(const Matcher &matcher, idx_t token_index, ParserPackratEntry entry);

private:
	//! The entries of one memoized matcher, allocated from the parse's arena on the first store and left to the
	//! arena to reclaim. The entries are raw storage: one is only read once `present` says it was written, so a
	//! slot per token costs nothing to set up. A rule that never stores anything never allocates at all.
	struct PackratRow {
		ParserPackratEntry *entries = nullptr;
		uint8_t *present = nullptr;
	};

private:
	//! A row is megabytes for a long statement, and taking it from `vector` put the parser's largest single
	//! allocation on the C library's heap rather than with the allocator the rest of the match uses.
	ArenaAllocator &arena;
	vector<PackratRow> rows;
	idx_t token_count;
};

} // namespace duckdb
