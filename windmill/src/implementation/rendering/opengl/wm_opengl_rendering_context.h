#pragma once

#include "rendering/rendering_context.h"

#include <glad/glad.h>

#include "rendering/gpu_mesh.h"

#include "../wm_gpu_vertex.h"

namespace wm {

	class wm_opengl_rendering_context: public rendering_context {
	private:
		std::vector<wm_gpu_vertex> vertices;
		std::vector<uint32_t> indices;

		GLuint shader_program;
		ptr<gpu_mesh> vao = nullptr;
		ptr<gpu_buffer> ubo = nullptr;
		GLuint texture;
		GLuint sampler;

		static std::string get_object_type(const GLenum type);
		static std::string get_message_source(const GLenum source);
		static std::string get_message_type(const GLenum type);
		static std::string get_message_severity(const GLenum severity);

		static void create_debug_message_callback();
		static void set_debug_label(const GLenum type, const GLuint id, const std::string& label);

		void initialize_opengl();
		void initialize_imgui();

		//buffer
		void load_mesh(const std::string& file_name);
		void create_vao(const std::string& file_name);
		void create_ubo();

		//texture
		void create_texture(const std::string& file_name);
		void create_sampler();

		//shader
		GLuint create_shader(const std::string& path, const GLenum type);
		void create_shader_program(const std::string& vertex_path, const std::string& fragment_path);
		std::vector<char> read_binary_file(const std::string& file_name);
		std::string read_text_file(const std::string& file_name);
		void load_uniforms();
	public:
		void initialize() override;
		void update() override;
		void destroy() override;
	};

}
