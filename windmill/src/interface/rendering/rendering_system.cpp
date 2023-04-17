#include "rendering_system.h"
#include "core/engine.h"
#include "defines/code_generation_defines.h"

#include "../../implementation/rendering/vulkan/wm_vulkan_rendering_system.h"
#include "../../implementation/rendering/opengl/wm_opengl_rendering_system.h"
#include "../../implementation/rendering/direct3d11/wm_direct3d11_rendering_system.h"

namespace wm {

	rendering_api rendering_system::api = rendering_api::vulkan;

	ptr<rendering_system> rendering_system::create() {
		auto selected_api = get_rendering_api();
		switch(selected_api) {
			case wm::rendering_api::vulkan:
				return ptr<rendering_system>(new wm_vulkan_rendering_system());
			case wm::rendering_api::opengl:
				return ptr<rendering_system>(new wm_opengl_rendering_system());
			case wm::rendering_api::direct3d11:
			#ifdef WM_PLATFORM_WINDOWS
				return ptr<rendering_system>(new wm_direct3d11_rendering_system());
			#else
				WM_THROW_ERROR("The Direct3D 11 rendering API is only supported on Windows");
			#endif
			default:
				WM_THROW_ERROR("Unknown rendering API");
		}
	}

	WM_GET_KEY(rendering_system, "WM_RENDERING_SYSTEM");

	rendering_api rendering_system::get_rendering_api() {
		return rendering_system::api;
	}

	void rendering_system::set_rendering_api(const rendering_api api) {
		rendering_system::api = api;
	}

}
