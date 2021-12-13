#pragma once

#include "../../defines/general_defines.h"

namespace wm {

	class WM_PUBLIC position_state {
	private:
		glm::dvec2 previous_position;
		glm::dvec2 position;
	public:
		position_state(const glm::dvec2 previous_position, const glm::dvec2 position);
		glm::dvec2 get_movement() const;
		glm::dvec2 get_position() const;
	};

}
