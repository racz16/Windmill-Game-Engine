#include "window_focus_event.h"

namespace wm {

	key<window_focus_event> window_focus_event::get_key() {
		static const key<window_focus_event> key("WM_WINDOW_FOCUS_EVENT");
		return key;
	}

	window_focus_event::window_focus_event(const ptr<wm::window> window, const bool focus): window_event(window), focus(focus) { }

	bool window_focus_event::is_focused() const {
		return focus;
	}

	bool window_focus_event::is_lost_focus() const {
		return !focus;
	}

}
