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
	ParserPackratCache(idx_t token_count, idx_t matcher_count);
	~ParserPackratCache();

	optional_ptr<const ParserPackratEntry> Lookup(const Matcher &matcher, idx_t token_index) const;
	void Store(const Matcher &matcher, idx_t token_index, ParserPackratEntry entry);

private:
	//! One row per memoized matcher, allocated on first store so matchers that never run cost nothing
	vector<vector<ParserPackratEntry>> rows;
	//! Per row, which positions hold an entry
	vector<vector<uint8_t>> present;
	idx_t token_count;
};

} // namespace duckdb
