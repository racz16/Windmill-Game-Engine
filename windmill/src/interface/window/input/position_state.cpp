#include "position_state.h"

namespace wm {

	position_state::position_state(const glm::dvec2 previous_position, const glm::dvec2 position): previous_position(previous_position), position(position) { }

	glm::dvec2 position_state::get_movement() const {
		return position - previous_position;
	}

	glm::dvec2 position_state::get_position() const {
		return position;
	}

}
