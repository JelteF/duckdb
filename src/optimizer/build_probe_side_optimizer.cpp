#include "duckdb/optimizer/build_probe_side_optimizer.hpp"
#include "duckdb/optimizer/recursive_join_planning.hpp"

#include "duckdb/common/enums/join_type.hpp"
#include "duckdb/common/type_visitor.hpp"
#include "duckdb/common/types/row/tuple_data_layout.hpp"
#include "duckdb/execution/ht_entry.hpp"
#include "duckdb/execution/operator/join/join_filter_pushdown.hpp"
#include "duckdb/planner/operator/logical_any_join.hpp"
#include "duckdb/planner/operator/logical_comparison_join.hpp"
#include "duckdb/planner/operator/logical_cte.hpp"
#include "duckdb/planner/operator/logical_cteref.hpp"
#include "duckdb/planner/operator/logical_get.hpp"
#include "duckdb/planner/operator/logical_join.hpp"
#include "duckdb/planner/operator/logical_order.hpp"
#include "duckdb/optimizer/column_binding_replacer.hpp"
#include "duckdb/optimizer/join_filter_pushdown_optimizer.hpp"
#include "duckdb/optimizer/optimizer.hpp"
#include "duckdb/planner/expression/bound_columnref_expression.hpp"
#include "duckdb/planner/operator/logical_cross_product.hpp"
#include "duckdb/planner/operator/logical_projection.hpp"
#include "duckdb/main/settings.hpp"
#include "duckdb/storage/statistics/list_stats.hpp"

namespace duckdb {

struct JoinFilterBuildSideHeuristics {
	static constexpr idx_t MIN_FILTER_TARGET_CARDINALITY = 1000000;
	static constexpr idx_t MAX_BUILD_TO_TARGET_RATIO = 64;
	static constexpr idx_t SEMI_JOIN_FILTER_TARGET_RATIO = 3;
};

static void GetRowidBindings(LogicalOperator &op, vector<ColumnBinding> &bindings) {
	if (op.type == LogicalOperatorType::LOGICAL_GET) {
		auto &get = op.Cast<LogicalGet>();
		auto get_bindings = get.GetColumnBindings();
		auto &column_ids = get.GetColumnIds();
		bool has_row_id = false;
		for (auto &col_id : column_ids) {
			if (col_id.IsRowIdColumn()) {
				has_row_id = true;
				break;
			}
		}
		if (has_row_id) {
			for (auto &binding : get_bindings) {
				bindings.push_back(binding);
			}
		}
	}
	for (auto &child : op.children) {
		GetRowidBindings(*child, bindings);
	}
}

BuildProbeSideOptimizer::BuildProbeSideOptimizer(ClientContext &context, LogicalOperator &op) : context(context) {
	vector<ColumnBinding> updating_columns, current_op_bindings;
	auto bindings = op.GetColumnBindings();
	vector<ColumnBinding> row_id_bindings;
	// If any column bindings are a row_id, there is a good chance the statement is an insert/delete/update statement.
	// As an initialization step, we travers the plan and find which bindings are row_id bindings.
	// When we eventually do our build side probe side optimizations, if we get to a join where the left and right
	// cardinalities are the same, we prefer to have the child with the rowid bindings in the probe side.
	GetRowidBindings(op, preferred_on_probe_side);
	op.ResolveOperatorTypes();
}

static void FlipChildren(LogicalOperator &op) {
	std::swap(op.children[0], op.children[1]);
	switch (op.type) {
	case LogicalOperatorType::LOGICAL_COMPARISON_JOIN:
	case LogicalOperatorType::LOGICAL_DELIM_JOIN: {
		auto &join = op.Cast<LogicalComparisonJoin>();
		join.join_type = InverseJoinType(join.join_type);
		for (idx_t i = 0; i < join.conditions.size(); i++) {
			auto &cond = join.conditions[i];
			if (cond.IsComparison()) {
				auto left_expr = cond.RightReference()->Copy();
				auto right_expr = cond.LeftReference()->Copy();
				auto flipped_comparison = FlipComparisonExpression(cond.GetComparisonType());

				join.conditions[i] = JoinCondition(std::move(left_expr), std::move(right_expr), flipped_comparison);
			}
		}
		std::swap(join.left_projection_map, join.right_projection_map);
		return;
	}
	case LogicalOperatorType::LOGICAL_ANY_JOIN: {
		auto &join = op.Cast<LogicalAnyJoin>();
		join.join_type = InverseJoinType(join.join_type);
		std::swap(join.left_projection_map, join.right_projection_map);
		return;
	}
	case LogicalOperatorType::LOGICAL_CROSS_PRODUCT: {
		// don't need to do anything here
		return;
	}
	default:
		throw InternalException("Flipping children, but children were not flipped");
	}
}

static inline idx_t ComputeOverlappingBindings(const vector<ColumnBinding> &haystack,
                                               const vector<ColumnBinding> &needles) {
	idx_t result = 0;
	for (auto &needle : needles) {
		if (std::find(haystack.begin(), haystack.end(), needle) != haystack.end()) {
			result++;
		}
	}
	return result;
}

static idx_t MaxDynamicFilterTargetCardinality(LogicalOperator &op, const Expression &expr) {
	JoinFilterPushdownColumn column;
	if (!JoinFilterPushdownUtil::PushdownJoinFilterExpression(expr, column)) {
		return 0;
	}

	vector<JoinFilterPushdownColumn> columns {std::move(column)};
	vector<PushdownFilterTarget> targets;
	JoinFilterPushdownOptimizer::GetPushdownFilterTargets(op, std::move(columns), targets,
	                                                      EqualityPropagation::ENABLED);

	// Dynamic filters are applied at the scan target found here. Estimate the work that the
	// filter can avoid at that point, even if later filters reduce the cardinality at the join.
	idx_t result = 0;
	for (auto &target : targets) {
		auto &get = target.get;
		result = MaxValue(result, get.has_estimated_cardinality ? get.estimated_cardinality : idx_t(0));
	}
	return result;
}

static double DynamicFilterBuildBonus(LogicalComparisonJoin &join, const idx_t probe_idx, const idx_t build_idx,
                                      const idx_t build_cardinality) {
	if (!JoinFilterPushdownOptimizer::IsFiltering(join.children[build_idx])) {
		return 1;
	}

	idx_t max_target_cardinality = 0;
	for (auto &cond : join.conditions) {
		if (!cond.IsComparison() || cond.GetComparisonType() != ExpressionType::COMPARE_EQUAL) {
			continue;
		}
		auto &probe_expr = probe_idx == 0 ? cond.GetLHS() : cond.GetRHS();
		max_target_cardinality =
		    MaxValue(max_target_cardinality, MaxDynamicFilterTargetCardinality(*join.children[probe_idx], probe_expr));
	}
	if (max_target_cardinality < JoinFilterBuildSideHeuristics::MIN_FILTER_TARGET_CARDINALITY) {
		return 1;
	}
	if (build_cardinality > 0 &&
	    build_cardinality > max_target_cardinality / JoinFilterBuildSideHeuristics::MAX_BUILD_TO_TARGET_RATIO) {
		return 1;
	}
	return static_cast<double>(max_target_cardinality) / static_cast<double>(MaxValue<idx_t>(build_cardinality, 1));
}

static idx_t MaxDynamicFilterTargetCardinality(LogicalComparisonJoin &join, const idx_t probe_idx) {
	idx_t max_target_cardinality = 0;
	for (auto &cond : join.conditions) {
		if (!cond.IsComparison() || cond.GetComparisonType() != ExpressionType::COMPARE_EQUAL) {
			continue;
		}
		auto &probe_expr = probe_idx == 0 ? cond.GetLHS() : cond.GetRHS();
		max_target_cardinality =
		    MaxValue(max_target_cardinality, MaxDynamicFilterTargetCardinality(*join.children[probe_idx], probe_expr));
	}
	return max_target_cardinality;
}

//! Follows a column down to the scan that produces it, through operators that pass their children's columns through
static optional_ptr<LogicalGet> FindColumnScan(LogicalOperator &op, const ColumnBinding &binding,
                                               ColumnBinding &scan_binding) {
	switch (op.type) {
	case LogicalOperatorType::LOGICAL_GET: {
		auto &get = op.Cast<LogicalGet>();
		if (get.table_index != binding.table_index) {
			return nullptr;
		}
		scan_binding = binding;
		return &get;
	}
	case LogicalOperatorType::LOGICAL_PROJECTION: {
		auto &projection = op.Cast<LogicalProjection>();
		if (projection.table_index != binding.table_index) {
			return nullptr;
		}
		auto &expr = *projection.expressions[binding.column_index];
		if (expr.GetExpressionClass() != ExpressionClass::BOUND_COLUMN_REF) {
			return nullptr;
		}
		return FindColumnScan(*op.children[0], expr.Cast<BoundColumnRefExpression>().Binding(), scan_binding);
	}
	case LogicalOperatorType::LOGICAL_FILTER:
	case LogicalOperatorType::LOGICAL_ORDER_BY:
	case LogicalOperatorType::LOGICAL_LIMIT:
	case LogicalOperatorType::LOGICAL_TOP_N:
	case LogicalOperatorType::LOGICAL_SAMPLE:
	case LogicalOperatorType::LOGICAL_COMPARISON_JOIN:
	case LogicalOperatorType::LOGICAL_ANY_JOIN:
	case LogicalOperatorType::LOGICAL_DELIM_JOIN:
	case LogicalOperatorType::LOGICAL_CROSS_PRODUCT:
		for (auto &child : op.children) {
			auto get = FindColumnScan(*child, binding, scan_binding);
			if (get) {
				return get;
			}
		}
		return nullptr;
	default:
		return nullptr;
	}
}

idx_t BuildProbeSideOptimizer::EstimateNestedWidth(LogicalOperator &op, const ColumnBinding &binding,
                                                   const LogicalType &type) const {
	switch (type.InternalType()) {
	case PhysicalType::ARRAY: {
		auto child_type = ArrayType::GetChildType(type).InternalType();
		if (!TypeIsConstantSize(child_type)) {
			return 0;
		}
		return ArrayType::GetSize(type) * GetTypeIdSize(child_type);
	}
	case PhysicalType::LIST: {
		auto child_type = ListType::GetChildType(type).InternalType();
		if (!TypeIsConstantSize(child_type)) {
			return 0;
		}
		ColumnBinding scan_binding;
		auto get = FindColumnScan(op, binding, scan_binding);
		if (!get || !get->bind_data || (!get->function.statistics && !get->function.statistics_extended)) {
			return 0;
		}
		auto &column_index = get->GetColumnIndex(scan_binding);
		if (column_index.IsVirtualColumn()) {
			return 0;
		}
		unique_ptr<BaseStatistics> stats;
		if (get->function.statistics_extended) {
			TableFunctionGetStatisticsInput input(get->bind_data.get(), column_index);
			stats = get->function.statistics_extended(context, input);
		} else {
			stats = get->function.statistics(context, get->bind_data.get(), column_index.GetPrimaryIndex());
		}
		if (!stats || stats->GetStatsType() != StatisticsType::LIST_STATS) {
			return 0;
		}
		auto total_length = ListStats::TotalListLength(*stats);
		auto scan_cardinality = get->EstimateSourceCardinality(context);
		if (!total_length.IsValid() || scan_cardinality == 0) {
			return 0;
		}
		return total_length.GetIndex() / scan_cardinality * GetTypeIdSize(child_type);
	}
	default:
		return 0;
	}
}

vector<idx_t> BuildProbeSideOptimizer::EstimateNestedWidths(LogicalOperator &op) const {
	auto bindings = op.GetColumnBindings();
	if (bindings.size() != op.types.size()) {
		return {};
	}
	vector<idx_t> result;
	for (idx_t col_idx = 0; col_idx < bindings.size(); col_idx++) {
		result.push_back(EstimateNestedWidth(op, bindings[col_idx], op.types[col_idx]));
	}
	return result;
}

BuildSize BuildProbeSideOptimizer::GetBuildSizes(LogicalOperator &op, const idx_t lhs_cardinality,
                                                 const idx_t rhs_cardinality) const {
	BuildSize ret;
	switch (op.type) {
	case LogicalOperatorType::LOGICAL_COMPARISON_JOIN:
	case LogicalOperatorType::LOGICAL_CROSS_PRODUCT:
	case LogicalOperatorType::LOGICAL_ANY_JOIN:
	case LogicalOperatorType::LOGICAL_DELIM_JOIN: {
		ret.left_side = GetBuildSize(op.children[0]->types, lhs_cardinality, EstimateNestedWidths(*op.children[0]));
		ret.right_side = GetBuildSize(op.children[1]->types, rhs_cardinality, EstimateNestedWidths(*op.children[1]));
		return ret;
	}
	default:
		break;
	}
	return ret;
}

double BuildProbeSideOptimizer::GetBuildSize(vector<LogicalType> types, const idx_t cardinality,
                                             const vector<idx_t> &nested_widths) {
	// Row width in the hash table
	types.push_back(LogicalType::HASH);
	auto tuple_layout = TupleDataLayout();
	tuple_layout.Initialize(types, TupleDataValidityType::CAN_HAVE_NULL_VALUES);
	auto row_width = tuple_layout.GetRowWidth();

	for (const auto &type : types) {
		TypeVisitor::VisitReplace(type, [&](const LogicalType &visited_type) {
			// Penalty for variable-size types (we don't have statistics here yet)
			switch (visited_type.InternalType()) {
			case PhysicalType::VARCHAR:
				row_width += 8;
				break;
			case PhysicalType::LIST:
			case PhysicalType::ARRAY:
				row_width += NESTED_WIDTH_PENALTY;
				break;
			default:
				break;
			}

			// Penalty for number of (recursive) columns
			row_width += COLUMN_COUNT_PENALTY;

			return visited_type;
		});
	}
	// The fixed penalty stays a lower bound, so that short lists don't change the plans picked without statistics
	for (auto nested_width : nested_widths) {
		row_width += MaxValue<idx_t>(nested_width, NESTED_WIDTH_PENALTY) - NESTED_WIDTH_PENALTY;
	}

	// There is also a cost of NextPowerOfTwo(count * 2) * sizeof(ht_entry_t) per tuple in the hash table
	// This is a not a smooth cost function, so instead we do the average, which is ~3 * sizeof(ht_entry_t)
	row_width += 3 * sizeof(ht_entry_t);

	return MaxValue<double>(static_cast<double>(row_width) * static_cast<double>(cardinality), sizeof(ht_entry_t));
}

idx_t BuildProbeSideOptimizer::ChildHasJoins(LogicalOperator &op) {
	if (op.children.empty()) {
		return 0;
	} else if (op.type == LogicalOperatorType::LOGICAL_COMPARISON_JOIN ||
	           op.type == LogicalOperatorType::LOGICAL_ASOF_JOIN ||
	           op.type == LogicalOperatorType::LOGICAL_CROSS_PRODUCT) {
		return 1 + ChildHasJoins(*op.children[0]) + ChildHasJoins(*op.children[1]);
	}
	return ChildHasJoins(*op.children[0]);
}

bool BuildProbeSideOptimizer::TryFlipJoinChildren(LogicalOperator &op) const {
	auto recursive_preference = GetRecursiveProbeSidePreference(op);
	if (recursive_preference != RecursiveProbeSidePreference::NONE) {
		if (recursive_preference == RecursiveProbeSidePreference::SWAP) {
			FlipChildren(op);
			return true;
		}
		return false;
	}

	auto &left_child = *op.children[0];
	auto &right_child = *op.children[1];
	const auto lhs_cardinality = left_child.has_estimated_cardinality ? left_child.estimated_cardinality
	                                                                  : left_child.EstimateCardinality(context);
	const auto rhs_cardinality = right_child.has_estimated_cardinality ? right_child.estimated_cardinality
	                                                                   : right_child.EstimateCardinality(context);

	auto build_sizes = GetBuildSizes(op, lhs_cardinality, rhs_cardinality);
	auto &left_side_build_cost = build_sizes.left_side;
	auto &right_side_build_cost = build_sizes.right_side;

	bool swap = false;

	if (op.type == LogicalOperatorType::LOGICAL_COMPARISON_JOIN) {
		auto &join = op.Cast<LogicalComparisonJoin>();
		if (JoinFilterPushdownUtil::JoinTypeIsSupported(join.join_type)) {
			right_side_build_cost /= DynamicFilterBuildBonus(join, 0, 1, rhs_cardinality);
		}
		if (HasInverseJoinType(join.join_type) &&
		    JoinFilterPushdownUtil::JoinTypeIsSupported(InverseJoinType(join.join_type))) {
			left_side_build_cost /= DynamicFilterBuildBonus(join, 1, 0, lhs_cardinality);
		}
		if (join.join_type == JoinType::SEMI && JoinFilterPushdownOptimizer::IsFiltering(join.children[0])) {
			// SEMI joins often have a filtered domain on the LHS and a larger RHS with residual filters. If flipping
			// lets that domain generate a runtime filter for a much larger RHS scan, prefer the domain build side.
			auto right_filter_target = MaxDynamicFilterTargetCardinality(join, 1);
			if (right_filter_target >= JoinFilterBuildSideHeuristics::MIN_FILTER_TARGET_CARDINALITY &&
			    right_filter_target / JoinFilterBuildSideHeuristics::SEMI_JOIN_FILTER_TARGET_RATIO > lhs_cardinality &&
			    right_filter_target / JoinFilterBuildSideHeuristics::SEMI_JOIN_FILTER_TARGET_RATIO > rhs_cardinality) {
				swap = true;
			}
		}
	}

	idx_t left_child_joins = ChildHasJoins(*op.children[0]);
	idx_t right_child_joins = ChildHasJoins(*op.children[1]);
	// if the right child is a table scan, and the left child has joins, we should prefer the left child
	// to be the build side. Since the tuples of the left side will already have been built on/be in flight,
	// it will be faster to build on them again.
	if (right_child_joins == 0 && left_child_joins > 0) {
		right_side_build_cost *= (1 + PREFER_RIGHT_DEEP_PENALTY);
	}

	// RHS is build side.
	// if right_side metric is larger than left_side metric, then right_side is more costly to build on
	// than the lhs. So we swap
	if (right_side_build_cost > left_side_build_cost) {
		swap = true;
	}

	// swap for preferred on probe side
	if (rhs_cardinality == lhs_cardinality && !preferred_on_probe_side.empty()) {
		// inspect final bindings, we prefer them on the probe side
		auto bindings_left = left_child.GetColumnBindings();
		auto bindings_right = right_child.GetColumnBindings();
		auto bindings_in_left = ComputeOverlappingBindings(bindings_left, preferred_on_probe_side);
		auto bindings_in_right = ComputeOverlappingBindings(bindings_right, preferred_on_probe_side);
		// (if the sides are planning to be swapped AND
		// if more projected bindings are in the left (meaning right/build side after the swap)
		// then swap them back. The projected bindings stay in the left/probe side.)
		// OR
		// (if the sides are planning not to be swapped AND
		// if more projected bindings are in the right (meaning right/build)
		// then swap them. The projected bindings are swapped to the left/probe side.)
		if ((swap && bindings_in_left > bindings_in_right) || (!swap && bindings_in_right > bindings_in_left)) {
			swap = !swap;
		}
	}

	if (swap) {
		FlipChildren(op);
	}
	return swap;
}

RecursiveProbeSidePreference BuildProbeSideOptimizer::GetRecursiveProbeSidePreference(const LogicalOperator &op) const {
	if (active_recursive_cte_indexes.empty() || op.type != LogicalOperatorType::LOGICAL_COMPARISON_JOIN) {
		return RecursiveProbeSidePreference::NONE;
	}

	auto &join = op.Cast<LogicalComparisonJoin>();
	idx_t has_range = 0;
	bool prefer_range_joins = Settings::Get<PreferRangeJoinsSetting>(context);
	if (!join.HasEquality(has_range) || prefer_range_joins) {
		return RecursiveProbeSidePreference::NONE;
	}

	auto left = RecursiveJoinProperties::Extract(*op.children[0], active_recursive_cte_indexes);
	auto right = RecursiveJoinProperties::Extract(*op.children[1], active_recursive_cte_indexes);
	if (CanPlaceRecursiveProbe(join.join_type, left, right)) {
		return RecursiveProbeSidePreference::KEEP;
	}

	if (!HasInverseJoinType(join.join_type)) {
		return RecursiveProbeSidePreference::NONE;
	}

	if (CanPlaceRecursiveProbe(InverseJoinType(join.join_type), right, left)) {
		return RecursiveProbeSidePreference::SWAP;
	}

	return RecursiveProbeSidePreference::NONE;
}

static optional_ptr<LogicalOperator> FindCorrelatedDomainAttachment(LogicalCTE &cte) {
	if (cte.correlated_columns.empty()) {
		return nullptr;
	}

	auto correlated_column_count = cte.correlated_columns.size();
	auto anchor_bindings = cte.children[0]->GetColumnBindings();
	if (anchor_bindings.size() != cte.column_count || anchor_bindings.size() < correlated_column_count) {
		return nullptr;
	}

	vector<ColumnBinding> correlated_bindings(
	    anchor_bindings.end() - NumericCast<vector<ColumnBinding>::difference_type>(correlated_column_count),
	    anchor_bindings.end());
	auto current = cte.children[0].get();
	while (current->type == LogicalOperatorType::LOGICAL_PROJECTION) {
		auto &projection = current->Cast<LogicalProjection>();
		for (auto &binding : correlated_bindings) {
			if (binding.table_index != projection.table_index ||
			    binding.column_index.GetIndexUnsafe() >= projection.expressions.size()) {
				return nullptr;
			}
			auto &expression = projection.GetExpression(binding);
			if (expression.GetExpressionClass() != ExpressionClass::BOUND_COLUMN_REF) {
				return nullptr;
			}
			auto &column_ref = expression.Cast<BoundColumnRefExpression>();
			if (column_ref.Depth() != 0) {
				return nullptr;
			}
			binding = column_ref.Binding();
		}
		if (projection.children.size() != 1) {
			return nullptr;
		}
		current = projection.children[0].get();
	}

	if (current->type != LogicalOperatorType::LOGICAL_CROSS_PRODUCT) {
		return nullptr;
	}
	auto domain_bindings = current->children[1]->GetColumnBindings();
	if (domain_bindings != correlated_bindings) {
		return nullptr;
	}
	return current;
}

void BuildProbeSideOptimizer::VisitOperator(LogicalOperator &op) {
	if (op.type == LogicalOperatorType::LOGICAL_RECURSIVE_CTE) {
		auto &cte = op.Cast<LogicalCTE>();
		auto domain_attachment = FindCorrelatedDomainAttachment(cte);
		if (domain_attachment) {
			// FlattenDependentJoins appends the correlation domain to the recursive anchor. Optimize both inputs while
			// preserving the orientation of this generated attachment.
			VisitOperatorChildren(*domain_attachment);
		} else {
			VisitOperator(*op.children[0]);
		}
		active_recursive_cte_indexes.insert(cte.table_index);
		VisitOperator(*op.children[1]);
		active_recursive_cte_indexes.erase(cte.table_index);
		return;
	}

	VisitOperatorChildren(op);

	// then the currentoperator
	switch (op.type) {
	case LogicalOperatorType::LOGICAL_DELIM_JOIN: {
		auto &join = op.Cast<LogicalComparisonJoin>();
		if (HasInverseJoinType(join.join_type)) {
			join.delim_flipped = TryFlipJoinChildren(join);
		}
		break;
	}
	case LogicalOperatorType::LOGICAL_COMPARISON_JOIN: {
		auto &join = op.Cast<LogicalComparisonJoin>();
		switch (join.join_type) {
		case JoinType::SEMI:
		case JoinType::ANTI: {
			// flip only where a physical operator implements the flipped join: the hash join on an equality, and the
			// piecewise merge join for a RIGHT SEMI join on a single range condition
			idx_t has_range = 0;
			bool prefer_range_joins = Settings::Get<PreferRangeJoinsSetting>(context);
			const bool has_equality = join.HasEquality(has_range);
			const bool single_range = join.join_type == JoinType::SEMI && join.conditions.size() == 1 && has_range == 1;
			if ((has_equality && !prefer_range_joins) || single_range) {
				TryFlipJoinChildren(join);
			}
			break;
		}
		default:
			if (HasInverseJoinType(join.join_type)) {
				TryFlipJoinChildren(op);
			}
		}
		break;
	}
	case LogicalOperatorType::LOGICAL_ANY_JOIN: {
		auto &join = op.Cast<LogicalJoin>();
		// We do not yet support the RIGHT_SEMI or RIGHT_ANTI join types for these, so don't try to flip
		switch (join.join_type) {
		case JoinType::SEMI:
		case JoinType::ANTI:
			break; // RIGHT_SEMI/RIGHT_ANTI not supported yet for ANY/ASOF
		default:
			// We cannot flip projection maps are set (YET), but not flipping is worse than just clearing them
			// They will be set in the 2nd round of ColumnLifetimeAnalyzer
			join.left_projection_map.clear();
			join.right_projection_map.clear();
			TryFlipJoinChildren(op);
		}
		break;
	}
	case LogicalOperatorType::LOGICAL_CROSS_PRODUCT: {
		TryFlipJoinChildren(op);
		break;
	}
	default:
		break;
	}
}

} // namespace duckdb
