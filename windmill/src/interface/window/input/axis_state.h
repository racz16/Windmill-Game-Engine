#pragma once

#include "../../defines/general_defines.h"

namespace wm {

	class WM_PUBLIC axis_state {
	private:
		float previous_value;
		float value;
	public:
		axis_state(const float previous_value, const float value);
		bool is_in_the_middle(const float offset = 0.00001f) const;
		bool is_on_the_edge(const float offset = 0.00001f) const;
		float get_movement() const;
		float get_value() const;
	};

}
