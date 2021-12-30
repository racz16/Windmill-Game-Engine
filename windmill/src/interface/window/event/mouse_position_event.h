#pragma once

#include "../../event/event.h"
#include "../../core/key.h"

namespace wm {

	class WM_PUBLIC mouse_position_event: public event {
	private:
		glm::dvec2 position;
	public:
		static key<mouse_position_event> get_key();

		mouse_position_event(const glm::dvec2& position);
		glm::dvec2 get_new_position() const;
	};

}
