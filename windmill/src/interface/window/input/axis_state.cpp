#include "axis_state.h"

namespace wm {

	axis_state::axis_state(const float previous_value, const float value): previous_value(previous_value), value(value) { }

	bool axis_state::is_in_the_middle() const {
		return fabs(value) <= 0.0000155;
	}

	bool axis_state::is_on_the_edge() const {
		return value == 1.0 || value == -1.0;
	}

	float axis_state::get_movement() const {
		return value - previous_value;
	}

	float axis_state::get_value() const {
		return value;
	}

}
