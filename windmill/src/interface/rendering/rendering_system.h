#pragma once

#include "../core/key.h"
#include "../ptr/ptr.h"
#include "../core/system.h"
#include "rendering_api.h"
#include "../window/window_system.h"

namespace wm {

	class WM_PUBLIC rendering_system: public system {
	protected:
		static rendering_api api;

		void create_surface(const void* context, void* surface);
		void swap_buffers();
		vsync_mode get_vsync_mode() const;
		void set_vsync_mode(const vsync_mode mode);
		void make_context_current();
		window_system::get_function_address_t get_function_address() const;
	public:
		static ptr<rendering_system> create();
		static key<rendering_system> get_key();
		static rendering_api get_rendering_api();
		static void set_rendering_api(const rendering_api api);
	};

}
