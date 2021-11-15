#include "mouse_position_event.h"

namespace wm {

	key<mouse_position_event> mouse_position_event::get_key() {
		static const key<mouse_position_event> key("WM_MOUSE_POSITION_EVENT");
		return key;
	}

	mouse_position_event::mouse_position_event(const ptr<wm::window> window, const glm::vec2 position): window_event(window), position(position) { }

	glm::vec2 mouse_position_event::get_new_position() const {
		return position;
	}

}
