#include "window_focus_event.h"

namespace wm {

	key<window_focus_event> window_focus_event::get_key() {
		static const key<window_focus_event> key("WM_WINDOW_FOCUS_EVENT");
		return key;
	}

	window_focus_event::window_focus_event(const bool focus): event(true), focus(focus) { }

	bool window_focus_event::is_focused() const {
		return focus;
	}

	bool window_focus_event::is_lost_focus() const {
		return !focus;
	}

}
