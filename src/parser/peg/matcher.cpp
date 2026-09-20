#include "duckdb/parser/peg/matcher.hpp"

#include <algorithm>
#include "duckdb/parser/peg/matcher/repeat_matcher.hpp"
#include "duckdb/parser/peg/matcher/optional_matcher.hpp"
#include "duckdb/parser/peg/matcher/list_matcher.hpp"
#include "duckdb/parser/peg/matcher/precedence_ladder.hpp"
#include "duckdb/parser/peg/matcher/choice_matcher.hpp"
#include "duckdb/parser/peg/matcher/keyword_matcher.hpp"
#include "duckdb/parser/peg/matcher_stack.hpp"
#include "duckdb/parser/peg/compiled_grammar.hpp"
#include "duckdb/parser/peg/matcher_factory.hpp"
#include "duckdb/main/database.hpp"
#include "duckdb/parser/peg/transformer/peg_transformer.hpp"

#include "duckdb/common/printer.hpp"
#include "duckdb/common/optional.hpp"
#include "duckdb/common/string_map_set.hpp"
#include "duckdb/common/types/string_type.hpp"
#include "duckdb/parser/peg/keyword_helper.hpp"
#include "duckdb/common/case_insensitive_map.hpp"
#include "duckdb/common/exception/parser_exception.hpp"
#include "duckdb/parser/peg/tokenizer/tokenizer.hpp"
#include "duckdb/parser/peg/peg_parser.hpp"
#include "duckdb/parser/peg/transformer/parse_result.hpp"

namespace duckdb {

MatcherResult Matcher::MatchParseResult(MatchState &state) const {
	MatchInput input {*this, state};
	MatchStack stack;
	return stack.Execute(input);
}

SuggestionType Matcher::AddSuggestion(MatchState &state) const {
	if (!state.added_suggestions) {
		state.added_suggestions = make_uniq<reference_set_t<const Matcher>>();
	}
	auto &added_suggestions = *state.added_suggestions;
	auto entry = added_suggestions.find(*this);
	if (entry != added_suggestions.end()) {
		return SuggestionType::MANDATORY;
	}
	added_suggestions.insert(*this);
	return AddSuggestionInternal(state);
}

string Matcher::GetName() const {
	if (name.empty()) {
		return ToString();
	}
	return name;
}

void Matcher::Print() const {
	Printer::Print(ToString());
}

optional_ptr<ParseResult> MatchContext::EmptyOptionalResult() {
	if (!empty_optional) {
		empty_optional = allocator.Make<OptionalParseResult>();
	}
	return empty_optional;
}

void MatchState::AddSuggestion(MatcherSuggestion suggestion) {
	context.suggestions.push_back(std::move(suggestion));
}

bool Matcher::MatchesPredicateLeader(MatchState &state, const MatcherStartSet &set) const {
	for (idx_t i = 0; i < set.leader_count; i++) {
		if (set.leaders[i].get().CanStartWith(state, 0)) {
			return true;
		}
	}
	return false;
}

namespace {

//! Computes MatcherStartSet for a matcher graph. Rule references make the graph cyclic; a matcher reached again while
//! its own set is still being computed contributes "anything", which is conservative.
class StartSetBuilder {
public:
	struct Entry {
		unique_ptr<MatcherStartSet> set;
		//! Literal ids as a bitmap, so that merging up the graph is a word-wise OR. The finished set keeps it.
		vector<uint64_t> literal_bits;
		//! Collected while building; ComputeStartSets moves them into the allocator's shared buffer
		vector<reference<const Matcher>> predicate_leaders;
		bool nullable = false;
		bool in_progress = false;
		//! Matching this consumes exactly one token, whichever way it matches
		bool single_token = false;
		//! See Matcher::second_literal_id
		uint16_t second_literal = 0;
		optional_ptr<const GrammarLiteralTable> second_literal_table;
	};

	Entry &Compute(const Matcher &matcher) {
		auto &entry = entries[&matcher];
		if (entry.set) {
			return entry;
		}
		if (entry.in_progress) {
			return cycle_entry;
		}
		entry.in_progress = true;
		auto set = make_uniq<MatcherStartSet>();
		bool nullable = false;
		bool single_token = false;
		switch (matcher.Type()) {
		case MatcherType::KEYWORD: {
			auto &keyword = matcher.Cast<KeywordMatcher>();
			if (keyword.LiteralId()) {
				set->literal_table = keyword.GetLiteralTable();
				SetBit(entry.literal_bits, keyword.LiteralId());
			} else {
				entry.predicate_leaders.push_back(matcher);
			}
			single_token = true;
			break;
		}
		case MatcherType::CHOICE: {
			auto &alternatives = matcher.Cast<ChoiceMatcher>().matchers;
			// only when every alternative agrees: the choice matches if any one of them does
			single_token = !alternatives.empty();
			bool first = true;
			for (auto &child : alternatives) {
				auto &child_entry = Compute(child.get());
				Merge(*set, entry, child_entry);
				nullable = nullable || child_entry.nullable;
				single_token = single_token && child_entry.single_token;
				if (first) {
					entry.second_literal = child_entry.second_literal;
					entry.second_literal_table = child_entry.second_literal_table;
					first = false;
				} else if (entry.second_literal != child_entry.second_literal) {
					entry.second_literal = 0;
				}
			}
			break;
		}
		case MatcherType::LIST: {
			auto &elements = matcher.Cast<ListMatcher>().matchers;
			nullable = true;
			for (auto &child : elements) {
				auto &child_entry = Compute(child.get());
				Merge(*set, entry, child_entry);
				if (!child_entry.nullable) {
					nullable = false;
					break;
				}
			}
			if (elements.size() == 1) {
				// a rule body is a list of one, and passes both properties through
				auto &only = Compute(elements[0].get());
				single_token = only.single_token;
				entry.second_literal = only.second_literal;
				entry.second_literal_table = only.second_literal_table;
			} else if (elements.size() > 1) {
				auto &lead = Compute(elements[0].get());
				if (lead.single_token) {
					// the second element starts at the second token, so a literal it must be is one too
					SetSoleLiteral(entry, Compute(elements[1].get()));
				} else if (!lead.nullable) {
					// the list starts with the lead, so the lead's own second token is the list's
					entry.second_literal = lead.second_literal;
					entry.second_literal_table = lead.second_literal_table;
				}
			}
			break;
		}
		case MatcherType::OPTIONAL:
			Merge(*set, entry, Compute(matcher.Cast<OptionalMatcher>().GetChildMatcher()));
			nullable = true;
			break;
		case MatcherType::REPEAT: {
			auto &child_entry = Compute(matcher.Cast<RepeatMatcher>().GetChildMatcher());
			Merge(*set, entry, child_entry);
			nullable = child_entry.nullable;
			break;
		}
		case MatcherType::VARIABLE:
		case MatcherType::OPERATOR:
		case MatcherType::NUMBER_LITERAL:
			// these answer CanStartWith from the token itself
			entry.predicate_leaders.push_back(matcher);
			single_token = true;
			break;
		case MatcherType::STRING_LITERAL:
		case MatcherType::END_OF_INPUT:
			// a string literal can be a sequence of adjacent ones, and end of input consumes nothing
			entry.predicate_leaders.push_back(matcher);
			break;
		default:
			set->any = true;
			break;
		}
		// copied, not moved: the entry is memoized and later parents still merge its bits
		entry.set = std::move(set);
		entry.nullable = nullable;
		entry.single_token = single_token;
		entry.in_progress = false;
		return entry;
	}

	unique_ptr<MatcherStartSet> Take(const Matcher &matcher, bool &nullable, vector<uint64_t> &literal_bits,
	                                 vector<reference<const Matcher>> &leaders, uint16_t &second_literal,
	                                 optional_ptr<const GrammarLiteralTable> &second_literal_table) {
		auto &entry = Compute(matcher);
		nullable = entry.nullable;
		second_literal = entry.second_literal;
		second_literal_table = entry.second_literal_table;
		literal_bits = entry.literal_bits;
		leaders = entry.predicate_leaders;
		return std::move(entry.set);
	}

private:
	static void SetBit(vector<uint64_t> &bits, uint16_t literal_id) {
		idx_t word = literal_id / 64;
		if (word >= bits.size()) {
			bits.resize(word + 1, 0);
		}
		bits[word] |= uint64_t(1) << (literal_id % 64);
	}

	//! A matcher that can only match one literal, and has to match: that literal is then known at its position
	static void SetSoleLiteral(Entry &target, const Entry &source) {
		if (source.set->any || source.nullable || !source.predicate_leaders.empty()) {
			return;
		}
		uint16_t only = 0;
		for (idx_t word = 0; word < source.literal_bits.size(); word++) {
			auto bits = source.literal_bits[word];
			while (bits) {
				if (only != 0) {
					return;
				}
				only = NumericCast<uint16_t>(word * 64 + CountZeros<uint64_t>::Trailing(bits));
				bits &= bits - 1;
			}
		}
		target.second_literal = only;
		target.second_literal_table = source.set->literal_table;
	}

	static void Merge(MatcherStartSet &target, Entry &target_entry, const Entry &source) {
		auto &set = *source.set;
		if (set.any) {
			target.any = true;
		}
		if (!source.literal_bits.empty()) {
			target.literal_table = set.literal_table;
			if (target_entry.literal_bits.size() < source.literal_bits.size()) {
				target_entry.literal_bits.resize(source.literal_bits.size(), 0);
			}
			for (idx_t word = 0; word < source.literal_bits.size(); word++) {
				target_entry.literal_bits[word] |= source.literal_bits[word];
			}
		}
		for (auto &leader : source.predicate_leaders) {
			bool present = false;
			for (auto &existing : target_entry.predicate_leaders) {
				if (&existing.get() == &leader.get()) {
					present = true;
					break;
				}
			}
			if (!present) {
				target_entry.predicate_leaders.push_back(leader);
			}
		}
	}

	StartSetBuilder() {
		cycle_entry.set = make_uniq<MatcherStartSet>();
		cycle_entry.set->any = true;
	}
	friend class duckdb::MatcherAllocator;

	unordered_map<const Matcher *, Entry> entries;
	Entry cycle_entry;
};

} // namespace

void MatcherAllocator::ComputeStartSets() {
	StartSetBuilder builder;
	// compute everything first: Take moves the set out, and a matcher's set must stay available while the matchers
	// that reference it are still being computed
	for (auto &matcher : matchers) {
		builder.Compute(*matcher);
	}
	// the sets go into one contiguous array and their bitmaps into one contiguous buffer, so that reaching either
	// from a matcher is a load rather than a chase into a separately allocated node
	start_sets.resize(matchers.size());
	vector<idx_t> bit_offsets(matchers.size());
	vector<idx_t> bit_counts(matchers.size());
	vector<idx_t> leader_offsets(matchers.size());
	vector<idx_t> leader_counts(matchers.size());
	for (idx_t i = 0; i < matchers.size(); i++) {
		vector<uint64_t> literal_bits;
		vector<reference<const Matcher>> leaders;
		auto set = builder.Take(*matchers[i], matchers[i]->nullable, literal_bits, leaders,
		                        matchers[i]->second_literal_id, matchers[i]->second_literal_table);
		D_ASSERT(matchers[i]->second_literal_id == 0 || matchers[i]->second_literal_table);
		start_sets[i] = *set;
		bit_offsets[i] = start_set_bits.size();
		bit_counts[i] = literal_bits.size();
		start_set_bits.insert(start_set_bits.end(), literal_bits.begin(), literal_bits.end());
		leader_offsets[i] = start_set_leaders.size();
		leader_counts[i] = leaders.size();
		start_set_leaders.insert(start_set_leaders.end(), leaders.begin(), leaders.end());
	}
	// only now is the buffer final, so only now can the spans into it be resolved
	for (idx_t i = 0; i < matchers.size(); i++) {
		auto &set = start_sets[i];
		set.literal_words = start_set_bits.data() + bit_offsets[i];
		set.literal_word_count = NumericCast<uint32_t>(bit_counts[i]);
		set.leaders = start_set_leaders.data() + leader_offsets[i];
		set.leader_count = NumericCast<uint32_t>(leader_counts[i]);
		matchers[i]->start_set = set;
		matchers[i]->start_set_computed = true;
	}
}

PrecedenceLadder &MatcherAllocator::AddLadder(unique_ptr<PrecedenceLadder> ladder) {
	auto &result = *ladder;
	ladders.push_back(std::move(ladder));
	return result;
}

Matcher &MatcherAllocator::Allocate(unique_ptr<Matcher> matcher) {
	auto &result = *matcher;
	matchers.push_back(std::move(matcher));
	return result;
}

ParseResultAllocator::ParseResultAllocator() : arena(Allocator::DefaultAllocator()) {
}

ParseResultAllocator::~ParseResultAllocator() = default;

} // namespace duckdb
