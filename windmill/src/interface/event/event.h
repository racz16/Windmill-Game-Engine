#pragma once

#include "../core/defines.h"

namespace wm {

	class WM_PUBLIC event {
	private:
		std::chrono::time_point<std::chrono::steady_clock> time;
		bool built_in;
	public:
		event(const bool built_in);
		std::chrono::time_point<std::chrono::steady_clock> get_time_point() const;
		bool is_built_in() const;
		virtual ~event() { }
	};

}
