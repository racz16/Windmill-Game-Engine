#pragma once

#include "../../event/event.h"
#include "../../core/key.h"

namespace wm {

	class WM_PUBLIC window_position_event: public event {
	private:
		glm::ivec2 position;
	public:
		static key<window_position_event> get_key();

		window_position_event(const glm::ivec2& position);
		glm::ivec2 get_new_position() const;
	};

}
