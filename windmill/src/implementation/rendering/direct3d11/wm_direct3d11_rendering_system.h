#pragma once

#include "rendering/rendering_system.h"
#include "rendering/rendering_context.h"

namespace wm {

	class wm_direct3d11_rendering_system: public rendering_system {
	private:
		ptr<wm::rendering_context> rendering_context{nullptr};
	public:
		wm_direct3d11_rendering_system();
		void update() override;
		~wm_direct3d11_rendering_system();
	};

}
