#include "wm_opengl_rendering_system.h"

#include <sstream>

#include "defines/log_defines.h"
#include "component/camera/camera_component.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace wm {

	wm_opengl_rendering_system::wm_opengl_rendering_system() {
		initialize_opengl();
	#ifdef WM_BUILD_DEBUG
		create_debug_message_callback();
	#endif
		initialize_imgui();
		create_vao("res/mesh/helmet.obj");
		create_texture("res/mesh/helmet.jpg");
		create_sampler();
		create_ubo();
		create_shader_program("res/shader/shader.vert.spv", "res/shader/shader.frag.spv");

		WM_LOG_INFO_1("OpenGL rendering system created");
	}

	void wm_opengl_rendering_system::initialize_opengl() {
		make_context_current();
		set_vsync_mode(vsync_mode::off);
		int result = gladLoadGLLoader(rendering_system::get_function_address());
		WM_ASSERT(result);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
		glEnable(GL_MULTISAMPLE);
	}

	void wm_opengl_rendering_system::initialize_imgui() const {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(std::any_cast<GLFWwindow*>(engine::get_window_system()->get_native_id()), true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	std::string wm_opengl_rendering_system::get_message_source(const GLenum source) {
		switch(source) {
			case GL_DEBUG_SOURCE_API: return "API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
			case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
			case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
			case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
			case GL_DEBUG_SOURCE_OTHER: return "OTHER";
			default: return "UNKNOWN";
		}
	}

	std::string wm_opengl_rendering_system::get_message_type(const GLenum type) {
		switch(type) {
			case GL_DEBUG_TYPE_ERROR: return "ERROR";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
			case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
			case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
			case GL_DEBUG_TYPE_MARKER: return "MARKER";
			case GL_DEBUG_TYPE_OTHER: return "OTHER";
			default: return "UNKNOWN";
		}
	}

	std::string wm_opengl_rendering_system::get_message_severity(const GLenum severity) {
		switch(severity) {
			case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
			case GL_DEBUG_SEVERITY_LOW: return "LOW";
			case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
			case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
			default: return "UNKNOWN";
		}
	}

	void wm_opengl_rendering_system::create_debug_message_callback() const {
		int flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if(flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param) {
				auto src_str = get_message_source(source);
				auto type_str = get_message_type(type);
				auto severity_str = get_message_severity(severity);
				if(severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
					WM_LOG_INFO_3("OpenGL info 3: " + severity_str + ", " + type_str + ", " + src_str + ", " + std::to_string(id) + ", " + message);
				} else if(severity == GL_DEBUG_SEVERITY_LOW) {
					WM_LOG_INFO_1("OpenGL info 1: " + severity_str + ", " + type_str + ", " + src_str + ", " + std::to_string(id) + ", " + message);
				} else if(severity == GL_DEBUG_SEVERITY_MEDIUM) {
					WM_LOG_WARNING("OpenGL warning: " + severity_str + ", " + type_str + ", " + src_str + ", " + std::to_string(id) + ", " + message);
				} else {
					WM_LOG_ERROR("OpenGL error: " + severity_str + ", " + type_str + ", " + src_str + ", " + std::to_string(id) + ", " + message, __FUNCTION__, __LINE__);
				}
			}, nullptr);
		}

		WM_LOG_INFO_2("OpenGL debug message callback created");
	}

	void wm_opengl_rendering_system::load_mesh(const std::string& file_name) {
		auto mesh = engine::get_resource_system()->get_mesh(file_name);
		for(auto& v : mesh->get_vertices()) {
			vertices.push_back({v.get_position(), v.get_normal(), v.get_texture_coordinate()});
		}
		indices = mesh->get_indices();
	}

	std::vector<char> wm_opengl_rendering_system::read_binary_file(const std::string& file_name) const {
		std::ifstream file(file_name, std::ios::ate | std::ios::binary);
		WM_ASSERT(file.is_open());

		int32_t file_size = static_cast<int32_t>(file.tellg());
		std::vector<char> buffer(file_size);
		file.seekg(0);
		file.read(buffer.data(), file_size);
		file.close();

		return buffer;
	}

	std::string wm_opengl_rendering_system::read_text_file(const std::string& file_name) const {
		std::stringstream stringstream;
		try {
			std::fstream filestream;
			filestream.open(file_name);
			stringstream << filestream.rdbuf();
			filestream.close();
		} catch(std::fstream::failure& ex) {
			WM_THROW_ERROR("code: " + std::to_string(ex.code().value()) + ", what: " + ex.what());
		}
		return stringstream.str();
	}

	GLuint wm_opengl_rendering_system::create_shader(const std::string& path, const GLenum type) const {
		GLuint shader = glCreateShader(type);
		if(path.substr(path.size() - 4) == ".spv") {
			auto buffer = read_binary_file(path);
			glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, &buffer[0], buffer.size());
			glSpecializeShader(shader, "main", 0, 0, 0);
		} else {
			auto code = read_text_file(path);
			auto c_string_code = code.c_str();
			glShaderSource(shader, 1, &c_string_code, nullptr);
			glCompileShader(shader);
		}
		GLint result;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		if(!result) {
			GLint length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			auto log = new char[length];
			glGetShaderInfoLog(shader, length, nullptr, log);
			WM_LOG_ERROR(log, __FUNCTION__, __LINE__);
			delete[] log;
		}

		WM_LOG_INFO_2("OpenGL shader created");
		return shader;
	}

	void wm_opengl_rendering_system::create_shader_program(const std::string& vertex_path, const std::string& fragment_path) {
		auto vertex_shader = create_shader(vertex_path, GL_VERTEX_SHADER);
		auto fragment_shader = create_shader(fragment_path, GL_FRAGMENT_SHADER);

		shader_program = glCreateProgram();
		glAttachShader(shader_program, vertex_shader);
		glAttachShader(shader_program, fragment_shader);
		glLinkProgram(shader_program);
		GLint result;
		glGetProgramiv(shader_program, GL_LINK_STATUS, &result);
		if(!result) {
			GLint length;
			glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &length);
			auto log = new char[length];
			glGetProgramInfoLog(shader_program, length, nullptr, log);
			WM_LOG_ERROR(log, __FUNCTION__, __LINE__);
			delete[] log;
		}
		glDeleteShader(vertex_shader);
		WM_LOG_INFO_2("OpenGL shader destroyed");
		glDeleteShader(fragment_shader);
		WM_LOG_INFO_2("OpenGL shader destroyed");

		WM_LOG_INFO_2("OpenGL shader program created");
	}

	void wm_opengl_rendering_system::create_vao(const std::string& file_name) {
		load_mesh(file_name);

		glCreateBuffers(1, &vbo);
		glNamedBufferStorage(vbo, sizeof(gpu_vertex_2) * vertices.size(), &vertices[0], GL_NONE);
		WM_LOG_INFO_2("OpenGL vertex buffer created");

		glCreateVertexArrays(1, &vao);
		glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(gpu_vertex_2));
		//position
		GLuint position_attrib_index = 0;
		glEnableVertexArrayAttrib(vao, position_attrib_index);
		glVertexArrayAttribFormat(vao, position_attrib_index, 3, GL_FLOAT, GL_FALSE, offsetof(gpu_vertex_2, position));
		glVertexArrayAttribBinding(vao, position_attrib_index, 0);
		//normal
		GLuint normal_attrib_index = 1;
		glEnableVertexArrayAttrib(vao, normal_attrib_index);
		glVertexArrayAttribFormat(vao, normal_attrib_index, 3, GL_FLOAT, GL_FALSE, offsetof(gpu_vertex_2, normal));
		glVertexArrayAttribBinding(vao, normal_attrib_index, 0);
		//texture
		GLuint texture_attrib_index = 2;
		glEnableVertexArrayAttrib(vao, texture_attrib_index);
		glVertexArrayAttribFormat(vao, texture_attrib_index, 2, GL_FLOAT, GL_FALSE, offsetof(gpu_vertex_2, texture_coordinates));
		glVertexArrayAttribBinding(vao, texture_attrib_index, 0);

		glCreateBuffers(1, &ebo);
		glNamedBufferStorage(ebo, sizeof(GLuint) * indices.size(), &indices[0], GL_NONE);
		glVertexArrayElementBuffer(vao, ebo);

		WM_LOG_INFO_2("OpenGL element buffer created");
		WM_LOG_INFO_2("OpenGL vertex array created");
	}

	void wm_opengl_rendering_system::create_ubo() {
		glCreateBuffers(1, &ubo);
		glNamedBufferStorage(ubo, sizeof(uniform_buffer_object_2), nullptr, GL_DYNAMIC_STORAGE_BIT);

		WM_LOG_INFO_2("OpenGL uniform buffer created");
	}

	void wm_opengl_rendering_system::create_texture(const std::string& file_name) {
		auto image = engine::get_resource_system()->get_image(file_name);

		glCreateTextures(GL_TEXTURE_2D, 1, &texture);
		glTextureStorage2D(texture, 1, GL_RGBA8, image->get_size().x, image->get_size().y);
		glTextureSubImage2D(texture, 0, 0, 0, image->get_size().x, image->get_size().y, GL_RGBA, GL_UNSIGNED_BYTE, image->get_pixels());
		glGenerateTextureMipmap(texture);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		WM_LOG_INFO_2("OpenGL texture created");

		glBindTextureUnit(1, texture);
	}

	void wm_opengl_rendering_system::create_sampler() {
		glCreateSamplers(1, &sampler);

		WM_LOG_INFO_2("OpenGL sampler created");

		glBindSampler(1, sampler);
	}

	void wm_opengl_rendering_system::load_uniforms() const {
		static const float ROTATION_SPEED = 0.05f;
		static float rotation = 0.0f;
		const float delta_time = engine::get_time_system()->get_delta_time();
		rotation += delta_time * ROTATION_SPEED;

		auto parent = engine::get_scene_system()->get_node_by_tag("parent");
		if(parent.is_valid()) {
			parent->get_transform()->set_relative_rotation(glm::vec3(0.0f, 1.0f, 0.0f), rotation);
		}
		glm::mat4 model_matrix = glm::mat4(1.0f);
		glm::mat3 inverse_model_matrix = glm::mat3(1.0f);
		auto child = engine::get_scene_system()->get_node_by_tag("child");
		if(child.is_valid()) {
			child->get_transform()->set_absolute_rotation(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
			model_matrix = child->get_transform()->get_model_matrix();
			inverse_model_matrix = child->get_transform()->get_inverse_model_matrix();
		}
		uniform_buffer_object_2 ubo{};
		ubo.model = model_matrix;
		ubo.inverse_model = inverse_model_matrix;

		auto camera = engine::get_scene_system()->get_node_by_tag("camera")->get_component(camera_component::get_key());
		ubo.view = camera->get_view_matrix();
		ubo.projection = camera->get_projection_matrix();

		glNamedBufferSubData(this->ubo, 0, sizeof(uniform_buffer_object_2), &ubo);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->ubo);
	}

	void wm_opengl_rendering_system::update() {
		auto viewport = engine::get_window_system()->get_framebuffer_size();
		glViewport(0, 0, viewport.x, viewport.y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_program);
		load_uniforms();
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glm::ivec2 size = engine::get_window_system()->get_framebuffer_size();
		//ImGui::ShowDemoWindow();
		ImVec2 position{20.0f, 20.0f};
		ImGui::SetNextWindowPos(position);
		ImGui::Begin("Statistics", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_::ImGuiWindowFlags_NoNav);
		ImGui::Text((std::to_string(engine::get_time_system()->get_fps()) + " FPS").c_str());
		ImGui::Text((std::to_string(engine::get_time_system()->get_frame_time()) + " ms").c_str());
		ImGui::Text((std::string("Resolution: ") + std::to_string(size.x) + " x " + std::to_string(size.y) + " px").c_str());
		ImGui::Text("OpenGL");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		swap_buffers();
	}

	wm_opengl_rendering_system::~wm_opengl_rendering_system() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glDeleteProgram(shader_program);
		WM_LOG_INFO_2("OpenGL shader program destroyed");
		glDeleteVertexArrays(1, &vbo);
		WM_LOG_INFO_2("OpenGL vertex array destroyed");
		glDeleteBuffers(1, &ebo);
		WM_LOG_INFO_2("OpenGL element buffer destroyed");
		glDeleteBuffers(1, &vbo);
		WM_LOG_INFO_2("OpenGL vertex buffer destroyed");
		glDeleteBuffers(1, &ubo);
		WM_LOG_INFO_2("OpenGL uniform buffer destroyed");
		glDeleteSamplers(1, &sampler);
		WM_LOG_INFO_2("OpenGL sampler destroyed");
		glDeleteTextures(1, &texture);
		WM_LOG_INFO_2("OpenGL texture destroyed");

		WM_LOG_INFO_1("OpenGL rendering system destroyed");
	}

}
