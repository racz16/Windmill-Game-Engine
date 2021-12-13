#include "rendering_system.h"

#include "../../implementation/rendering/wm_vulkan_rendering_system.h"

namespace wm {

	ptr<rendering_system> rendering_system::create() {
		return ptr<rendering_system>(new wm_vulkan_rendering_system());
	}

	key<rendering_system> rendering_system::get_key() {
		static const key<rendering_system> key("WM_RENDERING_SYSTEM");
		return key;
	}

}
