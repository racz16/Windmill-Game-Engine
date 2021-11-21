#include "window_framebuffer_size_event.h"

namespace wm {

	key<window_framebuffer_size_event> window_framebuffer_size_event::get_key() {
		static const key<window_framebuffer_size_event> key("WM_WINDOW_FRAMEBUFFER_SIZE_EVENT");
		return key;
	}

	window_framebuffer_size_event::window_framebuffer_size_event(const ptr<wm::window> window, const glm::ivec2 size): window_event(window), size(size) { }

	glm::ivec2 window_framebuffer_size_event::get_new_size() const {
		return size;
	}

}
