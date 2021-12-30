#include "window_minimization_event.h"

namespace wm {

	key<window_minimization_event> window_minimization_event::get_key() {
		static const key<window_minimization_event> key("WM_WINDOW_MINIMIZATION_EVENT");
		return key;
	}

	window_minimization_event::window_minimization_event(const bool minimized): event(true), minimized(minimized) { }

	bool window_minimization_event::is_minimized() const {
		return minimized;
	}

	bool window_minimization_event::is_restored_from_minimization() const {
		return !minimized;
	}

}
