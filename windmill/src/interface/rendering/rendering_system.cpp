#include "rendering_system.h"
#include "core/engine.h"
#include "defines/code_generation_defines.h"

#include "../../implementation/rendering/wm_vulkan_rendering_system.h"

namespace wm {

	WM_CREATE(rendering_system, wm_vulkan_rendering_system);

	WM_GET_KEY(rendering_system, "WM_RENDERING_SYSTEM");

	void rendering_system::create_surface(const void* context, void* surface) const {
		engine::get_window_system()->create_surface(context, surface);
	}

}
