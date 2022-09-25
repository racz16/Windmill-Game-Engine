#include "window_drag_and_drop_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(window_drag_and_drop_event, "WM_WINDOW_DRAG_AND_DROP_EVENT");

	window_drag_and_drop_event::window_drag_and_drop_event(const std::vector<std::string>& paths): event(true), paths(paths) { }

	std::vector<std::string> window_drag_and_drop_event::get_paths() const {
		return paths;
	}

}
