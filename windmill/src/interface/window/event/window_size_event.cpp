#include "window_size_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(window_size_event, "WM_WINDOW_SIZE_EVENT");

	window_size_event::window_size_event(const glm::ivec2& size): event(true), size(size) { }

	glm::ivec2 window_size_event::get_new_size() const {
		return size;
	}

}
