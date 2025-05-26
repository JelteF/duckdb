#include "duckdb/parallel/task_notifier.hpp"
#include "duckdb/main/client_context.hpp"
#include "duckdb/main/client_context_state.hpp"

namespace duckdb {

TaskNotifier::TaskNotifier(optional_ptr<ClientContext> context_p, const std::string &_task_type)
    : context(context_p), task_type(_task_type) {
	if (context) {
		for (auto &state : context->registered_state->States()) {
			state->OnTaskStart(*context, task_type);
		}
	}
}

TaskNotifier::~TaskNotifier() {
	if (context) {
		for (auto &state : context->registered_state->States()) {
			state->OnTaskStop(*context, task_type);
		}
	}
}

} // namespace duckdb
