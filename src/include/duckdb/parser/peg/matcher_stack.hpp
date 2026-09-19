//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/peg/matcher_stack.hpp
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/optional.hpp"
#include "duckdb/common/optional_idx.hpp"
#include "duckdb/parser/peg/matcher.hpp"

namespace duckdb {

struct PackratMatchState {
	static bool IsEnabled(const Matcher &matcher, const MatchState &state) {
		return state.context.packrat_cache && matcher.IsPackratMemoized() && matcher.GetPackratId().IsValid();
	}

	optional<MatcherResult> TryLoadCachedResult(const Matcher &matcher, MatchState &state);
	//! Inline: every frame calls this, but only the memoized rules have anything to store
	void StoreResult(const Matcher &matcher, MatchState &state, const MatcherResult &result) const {
		if (!token_index_before.IsValid()) {
			return;
		}
		StoreResultInternal(matcher, state, result);
	}

private:
	void StoreResultInternal(const Matcher &matcher, MatchState &state, const MatcherResult &result) const;

private:
	optional_idx token_index_before;
	idx_t max_token_index_before = 0;
};

struct MatchStackFrame {
public:
	explicit MatchStackFrame(MatchInput input) : matcher(input.matcher), match_state(input.state) {
	}

public:
	bool IsInitialized() const {
		return process || has_result;
	}

public:
	const Matcher &matcher;
	MatchState &match_state;
	arena_ptr<MatchProcess> process;
	MatcherResult child_result = MatcherResult::Failure();
	MatcherResult result = MatcherResult::Failure();
	bool has_child_result = false;
	bool has_result = false;
	PackratMatchState packrat_state;
};

class MatchStack {
public:
	MatchStack();
	~MatchStack();

	MatcherResult Execute(MatchInput input);

private:
	static constexpr idx_t INITIAL_FRAME_CAPACITY = 64;

	MatcherResult ExecuteAtomicMatcher(MatchInput input);
	void DestroyTopFrame();
	void PushFrame(MatchInput input);
	void InitializeFrame(MatchStackFrame &frame);
	//! Returns true when the frame has completed.
	bool ExecuteFrame(MatchStackFrame &frame);
	MatcherResult FinalizeFrame(MatchStackFrame &frame);

private:
	vector<MatchStackFrame> frames;
};

} // namespace duckdb
