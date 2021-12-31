#pragma once

#include "../../core/key.h"
#include "../../event/event.h"

namespace wm {

	class WM_PUBLIC window_closed_event: public event {
	public:
		static key<window_closed_event> get_key();

		window_closed_event();
	};

}
