#include "wm_stopwatch.h"

namespace wm {

	void wm_stopwatch::start() {
		start_nanoseconds = std::chrono::high_resolution_clock::now();
	}

	void wm_stopwatch::stop() {
		auto stop_nanoseconds = std::chrono::high_resolution_clock::now();
		auto elapsed_time = stop_nanoseconds - start_nanoseconds;
		elapsed_nanoseconds = std::chrono::duration_cast<std::chrono::duration<float, std::nano>>(elapsed_time).count();
	}

	float wm_stopwatch::get_elapsed_nanoseconds() const {
		return elapsed_nanoseconds;
	}

	float wm_stopwatch::get_elapsed_milliseconds() const {
		return elapsed_nanoseconds / 1000.0f / 1000.0f;
	}

	float wm_stopwatch::get_elapsed_seconds() const {
		return get_elapsed_milliseconds() / 1000.0f / 1000.0f;
	}

}
