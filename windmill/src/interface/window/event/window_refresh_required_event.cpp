#include "window_refresh_required_event.h"

namespace wm {

	key<window_refresh_required_event> window_refresh_required_event::get_key() {
		static const key<window_refresh_required_event> key("WM_WINDOW_REFRESH_REQUIRED_EVENT");
		return key;
	}

	window_refresh_required_event::window_refresh_required_event(const ptr<wm::window> window): window_event(window) { }

}
