#include "window_position_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(window_position_event, "WM_WINDOW_POSITION_EVENT");

	window_position_event::window_position_event(const glm::ivec2& position): event(true), position(position) { }

	glm::ivec2 window_position_event::get_new_position() const {
		return position;
	}

}
