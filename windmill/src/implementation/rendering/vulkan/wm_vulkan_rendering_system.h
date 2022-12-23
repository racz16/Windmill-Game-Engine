#pragma once

#include "rendering/rendering_system.h"
#include "rendering/rendering_context.h"

namespace wm {

	class wm_vulkan_rendering_system: public rendering_system {
	private:
		ptr<rendering_context> rendering_context{nullptr};
	public:
		wm_vulkan_rendering_system();
		void update() override;
		~wm_vulkan_rendering_system();
	};

}
