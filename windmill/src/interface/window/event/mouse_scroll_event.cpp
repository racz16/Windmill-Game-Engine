#include "mouse_scroll_event.h"

namespace wm {

	key<mouse_scroll_event> mouse_scroll_event::get_key() {
		static const key<mouse_scroll_event> key("WM_MOUSE_SCROLL_EVENT");
		return key;
	}

	mouse_scroll_event::mouse_scroll_event(const ptr<wm::window> window, const glm::dvec2 offset): window_event(window), offset(offset) { }

	glm::dvec2 mouse_scroll_event::get_offset() const {
		return offset;
	}

}
