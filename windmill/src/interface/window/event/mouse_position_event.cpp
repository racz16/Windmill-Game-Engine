#include "mouse_position_event.h"

namespace wm {

	key<mouse_position_event> mouse_position_event::get_key() {
		static const key<mouse_position_event> key("WM_MOUSE_POSITION_EVENT");
		return key;
	}

	mouse_position_event::mouse_position_event(const glm::dvec2& position): event(true), position(position) { }

	glm::dvec2 mouse_position_event::get_new_position() const {
		return position;
	}

}
