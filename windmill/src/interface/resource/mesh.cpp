#include "mesh.h"

namespace wm {

	mesh::mesh(const std::vector<vertex>& vertices, const std::vector<uint32_t> indices, const bool texture_coordinates): vertices(vertices), indices(indices), texture_coordinates(texture_coordinates) { }

	const std::vector<vertex>& mesh::get_vertices() const {
		return vertices;
	}

	const std::vector<uint32_t>& mesh::get_indices() const {
		return indices;
	}

	bool mesh::has_texture_coordinates() const {
		return texture_coordinates;
	}

}
