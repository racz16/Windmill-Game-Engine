#pragma once

#include "defines.h"
#include "debug/log/log_system.h"
#include "window/window_system.h"

namespace wm {

	class WM_PUBLIC engine {
	public:
		static std::shared_ptr<engine> get_instance();
		static std::shared_ptr<log_system> get_log_system();
		static std::shared_ptr<window_system>get_window_system();

		virtual void initialize() = 0;
		virtual void run() = 0;
	};

}
