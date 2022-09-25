#include "mouse_scroll_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(mouse_scroll_event, "WM_MOUSE_SCROLL_EVENT");

	mouse_scroll_event::mouse_scroll_event(const glm::dvec2& offset): event(true), offset(offset) { }

	glm::dvec2 mouse_scroll_event::get_offset() const {
		return offset;
	}

}
