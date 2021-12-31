#include "window_position_event.h"

namespace wm {

	key<window_position_event> window_position_event::get_key() {
		static const key<window_position_event> key("WM_WINDOW_POSITION_EVENT");
		return key;
	}

	window_position_event::window_position_event(const glm::ivec2& position): event(true), position(position) { }

	glm::ivec2 window_position_event::get_new_position() const {
		return position;
	}

}
