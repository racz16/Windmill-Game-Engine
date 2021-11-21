#include "window/window_system.h"

#include "../../implementation/window/wm_glfw_window_system.h"

namespace wm {

	ptr<window_system> window_system::get_instance() {
		return wm_glfw_window_system::get_instance().convert<window_system>();
	}

	key<window_system> window_system::get_key() {
		static const key<window_system> key("WM_WINDOW_SYSTEM");
		return key;
	}

}
