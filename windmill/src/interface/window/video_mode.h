#pragma once

#include "../core/defines.h"

namespace wm {

	class WM_PUBLIC video_mode {
	private:
		glm::ivec2 size;
		glm::ivec3 bit_depth;
		int32_t refresh_rate;
	public:
		video_mode(const glm::ivec2& size, const glm::ivec3& bit_depth, const int32_t refresh_rate);
		glm::ivec2 get_size() const;
		glm::ivec3 get_bit_depth() const;
		int32_t get_refresh_rate() const;
	};

}
