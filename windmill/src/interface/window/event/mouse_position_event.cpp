#include "mouse_position_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(mouse_position_event, "WM_MOUSE_POSITION_EVENT");

	mouse_position_event::mouse_position_event(const glm::dvec2& position): event(true), position(position) { }

	glm::dvec2 mouse_position_event::get_new_position() const {
		return position;
	}

}
