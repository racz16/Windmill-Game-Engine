#include "wm_stopwatch.h"

namespace wm {

	void wm_stopwatch::start() {
		start_nanoseconds = std::chrono::high_resolution_clock::now();
	}

	void wm_stopwatch::stop() {
		auto stop_nanoseconds = std::chrono::high_resolution_clock::now();
		elapsed_nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_nanoseconds - start_nanoseconds).count();
	}

	float wm_stopwatch::get_elapsed_nanoseconds() {
		return elapsed_nanoseconds;
	}

	float wm_stopwatch::get_elapsed_milliseconds() {
		return elapsed_nanoseconds / 1000.0f / 1000.0f;
	}

	float wm_stopwatch::get_elapsed_seconds() {
		return get_elapsed_milliseconds() / 1000.0f / 1000.0f;
	}

}
