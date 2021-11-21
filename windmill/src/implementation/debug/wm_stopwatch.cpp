#include "wm_stopwatch.h"

namespace wm {

	void wm_stopwatch::start() {
		start_time_point = std::chrono::steady_clock::now();
	}

	void wm_stopwatch::stop() {
		const auto stop_time_point = std::chrono::steady_clock::now();
		elapsed_duration = stop_time_point - start_time_point;
	}

	double wm_stopwatch::get_elapsed_time() const {
		return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(elapsed_duration).count();
	}

}
