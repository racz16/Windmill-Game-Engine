#include "gpu_buffer.h"

#include "rendering_system.h"

#include "../../implementation/rendering/opengl/wm_gl_buffer.h"
#include "../../implementation/rendering/vulkan/wm_vk_buffer.h"
#ifdef WM_PLATFORM_WINDOWS
	#include "../../implementation/rendering/direct3d11/wm_dx11_buffer.h"
#endif

namespace wm {

	ptr<gpu_buffer> gpu_buffer::create(const gpu_buffer_descriptor descriptor) {
		auto selected_api = rendering_system::get_rendering_api();
		switch(selected_api) {
			case wm::rendering_api::vulkan:
				return ptr<gpu_buffer>(new wm_vk_buffer(descriptor));
			case wm::rendering_api::opengl:
				return ptr<gpu_buffer>(new wm_gl_buffer(descriptor));
			case wm::rendering_api::direct3d11:
			#ifdef WM_PLATFORM_WINDOWS
				return ptr<gpu_buffer>(new wm_dx11_buffer(descriptor));
			#else
				WM_THROW_ERROR("The Direct3D 11 rendering API is only supported on Windows");
			#endif
			default:
				WM_THROW_ERROR("Unknown rendering API");
		}
	}

	WM_ENUM_FLAG_FUNCTION_DEFINITIONS(gpu_buffer_type);

	WM_GET_KEY(gpu_buffer, "WM_GPU_BUFFER");

}
