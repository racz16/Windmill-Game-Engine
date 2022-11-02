#pragma once

namespace wm {

	enum class audio_distance_model {
		none,
		inverse_distance,
		inverse_distance_clamped,
		linear_distance,
		linear_distance_clamped,
		exponent_distance,
		exponent_distance_clamped,
	};

}
