#pragma once

#include "vertex.h"
#include "../defines/general_defines.h"

namespace wm {

	class WM_PUBLIC mesh {
	private:
		std::vector<vertex> vertices;
		std::vector<uint32_t> indices;
		bool texture_coordinates;
	public:
		mesh(const std::vector<vertex> vertices, const std::vector<uint32_t> indices, const bool texture_coordinates);
		const std::vector<vertex>& get_vertices() const;
		const std::vector<uint32_t>& get_indices() const;
		bool has_texture_coordinates() const;
	};

}
