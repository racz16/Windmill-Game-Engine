#pragma once

#include "window/window_system.h"

#include "wm_glfw_window.h"
#include "../core/wm_base_system.h"

namespace wm {

	class wm_glfw_window_system: public wm_base_system, public window_system {
	private:
		std::vector<ptr<wm_glfw_window>> windows;

		wm_glfw_window_system();
		void set_error_callback() const;
		void initialize_glfw() const;
	public:
		static ptr<wm_glfw_window_system> get_instance();
		
		ptr<window> create_window(const glm::ivec2& size, const std::string& title, const bool fullscreen, const bool visible = true) override;
		int32_t get_window_count() const override;
		ptr<window> get_window(const int32_t index) const override;
		void destroy_window(ptr<window> window) override;
		void remove_window(const GLFWwindow* id);
		void update() override;
		video_mode get_current_video_mode() const override;
		std::vector<video_mode> get_all_video_modes() const override;
		~wm_glfw_window_system() override;

		bool is_active() const override { return wm_base_system::is_active(); }
		void set_active(const bool active) override { wm_base_system::set_active(active); }
	};

}
