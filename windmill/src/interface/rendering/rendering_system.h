#pragma once

#include "../core/system.h"
#include "rendering_api.h"
#include "../window/window_system.h"

namespace wm {

	class WM_PUBLIC rendering_system: public system {
	protected:
		static rendering_api api;
	public:
		static ptr<rendering_system> create();
		static key<rendering_system> get_key();
		static rendering_api get_rendering_api();
		static void set_rendering_api(const rendering_api api);
	};

}
