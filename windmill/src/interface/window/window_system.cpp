#include "window/window_system.h"
#include "defines/code_generation_defines.h"

#include "../../implementation/window/wm_glfw_window_system.h"

namespace wm {

	ptr<window_system> window_system::create(const glm::ivec2& size, const std::string& title, const bool fullscreen, const bool visible) {
		return ptr<window_system>(new wm_glfw_window_system(size, title, fullscreen, visible)).convert<window_system>();
	}

	WM_GET_KEY(window_system, "WM_WINDOW_SYSTEM");

}
