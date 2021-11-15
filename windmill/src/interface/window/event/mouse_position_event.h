#pragma once

#include "window_event.h"
#include "../../core/key.h"

namespace wm {

	class WM_PUBLIC mouse_position_event: public window_event {
	private:
		glm::vec2 position;
	public:
		static key<mouse_position_event> get_key();

		mouse_position_event(const ptr<wm::window> window, const glm::vec2 position);
		glm::vec2 get_new_position() const;
	};

}
