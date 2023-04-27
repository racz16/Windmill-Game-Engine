#pragma once

#include <glad/glad.h>

#include "rendering/gpu_buffer.h"

namespace wm {

	class wm_gl_buffer: public gpu_buffer {
	private:
		GLuint native_handle = -1;
		gpu_buffer_type type_flags;
		usage_frequency cpu_write_frequency;
		int32_t data_size;
		void* mapped_memory;
		int32_t stride;

		GLbitfield get_flags() const;
	public:
		wm_gl_buffer(const gpu_buffer_descriptor descriptor);
		std::any get_native_handle() const override;
		int32_t get_data_size() const override;
		gpu_buffer_type get_type_flags() const override;
		usage_frequency get_cpu_write_frequency() const override;
	#ifdef WM_BUILD_DEBUG
		void set_debug_label(const std::string& label) override;
	#endif
		void set_data(const void* data, const int32_t data_size, const int32_t offset = 0) override;
		int32_t get_stride() const override;
		~wm_gl_buffer();
	};

}
