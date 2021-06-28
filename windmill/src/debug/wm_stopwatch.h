#pragma  once

#include <chrono>

#include "debug/stopwatch.h"

namespace wm {

	class wm_stopwatch: public stopwatch {
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> start_nanoseconds;
		float elapsed_nanoseconds;
	public:
		void start() override;
		void stop() override;
		float get_elapsed_nanoseconds() override;
		float get_elapsed_milliseconds() override;
		float get_elapsed_seconds() override;
	};

}
