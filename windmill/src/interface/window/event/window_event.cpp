#include "window_event.h"

namespace wm {

	window_event::window_event(const ptr<wm::window> window): event(true), window(window) { }

	ptr<wm::window> window_event::get_window() const {
		return window;
	}

}
