#pragma once

#include "../window/window_system.h"

namespace wm {

	class WM_PUBLIC rendering_context {
	protected:
		void create_surface(const void* context, void* surface);
		void swap_buffers();
		vsync_mode get_vsync_mode() const;
		void set_vsync_mode(const vsync_mode mode);
		void make_context_current();
		window_system::get_function_address_t get_function_address() const;
		std::any get_win32_handle() const;
	public:
		static ptr<rendering_context> create();
		static key<rendering_context> get_key();

		virtual void initialize() = 0;
		virtual void update() = 0;
		virtual void destroy() = 0;
	};

}
