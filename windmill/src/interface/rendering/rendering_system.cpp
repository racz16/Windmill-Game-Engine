#include "rendering_system.h"
#include "core/engine.h"
#include "defines/code_generation_defines.h"

#include "../../implementation/rendering/wm_vulkan_rendering_system.h"
#include "../../implementation/rendering/wm_opengl_rendering_system.h"

namespace wm {

	rendering_api rendering_system::api = rendering_api::vulkan;

	ptr<rendering_system> rendering_system::create() {
		auto selected_api = get_rendering_api();
		if(selected_api == rendering_api::vulkan) {
			return ptr<rendering_system>(new wm_vulkan_rendering_system());
		} else if(selected_api == rendering_api::opengl) {
			return ptr<rendering_system>(new wm_opengl_rendering_system());
		} else {
			WM_THROW_ERROR("Unknown rendering API");
		}
	}

	WM_GET_KEY(rendering_system, "WM_RENDERING_SYSTEM");

	void rendering_system::create_surface(const void* context, void* surface) {
		engine::get_window_system()->create_surface(context, surface);
	}

	void rendering_system::swap_buffers() {
		engine::get_window_system()->swap_buffers();
	}

	vsync_mode rendering_system::get_vsync_mode() const {
		return engine::get_window_system()->get_vsync_mode();
	}

	void rendering_system::set_vsync_mode(const vsync_mode mode) {
		engine::get_window_system()->set_vsync_mode(mode);
	}

	void rendering_system::make_context_current() {
		engine::get_window_system()->make_context_current();
	}

	rendering_api rendering_system::get_rendering_api() {
		return rendering_system::api;
	}

	void rendering_system::set_rendering_api(const rendering_api api) {
		rendering_system::api = api;
	}

	window_system::get_function_address_t rendering_system::get_function_address() const {
		return engine::get_window_system()->get_function_address();
	}

}
