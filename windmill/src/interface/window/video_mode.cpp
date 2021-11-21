#include "video_mode.h"

namespace wm {

	video_mode::video_mode(const glm::ivec2& size, const glm::ivec3& bit_depth, const int32_t refresh_rate)
		: size(size), bit_depth(bit_depth), refresh_rate(refresh_rate) { }

	glm::ivec2 video_mode::get_size() const {
		return size;
	}

	glm::ivec3 video_mode::get_bit_depth() const {
		return bit_depth;
	}

	int32_t video_mode::get_refresh_rate() const {
		return refresh_rate;
	}

}
