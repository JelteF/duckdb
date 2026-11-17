//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/catalog/entry_scan_filters.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/common.hpp"
#include "duckdb/common/optional_ptr.hpp"

namespace duckdb {
class CatalogEntry;
class ClientContext;
class TableFilter;

//! Filters pushed into a system catalog scan on the entry identity columns (e.g. table_name / table_oid).
class EntryScanFilters {
public:
	optional_ptr<const TableFilter> GetName() const {
		return name;
	}
	optional_ptr<const TableFilter> GetOid() const {
		return oid;
	}
	void SetName(optional_ptr<const TableFilter> filter) {
		name = filter;
	}
	void SetOid(optional_ptr<const TableFilter> filter) {
		oid = filter;
	}

	//! Whether the entry satisfies every filter present; a missing filter accepts everything.
	bool Accepts(ClientContext &context, const CatalogEntry &entry) const;
	//! The names the name filter restricts the entries to, when it is an equality or IN over constants (possibly
	//! AND-ed with other predicates). Returns false when there is no name filter or it has another shape, so every
	//! entry must be listed.
	bool TryGetNames(vector<string> &names) const;

private:
	optional_ptr<const TableFilter> name;
	optional_ptr<const TableFilter> oid;
};

} // namespace duckdb
