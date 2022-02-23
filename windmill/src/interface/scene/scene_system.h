#pragma once

#include "../core/system.h"
#include "../ptr/ptr.h"
#include "../core/key.h"

namespace wm {

	class WM_PUBLIC scene_system: public system {
	public:
		static ptr<scene_system> create();
		static key<scene_system> get_key();

		
	};

}
