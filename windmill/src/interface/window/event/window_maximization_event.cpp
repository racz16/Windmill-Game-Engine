#include "window_maximization_event.h"

namespace wm {

	key<window_maximization_event> window_maximization_event::get_key() {
		static const key<window_maximization_event> key("WM_WINDOW_MAXIMIZATION_EVENT");
		return key;
	}

	window_maximization_event::window_maximization_event(const ptr<wm::window> window, const bool maximized): window_event(window), maximized(maximized) { }

	bool window_maximization_event::is_maximized() const {
		return maximized;
	}

	bool window_maximization_event::is_restored_from_maximization() const {
		return !maximized;
	}

}
