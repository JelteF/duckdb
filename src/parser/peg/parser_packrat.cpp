#include "duckdb/parser/peg/parser_packrat.hpp"

#include "duckdb/parser/peg/matcher.hpp"

namespace duckdb {

ParserPackratCache::ParserPackratCache(idx_t token_count_p, idx_t matcher_count)
    : rows(matcher_count), present(matcher_count), token_count(token_count_p + 1) {
}

ParserPackratCache::~ParserPackratCache() = default;

optional_ptr<const ParserPackratEntry> ParserPackratCache::Lookup(const Matcher &matcher, idx_t token_index) const {
	D_ASSERT(matcher.IsPackratMemoized());
	auto matcher_id = matcher.GetPackratId().GetIndex();
	D_ASSERT(matcher_id < rows.size());
	auto &row = rows[matcher_id];
	if (row.empty() || token_index >= token_count || !present[matcher_id][token_index]) {
		return nullptr;
	}
	return optional_ptr<const ParserPackratEntry>(&row[token_index]);
}

void ParserPackratCache::Store(const Matcher &matcher, idx_t token_index, ParserPackratEntry entry) {
	D_ASSERT(matcher.IsPackratMemoized());
	auto matcher_id = matcher.GetPackratId().GetIndex();
	D_ASSERT(matcher_id < rows.size());
	if (token_index >= token_count) {
		return;
	}
	auto &row = rows[matcher_id];
	auto &row_present = present[matcher_id];
	if (row.empty()) {
		row.resize(token_count);
		row_present.resize(token_count, 0);
	}
	// keep the first stored result, matching the previous insert-if-absent semantics
	if (row_present[token_index]) {
		return;
	}
	row[token_index] = entry;
	row_present[token_index] = 1;
}

} // namespace duckdb
