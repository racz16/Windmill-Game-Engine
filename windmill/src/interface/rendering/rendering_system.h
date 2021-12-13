#pragma once

#include "../core/key.h"
#include "../ptr/ptr.h"
#include "../core/system.h"

namespace wm {

	class WM_PUBLIC rendering_system: public system {
	public:
		static ptr<rendering_system> create();
		static key<rendering_system> get_key();
	};

}
