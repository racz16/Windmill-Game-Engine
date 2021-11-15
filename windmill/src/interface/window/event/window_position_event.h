#pragma once

#include "window_event.h"
#include "../../core/key.h"

namespace wm {

	class WM_PUBLIC window_position_event: public window_event {
	private:
		glm::ivec2 position;
	public:
		static key<window_position_event> get_key();

		window_position_event(const ptr<wm::window> window, const glm::ivec2 position);
		glm::ivec2 get_new_position() const;
	};

}
