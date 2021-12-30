#pragma once

#include "../../event/event.h"
#include "../../core/key.h"

namespace wm {

	class WM_PUBLIC window_drag_and_drop_event: public event {
	private:
		std::vector<std::string> paths;
	public:
		static key<window_drag_and_drop_event> get_key();

		window_drag_and_drop_event(const std::vector<std::string> paths);
		std::vector<std::string> get_paths() const;
	};

}
