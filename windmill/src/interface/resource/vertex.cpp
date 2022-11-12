#include "vertex.h"

namespace wm {

	vertex::vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texture_coordinate): position(position), normal(normal), texture_coordinate(texture_coordinate) { }

	glm::vec3 vertex::get_position() const {
		return position;
	}

	glm::vec3 vertex::get_normal() const {
		return normal;
	}

	glm::vec2 vertex::get_texture_coordinate() const {
		return texture_coordinate;
	}

}
