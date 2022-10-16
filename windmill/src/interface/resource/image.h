#pragma once

#include "../defines/general_defines.h"

namespace wm {

	class WM_PUBLIC image {
	private:
		const glm::ivec2 size;
		const int32_t channel_count;
		unsigned char* pixels;
	public:
		image(const glm::ivec2& size, const int32_t channel_count, unsigned char* pixels);
		glm::ivec2 get_size() const;
		int32_t get_channel_count() const;
		unsigned char* get_pixels() const;
		virtual ~image();
	};

}
