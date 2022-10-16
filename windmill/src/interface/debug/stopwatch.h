#pragma once

#include "../ptr/ptr.h"

namespace wm {

	class WM_PUBLIC stopwatch {
	public:
		static ptr<stopwatch> create();

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual double get_elapsed_time() const = 0;
		virtual ~stopwatch() = default;
	};

}
