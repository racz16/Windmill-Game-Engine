#pragma once

#include "rendering/gpu_mesh.h"

namespace wm {

	class wm_dx11_mesh: public gpu_mesh {
	private:
		ptr<gpu_buffer> vertex_buffer;
		ptr<gpu_buffer> index_buffer;
	public:
		wm_dx11_mesh(const gpu_mesh_descriptor descriptor);
		std::any get_native_handle() const override;
		int32_t get_data_size() const override;
		ptr<gpu_buffer> get_vertex_buffer() const override;
		ptr<gpu_buffer> get_index_buffer() const override;
	#ifdef WM_BUILD_DEBUG
		void set_debug_label(const std::string& label) override;
	#endif
		~wm_dx11_mesh();
	};

}
