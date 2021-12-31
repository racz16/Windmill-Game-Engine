#pragma once

#include "../../core/key.h"
#include "../../event/event.h"

namespace wm {

	class WM_PUBLIC window_refresh_required_event: public event {
	public:
		static key<window_refresh_required_event> get_key();

		window_refresh_required_event();
	};

}
