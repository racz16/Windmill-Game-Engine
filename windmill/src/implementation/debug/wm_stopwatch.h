#pragma  once

#include "debug/stopwatch.h"

namespace wm {

	class wm_stopwatch: public stopwatch {
	private:
		std::chrono::time_point<std::chrono::steady_clock> start_time_point;
		std::chrono::nanoseconds elapsed_duration;
	public:
		void start() override;
		void stop() override;
		double get_elapsed_time() const override;
	};

}
