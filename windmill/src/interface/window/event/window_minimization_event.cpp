#include "window_minimization_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(window_minimization_event, "WM_WINDOW_MINIMIZATION_EVENT");

	window_minimization_event::window_minimization_event(const bool minimized): event(true), minimized(minimized) { }

	bool window_minimization_event::is_minimized() const {
		return minimized;
	}

	bool window_minimization_event::is_restored_from_minimization() const {
		return !minimized;
	}

}
