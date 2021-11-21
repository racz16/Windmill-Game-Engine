#include "window_minimization_event.h"

namespace wm {

	key<window_minimization_event> window_minimization_event::get_key() {
		static const key<window_minimization_event> key("WM_WINDOW_MINIMIZATION_EVENT");
		return key;
	}

	window_minimization_event::window_minimization_event(const ptr<wm::window> window, const bool minimized): window_event(window), minimized(minimized) { }

	bool window_minimization_event::is_minimized() const {
		return minimized;
	}

	bool window_minimization_event::is_restored_from_minimization() const {
		return !minimized;
	}

}
