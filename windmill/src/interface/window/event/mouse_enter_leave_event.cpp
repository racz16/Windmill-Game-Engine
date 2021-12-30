#include "mouse_enter_leave_event.h"

namespace wm {

	key<mouse_enter_leave_event> mouse_enter_leave_event::get_key() {
		static const key<mouse_enter_leave_event> key("WM_MOUSE_ENTER_LEAVE_EVENT");
		return key;
	}

	mouse_enter_leave_event::mouse_enter_leave_event(const bool enter): event(true), enter(enter) { }

	bool mouse_enter_leave_event::is_enter() const {
		return enter;
	}

	bool mouse_enter_leave_event::is_leave() const {
		return !enter;
	}

}
