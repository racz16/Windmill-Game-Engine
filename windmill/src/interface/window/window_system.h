#pragma once

#include "video_mode.h"
#include "../core/system.h"
#include "../core/key.h"
#include "../ptr/ptr.h"
#include "cursor_shape.h"
#include "standard_cursor_shape.h"
#include "cursor_mode.h"
#include "input/window_input_handler.h"

namespace wm {

	class WM_PUBLIC window_system: public system {
	protected:
		friend class rendering_system;
		virtual void create_surface(const void* context, void* surface) = 0;
	public:
		static ptr<window_system> create(const glm::ivec2& size, const std::string& title, const bool fullscreen = false, const bool visible = true);
		static key<window_system> get_key();

		virtual video_mode get_current_video_mode() const = 0;
		virtual std::vector<video_mode> get_all_video_modes() const = 0;
		virtual std::vector<const char*> get_required_extensions() const = 0;
		virtual bool is_closing() const = 0;
		virtual void set_closing(const bool closing) = 0;
		virtual bool is_visible() const = 0;
		virtual void set_visibility(const bool visibility) = 0;
		virtual glm::ivec2 get_position() const = 0;
		virtual void set_position(const glm::ivec2& position) = 0;
		virtual std::string get_title() const = 0;
		virtual void set_title(const std::string& title) = 0;
		virtual bool is_minimized() const = 0;
		virtual void set_minimization(const bool minimization) = 0;
		virtual bool is_maximized() const = 0;
		virtual void set_maximization(const bool maximization) = 0;
		virtual glm::ivec2 get_aspect_ratio() const = 0;
		virtual void set_aspect_ratio(const glm::ivec2& aspect_ratio) = 0;
		virtual glm::ivec2 get_minimum_size() const = 0;
		virtual void set_minimum_size(const glm::ivec2& minimum_size) = 0;
		virtual glm::ivec2 get_maximum_size() const = 0;
		virtual void set_maximum_size(const glm::ivec2& maximum_size) = 0;
		virtual glm::ivec2 get_size() const = 0;
		virtual void set_size(const glm::ivec2& size) = 0;
		virtual glm::ivec2 get_framebuffer_size() const = 0;
		virtual glm::ivec4 get_frame_size() const = 0;
		virtual glm::vec2 get_content_scale() const = 0;
		virtual bool is_fullscreen() const = 0;
		virtual void set_fullscreen(const bool fullscreen) = 0;
		virtual int32_t get_refresh_rate() const = 0;
		virtual void set_refresh_rate(const int32_t refresh_rate) = 0;
		virtual bool is_resizable() const = 0;
		virtual void set_resizable(const bool resizable) = 0;
		virtual bool is_decorated() const = 0;
		virtual void set_decorated(const bool decorated) = 0;
		virtual bool is_always_on_top() const = 0;
		virtual void set_always_on_top(const bool always_on_top) = 0;
		virtual bool is_focus_on_show() const = 0;
		virtual void set_focus_on_show(const bool focus_on_show) = 0;
		virtual bool is_focused() const = 0;
		virtual void focus() = 0;
		virtual void request_attention() = 0;
		virtual cursor_mode get_cursor_mode() const = 0;
		virtual void set_cursor_mode(const cursor_mode mode) = 0;
		virtual cursor_shape get_cursor_shape() const = 0;
		virtual void set_cursor_shape(const standard_cursor_shape cursor_shape) = 0;
		virtual void set_cursor_shape(const std::string& file_path) = 0;
		virtual void set_icon(const std::string& file_path) const = 0;
		virtual ptr_view<window_input_handler> get_input_handler() const = 0;
	};

}
