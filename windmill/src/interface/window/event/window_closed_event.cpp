#include "window_closed_event.h"

namespace wm {

	key<window_closed_event> window_closed_event::get_key() {
		static const key<window_closed_event> key("WM_WINDOW_CLOSED_EVENT");
		return key;
	}

	window_closed_event::window_closed_event(): event(true) { }

}
