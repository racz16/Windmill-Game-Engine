#pragma once

#include "../core/defines.h"
#include "../core/system.h"
#include "../ptr/ptr.h"
#include "../core/key.h"

namespace wm {

	class WM_PUBLIC time_system: public system {
	public:
		static ptr<time_system> create();
		static key<time_system> get_key();

		virtual double get_time() const = 0;
		virtual double get_delta_time() const = 0;
		virtual double get_fps() const = 0;
		virtual double get_frame_time() const = 0;
		virtual double get_average_frame_time() const = 0;
	};

}
