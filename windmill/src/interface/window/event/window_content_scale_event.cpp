#include "window_content_scale_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(window_content_scale_event, "WM_WINDOW_CONTENT_SCALE_EVENT");

	window_content_scale_event::window_content_scale_event(const glm::vec2& scale): event(true), scale(scale) { }

	glm::vec2 window_content_scale_event::get_new_scale() const {
		return scale;
	}

}
