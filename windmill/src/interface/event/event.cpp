#include "event.h"

namespace wm {

	event::event(const bool built_in) : built_in(built_in), time(std::chrono::steady_clock::now()) { }

	std::chrono::time_point<std::chrono::steady_clock> event::get_time_point() const {
		return time;
	}

	bool event::is_built_in() const {
		return built_in;
	}

}
