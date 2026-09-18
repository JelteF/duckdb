//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/peg/matcher/precedence_hierarchy.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/parser/peg/matcher.hpp"

namespace duckdb {

//! The form a precedence hierarchy level is written in. Not serialized, so it is on the blacklist in
//! scripts/generate_enum_util.py.
enum class PrecedenceShape : uint8_t {
	//! X <- Y - the rule only forwards its operand
	ALIAS,
	//! X <- Y Tail*
	SUFFIX_REPEAT,
	//! X <- Y Tail?
	SUFFIX_OPTIONAL,
	//! X <- Prefix? Y
	PREFIX_OPTIONAL
};

//! One level of the operator precedence hierarchy: a collapsible rule of the form `X <- Y`, `X <- Y Tail*`,
//! `X <- Y Tail?` or `X <- Prefix? Y`, whose operand `Y` is the next level down.
struct PrecedenceLevel {
	PrecedenceShape shape = PrecedenceShape::ALIAS;
	//! The rule of this level, used to give a built node the rule and name it would have had
	optional_ptr<const CompiledGrammarRule> rule;
	//! The tail, or for PREFIX_OPTIONAL the prefix. Null for ALIAS.
	optional_ptr<Matcher> affix;
	//! The matcher of the operand, which is the next level down or the leaf
	optional_ptr<Matcher> operand;

	bool IsPrefix() const {
		return shape == PrecedenceShape::PREFIX_OPTIONAL;
	}
	bool IsSuffix() const {
		return shape == PrecedenceShape::SUFFIX_REPEAT || shape == PrecedenceShape::SUFFIX_OPTIONAL;
	}
};

//! A chain of precedence levels, shared by the matchers of all the rules in it.
//!
//! Written out, the chain makes every operand of an expression descend through a frame per level: `42` walks
//! `Expression`, `LambdaArrowExpression`, ..., `BaseExpression` before reaching a literal, and every one of those
//! frames then finds that no tail follows. Matching the chain as a hierarchy instead parses the operand once and walks
//! the levels outwards, attaching only the tails that are actually there. The parse result is the same one the
//! nested rules produce, including the collapse of a level that matched nothing but its operand.
struct PrecedenceHierarchy {
	//! Level 0 is the outermost rule (the lowest precedence)
	vector<PrecedenceLevel> levels;
	//! The rule the chain ends at, matched as written
	optional_ptr<Matcher> leaf;

	idx_t LeafLevel() const {
		return levels.size();
	}
};

} // namespace duckdb
