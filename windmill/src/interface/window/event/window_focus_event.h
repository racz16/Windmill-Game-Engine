#pragma once

#include "window_event.h"
#include "../../core/key.h"

namespace wm {

	class WM_PUBLIC window_focus_event: public window_event {
	private:
		bool focus;
	public:
		static key<window_focus_event> get_key();

		window_focus_event(const ptr<wm::window> window, const bool focus);
		bool is_focused() const;
		bool is_lost_focus() const;
	};

}
