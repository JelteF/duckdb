#include "duckdb/parser/peg/matcher.hpp"
#include "duckdb/parser/peg/matcher/choice_matcher.hpp"
#include "duckdb/parser/peg/matcher/literal_choice_matcher.hpp"
#include "duckdb/parser/peg/matcher/list_matcher.hpp"
#include "duckdb/parser/peg/matcher/optional_matcher.hpp"
#include "duckdb/parser/peg/matcher/repeat_matcher.hpp"

namespace duckdb {

//! Collects the children of a list or repeat while it is matching. The buffer lives in the process arena, which is
//! released as a whole when the match run ends, so a list that fails half-way costs nothing to unwind. Only the
//! children of a successful match are copied into the longer-lived parse result arena.
class ChildCollector {
public:
	ChildCollector(MatchState &state, idx_t initial_capacity) : arena(state.context.process_allocator) {
		Reserve(initial_capacity);
	}

	void Add(ParseResult &result) {
		if (count == capacity) {
			Reserve(capacity == 0 ? 4 : capacity * 2);
		}
		new (children + count) reference<ParseResult>(result);
		count++;
	}

	idx_t Size() const {
		return count;
	}

	//! The children collected so far, valid until the next Add
	ParseResultChildren Children() const {
		return ParseResultChildren(children, count);
	}

	ParseResultChildren Finalize(MatchState &state) const {
		return state.context.allocator.MakeChildren(children, count);
	}

private:
	void Reserve(idx_t new_capacity) {
		if (new_capacity <= capacity) {
			return;
		}
		arena.AlignNext();
		auto target =
		    reinterpret_cast<reference<ParseResult> *>(arena.Allocate(new_capacity * sizeof(reference<ParseResult>)));
		if (count > 0) {
			memcpy(static_cast<void *>(target), static_cast<const void *>(children),
			       count * sizeof(reference<ParseResult>));
		}
		children = target;
		capacity = new_capacity;
	}

private:
	ArenaAllocator &arena;
	reference<ParseResult> *children = nullptr;
	idx_t count = 0;
	idx_t capacity = 0;
};

MatchStep MatchStep::Child(MatchInput input) {
	return MatchStep(input, nullopt);
}

MatchStep MatchStep::Complete(MatcherResult result) {
	return MatchStep(nullopt, result);
}

optional<MatchInput> MatchStep::GetChild() {
	return child;
}

MatcherResult MatchStep::GetResult() const {
	D_ASSERT(!child);
	D_ASSERT(result);
	return result.value();
}

class AtomicMatchProcess : public MatchProcess {
public:
	AtomicMatchProcess(const AtomicMatcher &matcher_p, MatchState &state_p) : matcher(matcher_p), state(state_p) {
	}

	MatchStep Resume(optional<MatcherResult> child_result) override {
		D_ASSERT(!child_result);
		D_ASSERT(!completed);
		completed = true;
		return MatchStep::Complete(matcher.MatchAtomic(state));
	}

private:
	const AtomicMatcher &matcher;
	MatchState &state;
	bool completed = false;
};

arena_ptr<MatchProcess> AtomicMatcher::StartMatch(MatchState &state) const {
	return state.Make<AtomicMatchProcess>(*this, state);
}

class ListMatchProcess : public MatchProcess {
public:
	ListMatchProcess(const ListMatcher &matcher_p, MatchState &state_p)
	    : matcher(matcher_p), state(state_p), list_state(state_p), results(state_p, matcher_p.matchers.size()) {
		saved_suggestion_size = matcher.suppress_suggestions ? list_state.context.suggestions.size() : 0;
		if (auto current = list_state.token_iterator.Current()) {
			start_offset = optional_idx(current->offset);
		}
	}

	MatchStep Resume(optional<MatcherResult> child_result) override {
		D_ASSERT(awaiting_child == child_result.has_value());
		if (child_result) {
			awaiting_child = false;
			if (!child_result->IsSuccess()) {
				DiscardSuggestions();
				return MatchStep::Complete(MatcherResult::Failure());
			}
			if (child_result->HasParseResult()) {
				results.Add(*child_result->GetParseResult());
			}
			child_index++;
		}
		while (child_index < matcher.matchers.size()) {
			auto &child_matcher = matcher.matchers[child_index].get();
			auto current = list_state.token_iterator.Current();
			bool at_autocomplete_cursor = current && current->type == TokenType::END_OF_INPUT_AUTOCOMPLETE;
			if (!at_autocomplete_cursor) {
				awaiting_child = true;
				return MatchStep::Child({child_matcher, list_state});
			}
			if (matcher.suppress_suggestions) {
				DiscardSuggestions();
				return MatchStep::Complete(MatcherResult::Failure());
			}
			if (child_matcher.AddSuggestion(list_state) == SuggestionType::OPTIONAL) {
				child_index++;
				continue;
			}
			state.token_iterator.SetPosition(list_state.token_iterator);
			return MatchStep::Complete(MatcherResult::Failure());
		}
		state.token_iterator.SetPosition(list_state.token_iterator);
		DiscardSuggestions();
		if (matcher.IsCollapsible()) {
			auto passthrough = FindPassthroughResult();
			if (passthrough) {
				passthrough->collapsed = true;
				return MatchStep::Complete(MatcherResult::Success(passthrough));
			}
		}
		auto list_name = matcher.HasName() ? &matcher.GetNameRef() : nullptr;
		return MatchStep::Complete(
		    state.AllocateParseResult<ListParseResult>(results.Finalize(state), list_name, start_offset));
	}

private:
	//! For a collapsible rule: the single child that produced a result, provided every other child is an optional
	//! that matched nothing. Returns nullptr when the list cannot be collapsed.
	optional_ptr<ParseResult> FindPassthroughResult() const {
		optional_ptr<ParseResult> passthrough;
		for (auto &child : results.Children()) {
			auto &child_result = child.get();
			if (child_result.type == ParseResultType::OPTIONAL &&
			    !child_result.Cast<OptionalParseResult>().HasResult()) {
				continue;
			}
			if (passthrough) {
				return nullptr;
			}
			passthrough = child_result;
		}
		return passthrough;
	}

	void DiscardSuggestions() {
		if (!matcher.suppress_suggestions) {
			return;
		}
		list_state.context.suggestions.erase(list_state.context.suggestions.begin() +
		                                         NumericCast<int64_t>(saved_suggestion_size),
		                                     list_state.context.suggestions.end());
	}

private:
	const ListMatcher &matcher;
	MatchState &state;
	MatchState list_state;
	ChildCollector results;
	idx_t child_index = 0;
	idx_t saved_suggestion_size = 0;
	optional_idx start_offset;
	bool awaiting_child = false;
};

arena_ptr<MatchProcess> ListMatcher::StartMatch(MatchState &state) const {
	return state.Make<ListMatchProcess>(*this, state);
}

template <bool SINGLE_CHILD>
class ChoiceMatchProcess : public MatchProcess {
public:
	ChoiceMatchProcess(const ChoiceMatcher &matcher_p, MatchState &state_p, idx_t child_index_p = 0)
	    : matcher(matcher_p), state(state_p), child_index(child_index_p) {
		if (auto current = state.token_iterator.Current()) {
			start_offset = optional_idx(current->offset);
		}
	}

	MatchStep Resume(optional<MatcherResult> child_result) override {
		D_ASSERT(awaiting_child == child_result.has_value());
		if (child_result) {
			awaiting_child = false;
			D_ASSERT(child_state);
			if (child_result->IsSuccess()) {
				state.token_iterator.SetPosition(child_state.value().token_iterator);
				if (!child_result->HasParseResult()) {
					return MatchStep::Complete(MatcherResult::Success());
				}
				return MatchStep::Complete(state.AllocateParseResult<ChoiceParseResult>(*child_result->GetParseResult(),
				                                                                        child_index, start_offset));
			}
			if (SINGLE_CHILD) {
				return MatchStep::Complete(MatcherResult::Failure());
			}
			child_index++;
			child_state.reset();
		}
		if (!SINGLE_CHILD) {
			while (child_index < matcher.matchers.size() && !matcher.matchers[child_index].get().MayMatchHere(state)) {
				child_index++;
			}
		}
		if (child_index >= matcher.matchers.size()) {
			return MatchStep::Complete(MatcherResult::Failure());
		}
		child_state.emplace(state);
		awaiting_child = true;
		return MatchStep::Child({matcher.matchers[child_index].get(), *child_state});
	}

private:
	const ChoiceMatcher &matcher;
	MatchState &state;
	optional<MatchState> child_state;
	idx_t child_index = 0;
	optional_idx start_offset;
	bool awaiting_child = false;
};

arena_ptr<MatchProcess> ChoiceMatcher::StartMatch(MatchState &state) const {
	return state.Make<ChoiceMatchProcess<false>>(*this, state);
}

arena_ptr<MatchProcess> LiteralChoiceMatcher::StartMatch(MatchState &state) const {
	auto literal = state.token_iterator.CurrentLiteralInfo(table);
	auto entry = literal_children.find(literal.LiteralId());
	auto child_index = entry == literal_children.end() ? matchers.size() : entry->second;
	return state.Make<ChoiceMatchProcess<true>>(*this, state, child_index);
}

class OptionalMatchProcess : public MatchProcess {
public:
	OptionalMatchProcess(const OptionalMatcher &matcher_p, MatchState &state_p)
	    : matcher(matcher_p), state(state_p), child_state(state_p) {
		if (auto current = child_state.token_iterator.Current()) {
			start_offset = optional_idx(current->offset);
		}
	}

	MatchStep Resume(optional<MatcherResult> child_result) override {
		D_ASSERT(awaiting_child == child_result.has_value());
		if (!child_result) {
			if (!matcher.GetChildMatcher().MayMatchHere(child_state)) {
				return MatchStep::Complete(state.AllocateParseResult<OptionalParseResult>());
			}
			awaiting_child = true;
			return MatchStep::Child({matcher.GetChildMatcher(), child_state});
		}
		awaiting_child = false;
		if (!child_result->IsSuccess()) {
			return MatchStep::Complete(state.AllocateParseResult<OptionalParseResult>());
		}
		state.token_iterator.SetPosition(child_state.token_iterator);
		if (!child_result->HasParseResult()) {
			return MatchStep::Complete(MatcherResult::Success());
		}
		return MatchStep::Complete(
		    state.AllocateParseResult<OptionalParseResult>(child_result->GetParseResult(), start_offset));
	}

private:
	const OptionalMatcher &matcher;
	MatchState &state;
	MatchState child_state;
	optional_idx start_offset;
	bool awaiting_child = false;
};

arena_ptr<MatchProcess> OptionalMatcher::StartMatch(MatchState &state) const {
	return state.Make<OptionalMatchProcess>(*this, state);
}

class RepeatMatchProcess : public MatchProcess {
public:
	RepeatMatchProcess(const RepeatMatcher &matcher_p, MatchState &state_p)
	    : matcher(matcher_p), state(state_p), repeat_state(state_p), results(state_p, 0) {
		if (auto current = repeat_state.token_iterator.Current()) {
			start_offset = optional_idx(current->offset);
		}
	}

	MatchStep Resume(optional<MatcherResult> child_result) override {
		D_ASSERT(awaiting_child == child_result.has_value());
		if (child_result) {
			awaiting_child = false;
			if (!child_result->IsSuccess()) {
				if (!matched_once) {
					return MatchStep::Complete(MatcherResult::Failure());
				}
				return MatchStep::Complete(CreateResult());
			}
			matched_once = true;
			if (child_result->HasParseResult()) {
				results.Add(*child_result->GetParseResult());
			}
			state.token_iterator.SetPosition(repeat_state.token_iterator);
			auto current = repeat_state.token_iterator.Current();
			if (current && current->type == TokenType::END_OF_INPUT_AUTOCOMPLETE) {
				matcher.GetChildMatcher().AddSuggestion(state);
				return MatchStep::Complete(CreateResult());
			}
		}
		if (!matcher.GetChildMatcher().MayMatchHere(repeat_state)) {
			if (!matched_once) {
				return MatchStep::Complete(MatcherResult::Failure());
			}
			return MatchStep::Complete(CreateResult());
		}
		awaiting_child = true;
		return MatchStep::Child({matcher.GetChildMatcher(), repeat_state});
	}

private:
	MatcherResult CreateResult() {
		return state.AllocateParseResult<RepeatParseResult>(results.Finalize(state), start_offset);
	}

private:
	const RepeatMatcher &matcher;
	MatchState &state;
	MatchState repeat_state;
	ChildCollector results;
	bool matched_once = false;
	optional_idx start_offset;
	bool awaiting_child = false;
};

arena_ptr<MatchProcess> RepeatMatcher::StartMatch(MatchState &state) const {
	return state.Make<RepeatMatchProcess>(*this, state);
}

} // namespace duckdb
