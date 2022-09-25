#include "window_focus_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(window_focus_event, "WM_WINDOW_FOCUS_EVENT");

	window_focus_event::window_focus_event(const bool focus): event(true), focus(focus) { }

	bool window_focus_event::is_focused() const {
		return focus;
	}

	bool window_focus_event::is_lost_focus() const {
		return !focus;
	}

}
