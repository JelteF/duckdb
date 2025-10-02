#include "duckdb/parallel/lock_notifier.hpp"
#include "duckdb/main/client_context.hpp"
#include "duckdb/main/client_context_state.hpp"

namespace duckdb {

LockNotifier::LockNotifier(optional_ptr<ClientContext> context_p, const std::string &_lock_type)
    : context(context_p), lock_type(_lock_type) {
	if (context) {
		for (auto &state : context->registered_state->States()) {
			state->OnLockAcquisitionStart(*context, lock_type);
		}
	}
}

LockNotifier::~LockNotifier() {
	if (context) {
		for (auto &state : context->registered_state->States()) {
			state->OnLockAcquisitionEnd(*context, lock_type);
		}
	}
}

} // namespace duckdb
