#include "duckdb/parser/peg/grammar_literal_table.hpp"

#include "duckdb/common/atomic.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/parser/peg/keyword_helper/default_keyword_maps.hpp"
#include "duckdb/parser/peg/parsed_grammar.hpp"

namespace duckdb {

static uint64_t NextLiteralTableId() {
	static atomic<uint64_t> next_id {1};
	auto result = next_id.fetch_add(1, std::memory_order_relaxed);
	D_ASSERT(result != 0);
	return result;
}

GrammarLiteralTable::GrammarLiteralTable(const ParsedGrammar &grammar, const DefaultKeywordMaps &keyword_maps)
    : cache_id(NextLiteralTableId()) {
	vector<reference<const PEGExpression>> pending;
	for (auto &entry : grammar.rules) {
		pending.push_back(entry.second->recipe.expression);
	}
	while (!pending.empty()) {
		auto &expression = pending.back().get();
		pending.pop_back();
		if (expression.type == PEGExpression::Type::LITERAL) {
			Register(expression.text.GetString());
		}
		for (auto &child : expression.children) {
			pending.push_back(child);
		}
	}
	RegisterCategory(keyword_maps.reserved_keyword_map, PEGKeywordCategory::KEYWORD_RESERVED);
	RegisterCategory(keyword_maps.unreserved_keyword_map, PEGKeywordCategory::KEYWORD_UNRESERVED);
	RegisterCategory(keyword_maps.colname_keyword_map, PEGKeywordCategory::KEYWORD_COL_NAME);
	RegisterCategory(keyword_maps.typefunc_keyword_map, PEGKeywordCategory::KEYWORD_TYPE_FUNC);
	RegisterCategory(keyword_maps.typename_keyword_map, PEGKeywordCategory::KEYWORD_TYPE_NAME);
}

void GrammarLiteralTable::RegisterCategory(const case_insensitive_set_t &words, PEGKeywordCategory category) {
	for (auto &word : words) {
		Register(word, category);
	}
}

idx_t GrammarLiteralTable::FindSlot(const char *data, idx_t size) const {
	const auto mask = entries.size() - 1;
	auto slot = static_cast<idx_t>(StringUtil::CIHash(data, size)) & mask;
	while (entries[slot].info.LiteralId()) {
		auto &entry = entries[slot];
		if (entry.size == size && StringUtil::CIEquals(entry.data, entry.size, data, size)) {
			break;
		}
		slot = (slot + 1) & mask;
	}
	return slot;
}

void GrammarLiteralTable::Grow() {
	vector<LiteralEntry> old_entries;
	old_entries.swap(entries);
	entries.resize(old_entries.empty() ? INITIAL_CAPACITY : old_entries.size() * 2);
	for (auto &entry : old_entries) {
		if (!entry.info.LiteralId()) {
			continue;
		}
		entries[FindSlot(entry.data, entry.size)] = entry;
	}
}

void GrammarLiteralTable::Register(const string &text, PEGKeywordCategory category) {
	if ((keys.size() + 1) * 2 > entries.size()) {
		Grow();
	}
	auto slot = FindSlot(text.c_str(), text.size());
	if (!entries[slot].info.LiteralId()) {
		if (keys.size() >= LiteralInfo::MAX_LITERAL_ID) {
			throw InvalidInputException("Grammar has too many distinct literals");
		}
		keys.push_back(make_uniq<string>(text));
		auto &key = *keys.back();
		entries[slot] = LiteralEntry {key.c_str(), static_cast<uint32_t>(key.size()),
		                              LiteralInfo(static_cast<uint16_t>(keys.size()))};
	}
	entries[slot].info.AddCategory(category);
}

} // namespace duckdb
