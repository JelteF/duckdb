//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/peg/grammar_literal_table.hpp
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/case_insensitive_map.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/parser/peg/literal_info.hpp"

#include <string_view>

namespace duckdb {

class ParsedGrammar;

//! Immutable after construction, including literals only present in keyword-category rules.
class GrammarLiteralTable {
public:
	DUCKDB_API GrammarLiteralTable(const ParsedGrammar &grammar, const case_insensitive_map_t<LiteralInfo> &keywords);
	GrammarLiteralTable(const GrammarLiteralTable &) = delete;
	GrammarLiteralTable &operator=(const GrammarLiteralTable &) = delete;

	uint64_t CacheId() const {
		return cache_id;
	}

	LiteralInfo Lookup(const char *data, idx_t size) const {
		if (entries.empty()) {
			return LiteralInfo();
		}
		const auto mask = entries.size() - 1;
		auto slot = static_cast<idx_t>(StringUtil::CIHash(data, size)) & mask;
		while (true) {
			auto &entry = entries[slot];
			if (!entry.info.LiteralId()) {
				return LiteralInfo();
			}
			if (entry.size == size && StringUtil::CIEquals(entry.data, entry.size, data, size)) {
				return entry.info;
			}
			slot = (slot + 1) & mask;
		}
	}
	LiteralInfo Lookup(std::string_view text) const {
		return Lookup(text.data(), text.size());
	}

private:
	struct LiteralEntry {
		const char *data = nullptr;
		uint32_t size = 0;
		LiteralInfo info;
	};
	static constexpr idx_t INITIAL_CAPACITY = 1024;

private:
	void Register(const string &text, keyword_categories_t categories = keyword_categories_t());
	idx_t FindSlot(const char *data, idx_t size) const;
	void Grow();

private:
	const uint64_t cache_id;
	vector<LiteralEntry> entries;
	vector<unique_ptr<string>> keys;
};

} // namespace duckdb
