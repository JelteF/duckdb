#include "duckdb/parser/peg/matcher.hpp"

#include <algorithm>
#include "duckdb/parser/peg/matcher/repeat_matcher.hpp"
#include "duckdb/parser/peg/matcher/optional_matcher.hpp"
#include "duckdb/parser/peg/matcher/list_matcher.hpp"
#include "duckdb/parser/peg/matcher/choice_matcher.hpp"
#include "duckdb/parser/peg/matcher/keyword_matcher.hpp"
#include "duckdb/parser/peg/matcher_stack.hpp"
#include "duckdb/parser/peg/compiled_grammar.hpp"
#include "duckdb/parser/peg/matcher_factory.hpp"
#include "duckdb/main/database.hpp"
#include "duckdb/parser/peg/transformer/peg_transformer.hpp"

#include "duckdb/common/bit_utils.hpp"
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

void MatchState::AddSuggestion(MatcherSuggestion suggestion) {
	context.suggestions.push_back(std::move(suggestion));
}

bool Matcher::MayMatchHere(MatchState &state) const {
	auto token = state.token_iterator.Current();
	if (!token || token->type == TokenType::END_OF_INPUT_AUTOCOMPLETE) {
		return true;
	}
	if (!start_set) {
		return CanStartWith(state);
	}
	auto &set = *start_set;
	if (set.any || set.nullable) {
		return true;
	}
	if (!set.literal_ids.empty()) {
		auto literal_id = state.token_iterator.CurrentLiteralInfo(*set.literal_table).LiteralId();
		if (literal_id && (set.literal_signature & MatcherStartSet::SignatureBit(literal_id)) &&
		    std::binary_search(set.literal_ids.begin(), set.literal_ids.end(), literal_id)) {
			return true;
		}
	}
	for (auto &leader : set.predicate_leaders) {
		if (leader.get().CanStartWith(state)) {
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
		//! Literal ids as a bitmap while building, so that merging up the graph is a word-wise OR rather than a
		//! sort of ever larger id lists
		vector<uint64_t> literal_bits;
		bool nullable = false;
		bool in_progress = false;
	};

	explicit StartSetBuilder(const GrammarLiteralTable &literal_table) : literal_table(literal_table) {
		cycle_entry.set = make_uniq<MatcherStartSet>();
		cycle_entry.set->any = true;
	}

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
		switch (matcher.Type()) {
		case MatcherType::KEYWORD: {
			// a custom keyword matcher answers with no dispatch literal, because it may match more than its
			// own spelling - it has to be tried whatever the token is
			auto literal = matcher.Cast<KeywordMatcher>().GetDispatchLiteral(literal_table);
			if (literal.IsValid()) {
				set->literal_table = literal_table;
				SetBit(entry.literal_bits, NumericCast<uint16_t>(literal.GetIndex()));
			} else {
				set->any = true;
			}
			break;
		}
		case MatcherType::CHOICE:
			for (auto &child : matcher.Cast<ChoiceMatcher>().matchers) {
				auto &child_entry = Compute(child.get());
				Merge(*set, entry, child_entry);
				nullable = nullable || child_entry.nullable;
			}
			break;
		case MatcherType::LIST: {
			nullable = true;
			for (auto &child : matcher.Cast<ListMatcher>().matchers) {
				auto &child_entry = Compute(child.get());
				Merge(*set, entry, child_entry);
				if (!child_entry.nullable) {
					nullable = false;
					break;
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
		case MatcherType::STRING_LITERAL:
		case MatcherType::END_OF_INPUT:
			// these answer CanStartWith from the token itself
			set->predicate_leaders.push_back(matcher);
			break;
		default:
			set->any = true;
			break;
		}
		for (idx_t word = 0; word < entry.literal_bits.size(); word++) {
			auto bits = entry.literal_bits[word];
			while (bits) {
				auto bit = CountZeros<uint64_t>::Trailing(bits);
				auto literal_id = static_cast<uint16_t>(word * 64 + bit);
				set->literal_ids.push_back(literal_id);
				set->literal_signature |= MatcherStartSet::SignatureBit(literal_id);
				bits &= bits - 1;
			}
		}
		set->nullable = nullable;
		entry.set = std::move(set);
		entry.nullable = nullable;
		entry.in_progress = false;
		return entry;
	}

	unique_ptr<MatcherStartSet> Take(const Matcher &matcher) {
		return std::move(Compute(matcher).set);
	}

private:
	static void SetBit(vector<uint64_t> &bits, uint16_t literal_id) {
		idx_t word = literal_id / 64;
		if (word >= bits.size()) {
			bits.resize(word + 1, 0);
		}
		bits[word] |= uint64_t(1) << (literal_id % 64);
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
		for (auto &leader : set.predicate_leaders) {
			bool present = false;
			for (auto &existing : target.predicate_leaders) {
				if (&existing.get() == &leader.get()) {
					present = true;
					break;
				}
			}
			if (!present) {
				target.predicate_leaders.push_back(leader);
			}
		}
	}

	const GrammarLiteralTable &literal_table;
	unordered_map<const Matcher *, Entry> entries;
	Entry cycle_entry;
};

} // namespace

void MatcherAllocator::ComputeStartSets(const GrammarLiteralTable &literal_table) {
	StartSetBuilder builder(literal_table);
	// compute everything first: Take moves the set out, and a matcher's set must stay available while the matchers
	// that reference it are still being computed
	for (auto &matcher : matchers) {
		builder.Compute(*matcher);
	}
	for (auto &matcher : matchers) {
		matcher->start_set = builder.Take(*matcher);
	}
	start_sets_computed = true;
}

Matcher &MatcherAllocator::Allocate(unique_ptr<Matcher> matcher) {
	D_ASSERT(!start_sets_computed);
	auto &result = *matcher;
	result.packrat_id = optional_idx(matchers.size());
	matchers.push_back(std::move(matcher));
	return result;
}

optional_ptr<ParseResult> ParseResultAllocator::Allocate(unique_ptr<ParseResult> parse_result) {
	auto result_ptr = parse_result.get();
	parse_results.push_back(std::move(parse_result));
	return optional_ptr<ParseResult>(result_ptr);
}

} // namespace duckdb
