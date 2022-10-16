#include "defines/log_defines.h"

#include "wm_scoped_time_logger.h"

namespace wm {

	wm_scoped_time_logger::wm_scoped_time_logger(const std::string& name): name(name) {
		stopwatch.start();
	}

	wm_scoped_time_logger::~wm_scoped_time_logger() {
		stopwatch.stop();
		WM_LOG_DEBUG(
			name +
			std::string(": ") +
			std::to_string(stopwatch.get_elapsed_time()) +
			std::string(" ms")
		);
	}

}
