//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parallel/lock_notifier.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/optional_ptr.hpp"

namespace duckdb {
class ClientContext;

//! The LockNotifier notifies ClientContextState listener about acquired / released locks
class LockNotifier {
public:
	explicit LockNotifier(optional_ptr<ClientContext> context_p, const std::string &lock_type);

	~LockNotifier();

private:
	optional_ptr<ClientContext> context;
	const std::string lock_type;
};

} // namespace duckdb
