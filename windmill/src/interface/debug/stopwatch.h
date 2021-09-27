#pragma once

#include "../core/defines.h"
#include "../core/ptr.h"

namespace wm {

	class WM_PUBLIC stopwatch {
	public:
		static ptr<stopwatch> create();

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual float get_elapsed_nanoseconds() const = 0;
		virtual float get_elapsed_milliseconds() const = 0;
		virtual float get_elapsed_seconds() const = 0;
		virtual ~stopwatch() { }
	};

}
