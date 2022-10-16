#include "window_maximization_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(window_maximization_event, "WM_WINDOW_MAXIMIZATION_EVENT");

	window_maximization_event::window_maximization_event(const bool maximized):event(true), maximized(maximized) { }

	bool window_maximization_event::is_maximized() const {
		return maximized;
	}

	bool window_maximization_event::is_restored_from_maximization() const {
		return !maximized;
	}

}
