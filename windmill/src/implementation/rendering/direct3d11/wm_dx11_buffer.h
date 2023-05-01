#pragma once

#include "rendering/gpu_buffer.h"

namespace wm {

	class wm_dx11_buffer: public gpu_buffer {
	private:
		ID3D11Buffer* native_handle;
		gpu_buffer_type type_flags;
		usage_frequency cpu_write_frequency;
		int32_t data_size;
		int32_t stride;

		UINT get_bind_flags() const;
		UINT get_cpu_access_flags() const;
		D3D11_USAGE get_usage() const;
	public:
		wm_dx11_buffer(const gpu_buffer_descriptor descriptor);
		std::any get_native_handle() const override;
		int32_t get_data_size() const override;
		gpu_buffer_type get_type_flags() const override;
		usage_frequency get_cpu_write_frequency() const override;
	#ifdef WM_BUILD_DEBUG
		void set_debug_label(const std::string& label) override;
	#endif
		void set_data(void* data, const int32_t data_size, const int32_t offset = 0) override;
		int32_t get_stride() const override;
		~wm_dx11_buffer();
	};

}
