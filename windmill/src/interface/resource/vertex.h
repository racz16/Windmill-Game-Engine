#pragma once

#include "../defines/general_defines.h"

namespace wm {

	class WM_PUBLIC vertex {
	private:
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texture_coordinate;
	public:
		vertex(const glm::vec3 position, const glm::vec3 normal, const glm::vec2 texture_coordinate);
		const glm::vec3& get_position() const;
		const glm::vec3& get_normal() const;
		const glm::vec2& get_texture_coordinate() const;
	};

}
