#pragma once

#include "../defines/general_defines.h"

namespace wm {

	class WM_PUBLIC event {
	private:
		double time;
		bool built_in;
	public:
		event(const bool built_in = false);
		double get_time_point() const;
		bool is_built_in() const;
		virtual ~event() = default;
	};

}
