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
	//! Most rules have a handful of children, which fit in the collector itself
	static constexpr idx_t INLINE_CAPACITY = 6;

public:
	ChildCollector(MatchState &state, idx_t initial_capacity)
	    : arena(state.context.process_allocator), children(InlineChildren()), capacity(INLINE_CAPACITY) {
		Reserve(initial_capacity);
	}

	void Add(ParseResult &result) {
		if (count == capacity) {
			Reserve(capacity * 2);
		}
		new (children + count) reference<ParseResult>(result);
		count++;
	}

	idx_t Size() const {
		return count;
	}

	void Reset() {
		count = 0;
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
		auto target = reinterpret_cast<reference<ParseResult> *>(
		    arena.Allocate(new_capacity * sizeof(reference<ParseResult>)));
		if (count > 0) {
			memcpy(static_cast<void *>(target), static_cast<const void *>(children),
			       count * sizeof(reference<ParseResult>));
		}
		children = target;
		capacity = new_capacity;
	}

private:
	reference<ParseResult> *InlineChildren() {
		return reinterpret_cast<reference<ParseResult> *>(inline_storage);
	}

private:
	ArenaAllocator &arena;
	reference<ParseResult> *children;
	idx_t count = 0;
	idx_t capacity;
	alignas(reference<ParseResult>) char inline_storage[INLINE_CAPACITY * sizeof(reference<ParseResult>)];
};

class AtomicMatchProcess : public MatchProcess {
public:
	AtomicMatchProcess(const AtomicMatcher &matcher_p, MatchState &state_p) : matcher(matcher_p), state(state_p) {
	}

	MatchStep Resume(const MatcherResult *child_result) override {
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

	MatchStep Resume(const MatcherResult *child_result) override {
		D_ASSERT(awaiting_child == (child_result != nullptr));
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
			if (!AtAutocompleteCursor()) {
				// An optional child that cannot start at this token always produces the same empty result. Answering
				// that here saves a frame, a match process and a parse result per absent optional, which for the
				// operator precedence rules is one per level per operand.
				if (child_matcher.Type() == MatcherType::OPTIONAL && !child_matcher.MayMatchHere(list_state)) {
					if (list_state.BuildParseResult()) {
						results.Add(*list_state.context.EmptyOptionalResult());
					}
					child_index++;
					continue;
				}
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
	//! Only a stream tokenized for auto-completion has a cursor in it, so the common case is a flag test rather
	//! than a look at the current token
	bool AtAutocompleteCursor() {
		if (!list_state.token_iterator.HasAutocompleteCursor()) {
			return false;
		}
		auto current = list_state.token_iterator.Current();
		return current && current->type == TokenType::END_OF_INPUT_AUTOCOMPLETE;
	}

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

//! Matches a level of the operator precedence ladder. See PrecedenceLadder for what the levels are and why they are
//! not matched as the nested rules they are written as.
//!
//! The operand is parsed once, by the rule the chain ends at, and the levels are then walked outwards: a level whose
//! tail cannot start at the current token contributes nothing and is skipped, and a level whose tail does match
//! builds the node the written rule would have built. A level that contributes nothing is exactly the case the
//! collapse in ListMatchProcess handles, so the result handed back is the operand itself, marked collapsed.
class PrecedenceMatchProcess : public MatchProcess {
public:
	PrecedenceMatchProcess(const ListMatcher &matcher_p, MatchState &state_p)
	    : ladder(*matcher_p.GetLadder()), min_level(matcher_p.GetPrecedenceLevel()), state(state_p),
	      work_state(state_p), attempt_state(state_p), tails(state_p, 0) {
		if (auto current = work_state.token_iterator.Current()) {
			start_offset = optional_idx(current->offset);
		}
	}

	MatchStep Resume(const MatcherResult *child_result) override {
		switch (stage) {
		case Stage::START:
			return Start();
		case Stage::AWAIT_PREFIX:
			return PrefixMatched(child_result);
		case Stage::AWAIT_PREFIX_OPERAND:
			return PrefixOperandMatched(child_result);
		case Stage::AWAIT_LEAF:
			return LeafMatched(child_result);
		case Stage::AWAIT_TAIL:
			return TailMatched(child_result);
		default:
			throw InternalException("Unexpected state in the precedence ladder matcher");
		}
	}

private:
	enum class Stage : uint8_t { START, AWAIT_PREFIX, AWAIT_PREFIX_OPERAND, AWAIT_LEAF, AWAIT_TAIL };

	//! The outermost prefix level that is allowed here and whose prefix can start at the current token
	idx_t FindPrefixLevel() {
		for (auto level : ladder.prefix_levels) {
			if (level >= min_level && ladder.levels[level].affix->MayMatchHere(work_state)) {
				return level;
			}
		}
		return ladder.levels.size();
	}

	//! The suffix levels whose tail could start at the current token
	uint32_t CandidateLevels() {
		auto token = work_state.token_iterator.Current();
		if (!token) {
			return 0;
		}
		auto candidates = ladder.predicate_levels;
		if (ladder.literal_table) {
			auto literal_id = work_state.token_iterator.CurrentLiteralInfo(*ladder.literal_table).LiteralId();
			candidates |= ladder.LiteralLevels(literal_id);
		}
		return candidates;
	}

	MatchStep Start() {
		auto prefix_level = FindPrefixLevel();
		if (prefix_level < ladder.levels.size()) {
			pending_level = prefix_level;
			stage = Stage::AWAIT_PREFIX;
			attempt_state.token_iterator.SetPosition(work_state.token_iterator);
			return MatchStep::Child({*ladder.levels[prefix_level].affix, attempt_state});
		}
		stage = Stage::AWAIT_LEAF;
		return MatchStep::Child({*ladder.leaf, work_state});
	}

	MatchStep PrefixMatched(const MatcherResult *child_result) {
		D_ASSERT(child_result);
		if (!child_result->IsSuccess()) {
			// the start set only promised the prefix might match; without it this is an ordinary operand
			stage = Stage::AWAIT_LEAF;
			return MatchStep::Child({*ladder.leaf, work_state});
		}
		prefix_result = child_result->GetParseResult();
		work_state.token_iterator.SetPosition(attempt_state.token_iterator);
		stage = Stage::AWAIT_PREFIX_OPERAND;
		return MatchStep::Child({OperandMatcher(pending_level), work_state});
	}

	MatchStep PrefixOperandMatched(const MatcherResult *child_result) {
		D_ASSERT(child_result);
		if (!child_result->IsSuccess()) {
			return MatchStep::Complete(MatcherResult::Failure());
		}
		value = child_result->GetParseResult();
		if (BuildsResults() && value) {
			auto &entry = ladder.levels[pending_level];
			auto prefix_optional = MakeOptional(prefix_result, start_offset);
			value = MakeNode(entry, *prefix_optional, *value);
		}
		value_level = pending_level;
		climb_level = pending_level;
		return Climb();
	}

	MatchStep LeafMatched(const MatcherResult *child_result) {
		D_ASSERT(child_result);
		if (!child_result->IsSuccess()) {
			return MatchStep::Complete(MatcherResult::Failure());
		}
		value = child_result->GetParseResult();
		value_level = ladder.LeafLevel();
		climb_level = value_level;
		return Climb();
	}

	//! Walk outwards from the level the value currently sits at, looking for a level whose tail starts here
	MatchStep Climb() {
		if (climb_level <= min_level) {
			return Complete();
		}
		auto candidates = CandidateLevels() & PrecedenceLadder::LevelRange(min_level, climb_level);
		while (candidates) {
			auto level = idx_t(31 - __builtin_clz(candidates));
			candidates &= ~(uint32_t(1) << level);
			// a level the literal table could not answer for still needs its own probe
			if (((ladder.predicate_levels >> level) & 1) && !ladder.levels[level].affix->MayMatchHere(work_state)) {
				continue;
			}
			climb_level = level;
			return RequestTail();
		}
		climb_level = min_level;
		return Complete();
	}

	MatchStep RequestTail() {
		pending_level = climb_level;
		stage = Stage::AWAIT_TAIL;
		attempt_state.token_iterator.SetPosition(work_state.token_iterator);
		if (auto current = work_state.token_iterator.Current()) {
			tail_offset = optional_idx(current->offset);
		}
		return MatchStep::Child({*ladder.levels[climb_level].affix, attempt_state});
	}

	MatchStep TailMatched(const MatcherResult *child_result) {
		D_ASSERT(child_result);
		auto &entry = ladder.levels[pending_level];
		if (child_result->IsSuccess()) {
			work_state.token_iterator.SetPosition(attempt_state.token_iterator);
			if (BuildsResults() && child_result->HasParseResult()) {
				tails.Add(*child_result->GetParseResult());
			}
			if (entry.shape == PrecedenceShape::SUFFIX_REPEAT && entry.affix->MayMatchHere(work_state)) {
				return RequestTail();
			}
		}
		if (tails.Size() > 0) {
			value = MakeSuffixNode(entry);
			value_level = pending_level;
		}
		tails.Reset();
		return Climb();
	}

	MatchStep Complete() {
		state.token_iterator.SetPosition(work_state.token_iterator);
		if (!BuildsResults()) {
			return MatchStep::Complete(MatcherResult::Success());
		}
		// every level between the one the value sits at and this one matched nothing but their operand
		MarkCollapsed(min_level);
		return MatchStep::Complete(MatcherResult::Success(value));
	}

private:
	bool BuildsResults() const {
		return state.BuildParseResult();
	}

	//! A level that matched nothing but its operand hands out the operand's result in place of its own, which the
	//! transformer recognises by the collapsed flag. Set it when levels were skipped between `level` and the value.
	void MarkCollapsed(idx_t level) {
		if (value && value_level != level) {
			value->collapsed = true;
		}
	}

	const Matcher &OperandMatcher(idx_t level) const {
		return *ladder.levels[level].operand;
	}

	optional_ptr<ParseResult> MakeOptional(optional_ptr<ParseResult> child, optional_idx offset) {
		if (!child) {
			return state.context.EmptyOptionalResult();
		}
		return state.context.allocator.Make<OptionalParseResult>(child, offset);
	}

	//! Build the node the written rule would have built for this level
	optional_ptr<ParseResult> MakeNode(const PrecedenceLevel &entry, ParseResult &first, ParseResult &second) {
		reference<ParseResult> children[2] = {first, second};
		auto list_children = state.context.allocator.MakeChildren(children, 2);
		auto result = state.context.allocator.Make<ListParseResult>(list_children, &entry.rule->name, start_offset);
		result->SetRule(*entry.rule);
		return result;
	}

	optional_ptr<ParseResult> MakeSuffixNode(const PrecedenceLevel &entry) {
		if (!BuildsResults()) {
			return value;
		}
		// the operand of this level is the next level down; anything deeper collapsed on the way here
		MarkCollapsed(pending_level + 1);
		optional_ptr<ParseResult> affix_result;
		if (entry.shape == PrecedenceShape::SUFFIX_REPEAT) {
			auto repeat_children = tails.Finalize(state);
			affix_result = state.context.allocator.Make<RepeatParseResult>(repeat_children, tail_offset);
		} else {
			D_ASSERT(tails.Size() == 1);
			affix_result = tails.Children()[0].get();
		}
		auto optional_result = MakeOptional(affix_result, tail_offset);
		return MakeNode(entry, *value, *optional_result);
	}

private:
	const PrecedenceLadder &ladder;
	idx_t min_level;
	MatchState &state;
	//! The position everything matched so far has advanced to
	MatchState work_state;
	//! Used for a tail or prefix that may fail, which must not move the committed position
	MatchState attempt_state;
	ChildCollector tails;
	optional_ptr<ParseResult> value;
	optional_ptr<ParseResult> prefix_result;
	idx_t value_level = 0;
	idx_t climb_level = 0;
	idx_t pending_level = 0;
	optional_idx start_offset;
	optional_idx tail_offset;
	Stage stage = Stage::START;
};


template <bool SINGLE_CHILD>
class ChoiceMatchProcess : public MatchProcess {
public:
	ChoiceMatchProcess(const ChoiceMatcher &matcher_p, MatchState &state_p, idx_t child_index_p = 0,
	                   optional_ptr<const ListMatcher> wrapper_p = nullptr)
	    : matcher(matcher_p), state(state_p), wrapper(wrapper_p), child_index(child_index_p) {
		if (auto current = state.token_iterator.Current()) {
			start_offset = optional_idx(current->offset);
		}
	}

	MatchStep Resume(const MatcherResult *child_result) override {
		D_ASSERT(awaiting_child == (child_result != nullptr));
		if (child_result) {
			awaiting_child = false;
			D_ASSERT(child_state);
			if (child_result->IsSuccess()) {
				state.token_iterator.SetPosition(child_state.value().token_iterator);
				if (!child_result->HasParseResult()) {
					return MatchStep::Complete(WrapResult(nullptr));
				}
				auto choice_result = state.AllocateParseResult<ChoiceParseResult>(*child_result->GetParseResult(),
				                                                                  child_index, start_offset);
				return MatchStep::Complete(WrapResult(choice_result.GetParseResult()));
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
	//! A rule whose whole body is one ordered choice is matched in this frame rather than in a list frame of its
	//! own, so the result the list would have built is built here instead.
	MatcherResult WrapResult(optional_ptr<ParseResult> choice_result) {
		if (!wrapper) {
			return MatcherResult::Success(choice_result);
		}
		if (!choice_result) {
			return state.AllocateParseResult<ListParseResult>(ParseResultChildren(), WrapperName(), start_offset);
		}
		if (wrapper->IsCollapsible()) {
			choice_result->collapsed = true;
			return MatcherResult::Success(choice_result);
		}
		reference<ParseResult> children[1] = {*choice_result};
		auto list_children = state.context.allocator.MakeChildren(children, 1);
		return state.AllocateParseResult<ListParseResult>(list_children, WrapperName(), start_offset);
	}

	const string *WrapperName() const {
		return wrapper->HasName() ? &wrapper->GetNameRef() : nullptr;
	}

private:
	const ChoiceMatcher &matcher;
	MatchState &state;
	//! Set when this frame stands in for the list frame of a rule whose body is only this choice
	optional_ptr<const ListMatcher> wrapper;
	optional<MatchState> child_state;
	idx_t child_index = 0;
	optional_idx start_offset;
	bool awaiting_child = false;
};

arena_ptr<MatchProcess> ChoiceMatcher::StartMatch(MatchState &state) const {
	return state.Make<ChoiceMatchProcess<false>>(*this, state);
}

arena_ptr<MatchProcess> ListMatcher::StartMatch(MatchState &state) const {
	// Auto-completion needs the suggestions that the failing children of the written rules produce, so neither the
	// ladder nor the fused choice is taken when the stream has a cursor in it.
	if (!state.token_iterator.HasAutocompleteCursor()) {
		if (ladder) {
			return state.Make<PrecedenceMatchProcess>(*this, state);
		}
		if (fused_choice) {
			return fused_choice->StartFusedMatch(state, *this);
		}
	}
	return state.Make<ListMatchProcess>(*this, state);
}

idx_t LiteralChoiceMatcher::DispatchIndex(MatchState &state) const {
	auto literal = state.token_iterator.CurrentLiteralInfo(table);
	auto entry = literal_children.find(literal.LiteralId());
	return entry == literal_children.end() ? matchers.size() : entry->second;
}

arena_ptr<MatchProcess> LiteralChoiceMatcher::StartMatch(MatchState &state) const {
	return state.Make<ChoiceMatchProcess<true>>(*this, state, DispatchIndex(state));
}

arena_ptr<MatchProcess> LiteralChoiceMatcher::StartFusedMatch(MatchState &state, const ListMatcher &wrapper) const {
	return state.Make<ChoiceMatchProcess<true>>(*this, state, DispatchIndex(state), wrapper);
}

arena_ptr<MatchProcess> ChoiceMatcher::StartFusedMatch(MatchState &state, const ListMatcher &wrapper) const {
	return state.Make<ChoiceMatchProcess<false>>(*this, state, 0, wrapper);
}

class OptionalMatchProcess : public MatchProcess {
public:
	OptionalMatchProcess(const OptionalMatcher &matcher_p, MatchState &state_p)
	    : matcher(matcher_p), state(state_p), child_state(state_p) {
		if (auto current = child_state.token_iterator.Current()) {
			start_offset = optional_idx(current->offset);
		}
	}

	MatchStep Resume(const MatcherResult *child_result) override {
		D_ASSERT(awaiting_child == (child_result != nullptr));
		if (!child_result) {
			if (!matcher.GetChildMatcher().MayMatchHere(child_state)) {
				return MatchStep::Complete(EmptyResult());
			}
			awaiting_child = true;
			return MatchStep::Child({matcher.GetChildMatcher(), child_state});
		}
		awaiting_child = false;
		if (!child_result->IsSuccess()) {
			return MatchStep::Complete(EmptyResult());
		}
		state.token_iterator.SetPosition(child_state.token_iterator);
		if (!child_result->HasParseResult()) {
			return MatchStep::Complete(MatcherResult::Success());
		}
		return MatchStep::Complete(
		    state.AllocateParseResult<OptionalParseResult>(child_result->GetParseResult(), start_offset));
	}

private:
	MatcherResult EmptyResult() const {
		if (!state.BuildParseResult()) {
			return MatcherResult::Success();
		}
		return MatcherResult::Success(state.context.EmptyOptionalResult());
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

	MatchStep Resume(const MatcherResult *child_result) override {
		D_ASSERT(awaiting_child == (child_result != nullptr));
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
			auto current = repeat_state.token_iterator.HasAutocompleteCursor() ? repeat_state.token_iterator.Current()
			                                                                  : nullptr;
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
