#include "mouse_hover_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(mouse_hover_event, "WM_MOUSE_HOVER_EVENT");

	mouse_hover_event::mouse_hover_event(const bool enter): event(true), enter(enter) { }

	bool mouse_hover_event::is_entering_window() const {
		return enter;
	}

	bool mouse_hover_event::is_leaving_window() const {
		return !enter;
	}

}
