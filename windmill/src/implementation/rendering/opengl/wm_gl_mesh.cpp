#include "wm_gl_mesh.h"

#include "defines/log_defines.h"

#include "../wm_gpu_vertex.h"
#include "wm_gl_defines.h"

namespace wm {

	wm_gl_mesh::wm_gl_mesh(const gpu_mesh_descriptor descriptor): vertex_buffer(descriptor.vertex_buffer), index_buffer(descriptor.index_buffer) {

		glCreateVertexArrays(1, &native_handle);

		glVertexArrayVertexBuffer(native_handle, 0, GL_ID_OF(vertex_buffer), 0, vertex_buffer->get_stride());

		// TODO: make it more general

		//position
		GLuint position_attrib_index = 0;
		glEnableVertexArrayAttrib(native_handle, position_attrib_index);
		glVertexArrayAttribFormat(native_handle, position_attrib_index, 3, GL_FLOAT, GL_FALSE, offsetof(wm_gpu_vertex, position));
		glVertexArrayAttribBinding(native_handle, position_attrib_index, 0);
		//normal
		GLuint normal_attrib_index = 1;
		glEnableVertexArrayAttrib(native_handle, normal_attrib_index);
		glVertexArrayAttribFormat(native_handle, normal_attrib_index, 3, GL_FLOAT, GL_FALSE, offsetof(wm_gpu_vertex, normal));
		glVertexArrayAttribBinding(native_handle, normal_attrib_index, 0);
		//texture
		GLuint texture_attrib_index = 2;
		glEnableVertexArrayAttrib(native_handle, texture_attrib_index);
		glVertexArrayAttribFormat(native_handle, texture_attrib_index, 2, GL_FLOAT, GL_FALSE, offsetof(wm_gpu_vertex, texture_coordinates));
		glVertexArrayAttribBinding(native_handle, texture_attrib_index, 0);

		glVertexArrayElementBuffer(native_handle, GL_ID_OF(index_buffer));

		WM_LOG_INFO_2("OpenGL vertex array created");
	}

	std::any wm_gl_mesh::get_native_handle() const {
		return native_handle;
	}

	ptr<gpu_buffer> wm_gl_mesh::get_vertex_buffer() const {
		return vertex_buffer;
	}

	ptr<gpu_buffer> wm_gl_mesh::get_index_buffer() const {
		return index_buffer;
	}

	int32_t wm_gl_mesh::get_data_size() const {
		return 0;
	}

#ifdef WM_BUILD_DEBUG

	void wm_gl_mesh::set_debug_label(const std::string& label) {
		GL_LABEL(GL_VERTEX_ARRAY, native_handle, label);
	}

#endif

	wm_gl_mesh::~wm_gl_mesh() {
		if(index_buffer.is_valid()) {
			index_buffer.destroy();
			index_buffer = nullptr;
		}
		if(vertex_buffer.is_valid()) {
			vertex_buffer.destroy();
			vertex_buffer = nullptr;
		}
		glDeleteVertexArrays(1, &native_handle);
		WM_LOG_INFO_2("OpenGL vertex array destroyed");
	}

}
