#include "wm_gl_buffer.h"

#include "defines/log_defines.h"

#include "wm_gl_defines.h"

namespace wm {

	wm_gl_buffer::wm_gl_buffer(const gpu_buffer_descriptor descriptor)
		: type_flags(descriptor.type_flags), cpu_write_frequency(descriptor.cpu_write_frequency), data_size(descriptor.data_size), stride(descriptor.stride) {

		glCreateBuffers(1, &native_handle);
		glNamedBufferStorage(native_handle, data_size, descriptor.data, get_flags());
		if(cpu_write_frequency != usage_frequency::NEVER) {
			mapped_memory = glMapNamedBuffer(native_handle, GL_WRITE_ONLY);
		}
		WM_LOG_INFO_2("OpenGL buffer created");
	}

	GLbitfield wm_gl_buffer::get_flags() const {
		GLbitfield flags = GL_NONE;
		// TODO: make the CPU read available
		if(cpu_write_frequency != usage_frequency::NEVER) {
			flags |= GL_MAP_WRITE_BIT | GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT;
		}
		if(cpu_write_frequency == usage_frequency::FREQUENTLY) {
			flags |= GL_CLIENT_STORAGE_BIT;
		}
		return flags;
	}

	std::any wm_gl_buffer::get_native_handle() const {
		return native_handle;
	}

	gpu_buffer_type wm_gl_buffer::get_type_flags() const {
		return type_flags;
	}

	usage_frequency wm_gl_buffer::get_cpu_write_frequency() const {
		return cpu_write_frequency;
	}

	void wm_gl_buffer::set_data(void* data, const int32_t data_size, const int32_t offset) {
		std::memcpy(static_cast<char*>(mapped_memory) + offset, data, data_size);
	}

	int32_t wm_gl_buffer::get_data_size() const {
		return data_size;
	}

	int32_t wm_gl_buffer::get_stride() const {
		return stride;
	}

#ifdef WM_BUILD_DEBUG

	void wm_gl_buffer::set_debug_label(const std::string& label) {
		GL_LABEL(GL_BUFFER, native_handle, label);
	}

#endif

	wm_gl_buffer::~wm_gl_buffer() {
		if(cpu_write_frequency != usage_frequency::NEVER) {
			glUnmapNamedBuffer(native_handle);
		}
		glDeleteBuffers(1, &native_handle);
		WM_LOG_INFO_2("OpenGL buffer destroyed");
	}

}
