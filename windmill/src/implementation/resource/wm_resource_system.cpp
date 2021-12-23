#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#include "defines/debug_defines.h"

#include "wm_resource_system.h"

namespace wm {

	ptr<image> wm_resource_system::get_image(const std::string& file_path) {
		int32_t width;
		int32_t height;
		int32_t channel_count;
		auto pixels = stbi_load(file_path.c_str(), &width, &height, &channel_count, STBI_rgb_alpha);
		WM_ASSERT(pixels);
		return ptr<image>(new image(glm::ivec2(width, height), channel_count, pixels));
	}

}
