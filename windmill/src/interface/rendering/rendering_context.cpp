#include "rendering_context.h"

#include "../../implementation/rendering/opengl/wm_opengl_rendering_context.h"
#include "../../implementation/rendering/vulkan/wm_vulkan_rendering_context.h"

#include "core/engine.h"
#include "defines/code_generation_defines.h"

namespace wm {

	ptr<rendering_context> rendering_context::create() {
		auto selected_api = rendering_system::get_rendering_api();
		if(selected_api == rendering_api::vulkan) {
			return ptr<rendering_context>(new wm_vulkan_rendering_context());
		} else if(selected_api == rendering_api::opengl) {
			return ptr<rendering_context>(new wm_opengl_rendering_context());
		} else {
			WM_THROW_ERROR("Unknown rendering API");
		}
	}

	WM_GET_KEY(rendering_context, "WM_RENDERING_CONTEXT");

	void rendering_context::create_surface(const void* context, void* surface) {
		engine::get_window_system()->create_surface(context, surface);
	}

	void rendering_context::swap_buffers() {
		engine::get_window_system()->swap_buffers();
	}

	vsync_mode rendering_context::get_vsync_mode() const {
		return engine::get_window_system()->get_vsync_mode();
	}

	void rendering_context::set_vsync_mode(const vsync_mode mode) {
		engine::get_window_system()->set_vsync_mode(mode);
	}

	void rendering_context::make_context_current() {
		engine::get_window_system()->make_context_current();
	}

	window_system::get_function_address_t rendering_context::get_function_address() const {
		return engine::get_window_system()->get_function_address();
	}

}
