#pragma once

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "../interface/window/window.h"

namespace wm {

	class wm_glfw_window: public window {
	private:
		static const int32_t DONT_CARE = -1;
		static int32_t last_id;

		int32_t id;
		GLFWwindow* window_handler;
		std::string title;
		glm::vec2 aspect_ratio {DONT_CARE};
		glm::ivec2 minimum_size = glm::ivec2(DONT_CARE);
		glm::ivec2 maximum_size = glm::ivec2(DONT_CARE);
		bool fullscreen;
		int32_t refresh_rate = DONT_CARE;

		void set_window_hints(const bool visible);
		void create_window(const glm::ivec2& size);
		void set_window_monitor();
	public:
		wm_glfw_window(const glm::ivec2& size, const std::string& title, const bool fullscreen, const bool visible);
		int32_t get_id() const;
		bool is_closing() const override;
		void set_closing(const bool closing) override;
		bool is_visible() const override;
		void set_visibility(const bool visibility) override;
		glm::ivec2 get_position() const override;
		void set_position(const glm::ivec2& position) override;
		std::string get_title() const override;
		void set_title(const std::string& title) override;
		bool is_minimized() const override;
		void set_minimization(const bool minimization) override;
		bool is_maximized() const override;
		void set_maximization(const bool maximization) override;
		glm::vec2 get_aspect_ratio() const override;
		void set_aspect_ratio(const glm::vec2 aspect_ratio) override;
		glm::ivec2 get_minimum_size() const override;
		void set_minimum_size(const glm::ivec2& minimum_size) override;
		glm::ivec2 get_maximum_size() const override;
		void set_maximum_size(const glm::ivec2& maximum_size) override;
		glm::ivec2 get_size() const override;
		void set_size(const glm::ivec2& size) override;
		glm::ivec2 get_framebuffer_size() const override;
		glm::ivec4 get_frame_size() const override;
		glm::vec2 get_content_scale() const override;
		bool is_fullscreen() const override;
		void set_fullscreen(const bool fullscreen) override;
		int32_t get_refresh_rate() const override;
		void set_refresh_rate(const int32_t refresh_rate) override;
		bool is_resizable() const override;
		void set_resizable(const bool resizable) override;
		bool is_decorated() const override;
		void set_decorated(const bool decorated) override;
		bool is_always_on_top() const override;
		void set_always_on_top(const bool always_on_top) override;
		bool is_focus_on_show() const override;
		void set_focus_on_show(const bool focus_on_show) override;
		bool is_focused() const override;
		void focus() override;
		void request_attention() override;
		bool is_hovered() const override;
		~wm_glfw_window();
	};

}