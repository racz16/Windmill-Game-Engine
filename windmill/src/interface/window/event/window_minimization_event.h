#pragma once

#include "../../event/event.h"
#include "../../core/key.h"

namespace wm {

	class WM_PUBLIC window_minimization_event: public event {
	private:
		bool minimized;
	public:
		static key<window_minimization_event> get_key();

		window_minimization_event(const bool minimized);
		bool is_minimized() const;
		bool is_restored_from_minimization() const;
	};

}
