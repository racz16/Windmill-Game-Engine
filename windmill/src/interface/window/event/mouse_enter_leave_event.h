#pragma once

#include "window_event.h"
#include "../../core/key.h"

namespace wm {

	class WM_PUBLIC mouse_enter_leave_event: public window_event {
	private:
		bool enter;
	public:
		static key<mouse_enter_leave_event> get_key();

		mouse_enter_leave_event(const ptr<wm::window> window, const bool enter);
		bool is_enter() const;
		bool is_leave() const;
	};

}
