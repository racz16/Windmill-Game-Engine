#pragma once
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include "window/window_system.h"

namespace wm {

	class wm_glfw_window_system: public window_system {
	private:
		static const int32_t DONT_CARE = -1;

		GLFWwindow* window_handler;
		std::string title;
		bool fullscreen;
		glm::ivec2 min_size = glm::ivec2(DONT_CARE);
		glm::ivec2 max_size = glm::ivec2(DONT_CARE);
		float aspect_ratio_width = DONT_CARE;
		float aspect_ratio_height = DONT_CARE;
		glm::ivec2 screen_size;
		int32_t screen_refresh_rate;

		void set_error_callback();
		void initialize_glfw();
		void save_screen_parameters();
		void set_window_hints(const window_system_init init);
		void create_window(const window_system_init init);
	public:
		wm_glfw_window_system(const window_system_init init = {});
		wm_glfw_window_system(const wm_glfw_window_system& ws) = delete;
		void operator=(const wm_glfw_window_system& ws) = delete;
		bool is_closing() const override;
		void set_closing() override;
		void update() override;
		glm::ivec2 get_size() const override;
		void set_size(const glm::ivec2 size) override;
		glm::ivec2 get_framebuffer_size() const override;
		void get_frame_size() const override;
		glm::vec2 get_content_scale() const override;
		glm::ivec2 get_min_size() const override;
		void set_min_size(const glm::ivec2 min_size) override;
		glm::ivec2 get_max_size() const override;
		void set_max_size(const glm::ivec2 max_size) override;
		float get_aspect_ratio_width() const override;
		float get_aspect_ratio_height() const override;
		void set_aspect_ratio(const int32_t width, const int32_t height) override;
		glm::ivec2 get_position() const override;
		void set_position(const glm::ivec2 position) override;
		std::string get_title() const override;
		void set_title(const std::string& title) override;
		bool is_minimized() const override;
		void set_minimization(const bool minimization) override;
		bool is_maximized() const override;
		void set_maximization(const bool maximization) override;
		bool is_visible() const override;
		void set_visibility(const bool visibility) override;
		bool is_focused() const override;
		void focus() override;
		void request_attention() override;
		bool is_hovered() const override;
		bool is_resizable() const override;
		bool is_decorated() const override;
		bool is_always_on_top() const override;
		bool is_focus_on_show() const override;
		bool is_fullscreen() const override;
		void set_fullscreen(const bool fullscreen) override;
		glm::ivec2 get_screen_size() const override;
		int32_t get_screen_refresh_rate() const override;
		~wm_glfw_window_system() override;
	};

}
