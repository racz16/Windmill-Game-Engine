#pragma once

#include "defines.h"
#include "debug/log/log_system.h"

namespace wm {

	class WM_PUBLIC engine {
	public:
		static std::shared_ptr<engine> create();
		static std::shared_ptr<log_system> get_log_system();

		virtual void initialize() = 0;
		virtual void run() = 0;
		virtual void destroy() = 0;
	};

}
