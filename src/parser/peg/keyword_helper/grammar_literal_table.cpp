#include "duckdb/parser/peg/grammar_literal_table.hpp"

#include "duckdb/common/atomic.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/parser/peg/parsed_grammar.hpp"

namespace duckdb {

static uint64_t NextLiteralTableId() {
	static atomic<uint64_t> next_id {1};
	auto result = next_id.fetch_add(1, std::memory_order_relaxed);
	D_ASSERT(result != 0);
	return result;
}

GrammarLiteralTable::GrammarLiteralTable(const ParsedGrammar &grammar,
                                         const case_insensitive_map_t<LiteralInfo> &keywords)
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
	for (auto &entry : keywords) {
		Register(entry.first, entry.second.CategoryFlags());
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

void GrammarLiteralTable::Register(const string &text, keyword_categories_t categories) {
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
	entries[slot].info.AddCategories(categories);
}

} // namespace duckdb
