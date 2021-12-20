#pragma once

namespace wm {

	static const int32_t GAMEPAD_AXIS_COUNT = 6;

	enum class gamepad_axis {
		axis_left_x = 0,
		axis_left_y = 1,
		axis_right_x = 2,
		axis_right_y = 3,
		axis_left_trigger = 4,
		axis_right_trigger = 5,
	};

}
