#pragma once

#include "gpu_resource.h"
#include "usage_frequency.h"
#include "../core/key.h"
#include "../ptr/ptr.h"
#include "../defines/code_generation_defines.h"

namespace wm {

	enum class WM_PUBLIC gpu_buffer_type: uint8_t {
		UNDEFINED = 0,
		VERTEX_BUFFER = 1,
		INDEX_BUFFER = 2,
		UNIFORM_BUFFER = 4,
	};

	WM_ENUM_FLAG_FUNCTION_DECLARATIONS(gpu_buffer_type);

	class WM_PUBLIC gpu_buffer_descriptor {
	public:
		gpu_buffer_type type_flags;
		usage_frequency cpu_write_frequency;
		int32_t data_size;
		void* data;
		int32_t stride;
	};

	class WM_PUBLIC gpu_buffer: public gpu_resource {
	public:
		static ptr<gpu_buffer> create(const gpu_buffer_descriptor descriptor);
		static key<gpu_buffer> get_key();

		virtual gpu_buffer_type get_type_flags() const = 0;
		virtual usage_frequency get_cpu_write_frequency() const = 0;
		virtual void set_data(void* data, const int32_t data_size, const int32_t offset = 0) = 0;
		virtual int32_t get_stride() const = 0;
	};

}
