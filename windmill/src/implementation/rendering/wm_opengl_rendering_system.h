#pragma once

#include <glad/glad.h>

#include "rendering/rendering_system.h"

namespace wm {

	struct gpu_vertex_2 {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texture_coordinates;
	};

	struct uniform_buffer_object_2 {
		glm::mat4 model;
		glm::mat3 inverse_model;
		glm::mat4 view;
		glm::mat4 projection;
	};

	class wm_opengl_rendering_system: public rendering_system {
	private:
		std::vector<gpu_vertex_2> vertices;
		std::vector<uint32_t> indices;

		GLuint shader_program;
		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		GLuint ubo;
		GLuint texture;
		GLuint sampler;

		static std::string get_message_source(const GLenum source);
		static std::string get_message_type(const GLenum type);
		static std::string get_message_severity(const GLenum severity);
		
		void create_debug_message_callback() const;

		void initialize_opengl();
		void initialize_imgui() const;

		//buffer
		void load_mesh(const std::string& file_name);
		void create_vao(const std::string& file_name);
		void create_ubo();

		//texture
		void create_texture(const std::string& file_name);
		void create_sampler();

		//shader
		GLuint create_shader(const std::string& path, const GLenum type) const;
		void create_shader_program(const std::string& vertex_path, const std::string& fragment_path);
		std::vector<char> read_binary_file(const std::string& file_name) const;
		std::string read_text_file(const std::string& file_name) const;
		void load_uniforms() const;
	public:
		wm_opengl_rendering_system();
		void update() override;
		~wm_opengl_rendering_system();
	};


}
