#include "image.h"

namespace wm {

	image::image(const glm::ivec2 size, const int32_t channel_count, unsigned char* pixels): size(size), channel_count(channel_count), pixels(pixels) { }

	glm::ivec2 image::get_size() const {
		return size;
	}

	int32_t image::get_channel_count() const {
		return channel_count;
	}

	unsigned char* image::get_pixels() const {
		return pixels;
	}

	image::~image() {
		stbi_image_free(pixels);
		pixels = nullptr;
	}

}
