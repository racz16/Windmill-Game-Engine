#pragma once

#include "gpu_buffer.h"
#include "../core/key.h"
#include "../ptr/ptr.h"

namespace wm {

	class WM_PUBLIC gpu_mesh_descriptor {
	public:
		ptr<gpu_buffer> vertex_buffer = nullptr;
		ptr<gpu_buffer> index_buffer = nullptr;
	};

	class WM_PUBLIC gpu_mesh: public gpu_resource {
	public:
		static ptr<gpu_mesh> create(const gpu_mesh_descriptor descriptor);
		static key<gpu_mesh> get_key();

		virtual ptr<gpu_buffer> get_vertex_buffer() const = 0;
		virtual ptr<gpu_buffer> get_index_buffer() const = 0;
	};

}
