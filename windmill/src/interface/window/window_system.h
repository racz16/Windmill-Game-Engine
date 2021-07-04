#pragma once

#include "../defines.h"
#include "window_system_init.h"

namespace wm {

	class WM_PUBLIC window_system {
	public:
		static std::shared_ptr<window_system> get_instance(const window_system_init init = {});

		virtual void update() = 0;
		virtual bool is_closing() const = 0;
		virtual void set_closing() = 0;
		virtual glm::ivec2 get_min_size() const = 0;
		virtual void set_min_size(const glm::ivec2 min_size) = 0;
		virtual glm::ivec2 get_max_size() const = 0;
		virtual void set_max_size(const glm::ivec2 max_size) = 0;
		virtual float get_aspect_ratio_width() const = 0;
		virtual float get_aspect_ratio_height() const = 0;
		virtual void set_aspect_ratio(const int32_t width, const int32_t height) = 0;
		virtual glm::ivec2 get_position() const = 0;
		virtual void set_position(const glm::ivec2 position) = 0;
		virtual std::string get_title() const = 0;
		virtual void set_title(const std::string& title) = 0;
		virtual bool is_minimized() const = 0;
		virtual void set_minimization(const bool minimization) = 0;
		virtual bool is_maximized() const = 0;
		virtual void set_maximization(const bool maximization) = 0;
		virtual bool is_visible() const = 0;
		virtual void set_visibility(const bool visibility) = 0;
		virtual bool is_focused() const = 0;
		virtual void focus() = 0;
		virtual bool is_fullscreen() const = 0;
		virtual void set_fullscreen(const bool fullscreen) = 0;
		virtual glm::ivec2 get_size() const = 0;
		virtual void set_size(const glm::ivec2 size) = 0;
		virtual glm::ivec2 get_framebuffer_size() const = 0;
		virtual void get_frame_size() const = 0;
		virtual glm::vec2 get_content_scale() const = 0;
		virtual void request_attention() = 0;
		virtual bool is_hovered() const = 0;
		virtual bool is_resizable() const = 0;
		virtual bool is_decorated() const = 0;
		virtual bool is_always_on_top() const = 0;
		virtual bool is_focus_on_show() const = 0;
		virtual glm::ivec2 get_screen_size() const = 0;
		virtual int32_t get_screen_refresh_rate() const = 0;
		virtual ~window_system() { }
	};

}
