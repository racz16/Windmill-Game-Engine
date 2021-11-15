#pragma once

#include "window_event.h"
#include "../../core/key.h"

namespace wm {

	class WM_PUBLIC window_maximization_event: public window_event {
	private:
		bool maximized;
	public:
		static key<window_maximization_event> get_key();

		window_maximization_event(const ptr<wm::window> window, const bool maximized);
		bool is_maximized() const;
		bool is_restored_from_maximization() const;
	};

}
