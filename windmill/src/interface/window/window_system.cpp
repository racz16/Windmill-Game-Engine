#include "window/window_system.h"
#include "../../window/wm_glfw_window_system.h"

namespace wm {

	std::shared_ptr<window_system> window_system::get_instance(const window_system_init init) {
		static auto instance = std::make_shared<wm_glfw_window_system>(init);
		return instance;
	}

}
