#pragma once

#include "defines.h"

namespace wm {

	class WM_PUBLIC application {
	private:
		std::string name;
	protected:
		virtual void initialize();
		virtual void add_log_system();
		virtual void add_window_system();
		virtual void loop();
		virtual void destroy();
	public:
		application(const std::string& name = "application");
		virtual void run();
		virtual ~application() { }
	};

}
