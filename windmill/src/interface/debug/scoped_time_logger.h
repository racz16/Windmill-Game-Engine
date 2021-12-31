#pragma once

#include "../defines/general_defines.h"

namespace wm {

	class WM_PUBLIC scoped_time_logger {
	public:
		static std::unique_ptr<scoped_time_logger> create();

		virtual ~scoped_time_logger() { }
	};

}
