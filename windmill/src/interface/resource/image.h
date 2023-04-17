#pragma once

#include "../defines/general_defines.h"

namespace wm {

	class WM_PUBLIC image {
	private:
		const glm::ivec2 size;
		const int32_t channel_count;
		uint8_t* pixels;
	public:
		image(const glm::ivec2& size, const int32_t channel_count, uint8_t* pixels);
		glm::ivec2 get_size() const;
		int32_t get_channel_count() const;
		uint8_t* get_pixels() const;
		virtual ~image();
	};

}
