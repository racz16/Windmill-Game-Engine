#pragma once

#include "../defines/general_defines.h"

namespace wm {

	class WM_PUBLIC application {
	protected:
		virtual void initialize();
		virtual void add_log_system();
		virtual void add_event_system();
		virtual void add_time_system();
		virtual void add_window_system();
		virtual void add_rendering_system();
		virtual bool loop_condition();
		virtual void loop();
		virtual void destroy();
	public:
		application(const std::string& name = "application");
		virtual void run();
		virtual ~application() { }
	};

}
