#pragma once

#include "../../core/key.h"
#include "window_event.h"

namespace wm {

	class WM_PUBLIC window_closed_event: public window_event {
	public:
		static key<window_closed_event> get_key();

		window_closed_event(const ptr<wm::window> window);
	};

}
