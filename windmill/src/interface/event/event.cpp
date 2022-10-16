#include "event.h"

namespace wm {

	event::event(const bool built_in) : built_in(built_in), time(std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(std::chrono::steady_clock::now().time_since_epoch()).count()) { }

	double event::get_time_point() const {
		return time;
	}

	bool event::is_built_in() const {
		return built_in;
	}

}
