#include "duckdb/parser/peg/parser_packrat.hpp"

#include "duckdb/parser/peg/matcher.hpp"

#include <cstring>

namespace duckdb {

ParserPackratCache::ParserPackratCache(ArenaAllocator &arena_p, idx_t token_count_p, idx_t matcher_count)
    : arena(arena_p), rows(matcher_count), token_count(token_count_p + 1) {
}

ParserPackratCache::~ParserPackratCache() = default;

optional_ptr<const ParserPackratEntry> ParserPackratCache::Lookup(const Matcher &matcher, idx_t token_index) const {
	D_ASSERT(matcher.IsPackratMemoized());
	auto matcher_id = matcher.GetPackratId().GetIndex();
	D_ASSERT(matcher_id < rows.size());
	auto &row = rows[matcher_id];
	if (!row.entries || token_index >= token_count || !row.present[token_index]) {
		return nullptr;
	}
	return optional_ptr<const ParserPackratEntry>(row.entries + token_index);
}

void ParserPackratCache::Store(const Matcher &matcher, idx_t token_index, ParserPackratEntry entry) {
	D_ASSERT(matcher.IsPackratMemoized());
	auto matcher_id = matcher.GetPackratId().GetIndex();
	D_ASSERT(matcher_id < rows.size());
	if (token_index >= token_count) {
		return;
	}
	auto &row = rows[matcher_id];
	if (!row.entries) {
		// first store for this rule: take both arrays from the arena in one go, leaving the entries raw
		arena.AlignNext();
		row.entries = reinterpret_cast<ParserPackratEntry *>(arena.Allocate(token_count * sizeof(ParserPackratEntry)));
		row.present = arena.Allocate(token_count);
		memset(row.present, 0, token_count);
	}
	// keep the first stored result, matching the previous insert-if-absent semantics
	if (row.present[token_index]) {
		return;
	}
	row.entries[token_index] = entry;
	row.present[token_index] = 1;
}

} // namespace duckdb
