#include "window_size_event.h"

namespace wm {

	key<window_size_event> window_size_event::get_key() {
		static const key<window_size_event> key("WM_WINDOW_SIZE_EVENT");
		return key;
	}

	window_size_event::window_size_event(const glm::ivec2& size): event(true), size(size) { }

	glm::ivec2 window_size_event::get_new_size() const {
		return size;
	}

}
