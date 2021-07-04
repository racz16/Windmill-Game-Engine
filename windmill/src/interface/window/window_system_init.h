#pragma once

#include <glm/vec2.hpp>
#include "../defines.h"

namespace wm {

	struct WM_PUBLIC window_system_init {
	public:
		std::string title = "Windmill";
		glm::ivec2 size = glm::ivec2(640, 480);
		bool fullscreen = false;
		bool visible = true;
		bool focused = true;
		bool maximized = false;
		bool focus_on_show = true;
		bool resizable = true;
		bool decorated = true;
		bool always_on_top = false;
		bool center_cursor = false;
	};

}
