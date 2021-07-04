#include "wm_engine.h"
#include "debug/log/wm_scoped_time_logger.h"
#include "window/wm_glfw_window_system.h"

namespace wm {

	void wm_engine::initialize() { }

	void wm_engine::run() {
		wm_scoped_time_logger stl {"elapsed time in \"run\""};
		try {
			while(!engine::get_window_system()->is_closing()) {
				engine::get_window_system()->update();
			}
		} catch(const std::exception& ex) {
			WM_LOG_ERROR(ex.what());
			WM_BREAKPOINT();
		} catch(...) {
			WM_LOG_ERROR("Unknown exception");
			WM_BREAKPOINT();
		}
	}

}
