#pragma once

#include "../../event/event.h"
#include "../../core/key.h"

namespace wm {

	class WM_PUBLIC mouse_enter_leave_event: public event {
	private:
		bool enter;
	public:
		static key<mouse_enter_leave_event> get_key();

		mouse_enter_leave_event(const bool enter);
		bool is_enter() const;
		bool is_leave() const;
	};

}
