#include "gpu_mesh.h"

#include "defines/code_generation_defines.h"
#include "rendering_system.h"

#include "../../implementation/rendering/opengl/wm_gl_mesh.h"
#include "../../implementation/rendering/vulkan/wm_vk_mesh.h"
#ifdef WM_PLATFORM_WINDOWS
	#include "../../implementation/rendering/direct3d11/wm_dx11_mesh.h"
#endif

namespace wm {

	ptr<gpu_mesh> gpu_mesh::create(const gpu_mesh_descriptor descriptor) {
		auto selected_api = rendering_system::get_rendering_api();
		switch(selected_api) {
			case wm::rendering_api::vulkan:
				return ptr<gpu_mesh>(new wm_vk_mesh(descriptor));
			case wm::rendering_api::opengl:
				return ptr<gpu_mesh>(new wm_gl_mesh(descriptor));
			case wm::rendering_api::direct3d11:
			#ifdef WM_PLATFORM_WINDOWS
				return ptr<gpu_mesh>(new wm_dx11_mesh(descriptor));
			#else
				WM_THROW_ERROR("The Direct3D 11 rendering API is only supported on Windows");
			#endif
			default:
				WM_THROW_ERROR("Unknown rendering API");
		}
	}

	WM_GET_KEY(gpu_mesh, "WM_GPU_MESH");

}
