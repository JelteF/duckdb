#include "duckdb/parser/peg/parser_packrat.hpp"

#include "duckdb/parser/peg/matcher.hpp"

#include <cstring>

namespace duckdb {

ParserPackratCache::ParserPackratCache(idx_t base_token_index_p, idx_t token_count_p, idx_t matcher_count)
    : rows(matcher_count), base_token_index(base_token_index_p),
      token_count(token_count_p >= base_token_index_p ? token_count_p - base_token_index_p + 1 : 1) {
}

ParserPackratCache::~ParserPackratCache() = default;

void ParserPackratCache::PackratRow::Grow(idx_t required, idx_t maximum) {
	auto new_capacity = MaxValue<idx_t>(required, MaxValue<idx_t>(capacity * 4, INITIAL_ROW_CAPACITY));
	new_capacity = MinValue<idx_t>(new_capacity, maximum);
	auto new_entries = make_unsafe_uniq_array_uninitialized<data_t>(new_capacity * sizeof(ParserPackratEntry));
	auto new_present = make_unsafe_uniq_array<uint8_t>(new_capacity);
	if (capacity > 0) {
		memcpy(new_entries.get(), entries.get(), capacity * sizeof(ParserPackratEntry));
		memcpy(new_present.get(), present.get(), capacity);
	}
	entries = std::move(new_entries);
	present = std::move(new_present);
	capacity = new_capacity;
}

optional_ptr<const ParserPackratEntry> ParserPackratCache::Lookup(const Matcher &matcher, idx_t token_index) const {
	D_ASSERT(matcher.IsPackratMemoized());
	auto matcher_id = matcher.GetPackratId().GetIndex();
	D_ASSERT(matcher_id < rows.size());
	auto row_index = RowIndex(token_index);
	auto &row = rows[matcher_id];
	if (row_index >= row.capacity || !row.present[row_index]) {
		return nullptr;
	}
	return optional_ptr<const ParserPackratEntry>(row.Entries() + row_index);
}

void ParserPackratCache::Store(const Matcher &matcher, idx_t token_index, ParserPackratEntry entry) {
	D_ASSERT(matcher.IsPackratMemoized());
	auto matcher_id = matcher.GetPackratId().GetIndex();
	D_ASSERT(matcher_id < rows.size());
	auto row_index = RowIndex(token_index);
	if (row_index >= token_count) {
		return;
	}
	auto &row = rows[matcher_id];
	if (row_index >= row.capacity) {
		row.Grow(row_index + 1, token_count);
	}
	// keep the first stored result, matching the previous insert-if-absent semantics
	if (row.present[row_index]) {
		return;
	}
	row.Entries()[row_index] = entry;
	row.present[row_index] = 1;
}

} // namespace duckdb
