#pragma once

#include "../defines.h"

namespace wm {

	class WM_PUBLIC stopwatch {
	public:
		static std::shared_ptr<stopwatch> create();

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual float get_elapsed_nanoseconds() = 0;
		virtual float get_elapsed_milliseconds() = 0;
		virtual float get_elapsed_seconds() = 0;
		virtual ~stopwatch() { };
	};

}
