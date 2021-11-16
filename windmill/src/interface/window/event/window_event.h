#pragma once

#include "../../ptr/ptr.h"
#include "../../event/event.h"
#include "../../window/window.h"

namespace wm {

	class WM_PUBLIC window_event: public event {
	private:
		ptr<wm::window> window;
	public:
		window_event(const ptr<wm::window> window);
		ptr<wm::window> get_window() const;
	};

}
