#include "wm_scoped_time_logger.h"
#include "core/engine.h"

namespace wm {

	wm_scoped_time_logger::wm_scoped_time_logger(const std::string& name) {
		this->name = name;
		stopwatch.start();
	}

	wm_scoped_time_logger::~wm_scoped_time_logger() {
		stopwatch.stop();
		WM_LOG_DEBUG(
			name +
			std::string(": ") +
			std::to_string(stopwatch.get_elapsed_milliseconds()) +
			std::string(" ms")
		);
	}

}
