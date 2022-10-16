#pragma once

#include "../../event/event.h"
#include "../../core/key.h"

namespace wm {

	class WM_PUBLIC mouse_hover_event: public event {
	private:
		bool enter;
	public:
		static key<mouse_hover_event> get_key();

		mouse_hover_event(const bool enter);
		bool is_entering_window() const;
		bool is_leaving_window() const;
	};

}
