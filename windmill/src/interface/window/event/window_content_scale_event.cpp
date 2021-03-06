#include "window_content_scale_event.h"

namespace wm {

	key<window_content_scale_event> window_content_scale_event::get_key() {
		static const key<window_content_scale_event> key("WM_WINDOW_CONTENT_SCALE_EVENT");
		return key;
	}

	window_content_scale_event::window_content_scale_event(const glm::vec2& scale): event(true), scale(scale) { }

	glm::vec2 window_content_scale_event::get_new_scale() const {
		return scale;
	}

}
