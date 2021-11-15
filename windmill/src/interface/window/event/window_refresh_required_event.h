#pragma once

#include "../../core/key.h"
#include "window_event.h"

namespace wm {

	class WM_PUBLIC window_refresh_required_event: public window_event {
	public:
		static key<window_refresh_required_event> get_key();

		window_refresh_required_event(const ptr<wm::window> window);
	};

}
