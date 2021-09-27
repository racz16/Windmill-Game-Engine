#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "window.h"
#include "video_mode.h"
#include "../core/defines.h"
#include "../core/system.h"
#include "../core/key.h"
#include "../core/ptr.h"

namespace wm {

	class WM_PUBLIC window_system: public system {
	public:
		static ptr<window_system> get_instance();
		static key<window_system> get_key();

		virtual ptr<window> create_window(const glm::ivec2& size, const std::string& title, const bool fullscreen, const bool visible = true) = 0;
		virtual int32_t get_window_count() const = 0;
		virtual ptr<window> get_window(const int32_t index) const = 0;
		virtual void destroy_window(ptr<window> window) = 0;
		virtual video_mode get_current_video_mode() const = 0;
		virtual std::vector<video_mode> get_all_video_modes() const = 0;
		virtual ~window_system() = 0 {}
	};

}
