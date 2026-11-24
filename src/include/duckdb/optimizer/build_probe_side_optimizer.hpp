//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/optimizer/build_side_probe_side_optimizer.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/unordered_set.hpp"
#include "duckdb/common/vector.hpp"
#include "duckdb/planner/logical_operator.hpp"

namespace duckdb {

struct BuildSize {
	double left_side;
	double right_side;

	// Initialize with 1 so the build side is just the cardinality if types aren't
	// known.
	BuildSize() : left_side(1), right_side(1) {
	}
};

enum class RecursiveProbeSidePreference : uint8_t { NONE, KEEP, SWAP };

class BuildProbeSideOptimizer : LogicalOperatorVisitor {
private:
	static constexpr idx_t COLUMN_COUNT_PENALTY = 2;
	static constexpr double PREFER_RIGHT_DEEP_PENALTY = 0.15;
	//! Bytes per row assumed for the elements of a LIST or ARRAY whose size is unknown
	static constexpr idx_t NESTED_WIDTH_PENALTY = 32;

public:
	explicit BuildProbeSideOptimizer(ClientContext &context, LogicalOperator &op);
	void VisitOperator(LogicalOperator &op) override;
	void VisitExpression(unique_ptr<Expression> *expression) override {};

	//! nested_widths optionally holds, per column, the estimated bytes per row of a LIST or ARRAY's elements
	static double GetBuildSize(vector<LogicalType> types, idx_t cardinality, const vector<idx_t> &nested_widths = {});

private:
	bool TryFlipJoinChildren(LogicalOperator &op) const;
	static idx_t ChildHasJoins(LogicalOperator &op);
	RecursiveProbeSidePreference GetRecursiveProbeSidePreference(const LogicalOperator &op) const;

	BuildSize GetBuildSizes(LogicalOperator &op, idx_t lhs_cardinality, idx_t rhs_cardinality) const;
	vector<idx_t> EstimateNestedWidths(LogicalOperator &op) const;
	idx_t EstimateNestedWidth(LogicalOperator &op, const ColumnBinding &binding, const LogicalType &type) const;

private:
	ClientContext &context;
	vector<ColumnBinding> preferred_on_probe_side;
	unordered_set<TableIndex> active_recursive_cte_indexes;
};

} // namespace duckdb
