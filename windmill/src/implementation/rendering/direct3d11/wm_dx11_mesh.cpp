#include "wm_dx11_mesh.h"

namespace wm {

	wm_dx11_mesh::wm_dx11_mesh(const gpu_mesh_descriptor descriptor): vertex_buffer(descriptor.vertex_buffer), index_buffer(descriptor.index_buffer) {}

	std::any wm_dx11_mesh::get_native_handle() const {
		return nullptr;
	}

	int32_t wm_dx11_mesh::get_data_size() const {
		return 0;
	}

	ptr<gpu_buffer> wm_dx11_mesh::get_vertex_buffer() const {
		return vertex_buffer;
	}

	ptr<gpu_buffer> wm_dx11_mesh::get_index_buffer() const {
		return index_buffer;
	}

#ifdef WM_BUILD_DEBUG

	void wm_dx11_mesh::set_debug_label(const std::string& label) {}

#endif

	wm_dx11_mesh::~wm_dx11_mesh() {
		if(index_buffer.is_valid()) {
			index_buffer.destroy();
		}
		if(vertex_buffer.is_valid()) {
			vertex_buffer.destroy();
		}
	}

}