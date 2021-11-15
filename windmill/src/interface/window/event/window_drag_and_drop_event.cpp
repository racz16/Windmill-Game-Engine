#include "window_drag_and_drop_event.h"

namespace wm {

	key<window_drag_and_drop_event> window_drag_and_drop_event::get_key() {
		static const key<window_drag_and_drop_event> key("WM_WINDOW_DRAG_AND_DROP_EVENT");
		return key;
	}

	window_drag_and_drop_event::window_drag_and_drop_event(const ptr<wm::window> window, const std::vector<std::string> paths): window_event(window), paths(paths) { }

	std::vector<std::string> window_drag_and_drop_event::get_paths() const {
		return paths;
	}

}
