#pragma once

#include "../core/system.h"
#include "../core/key.h"
#include "../ptr/ptr.h"

namespace wm {

	class WM_PUBLIC audio_system: public system {
	public:
		static ptr<audio_system> create();
		static key<audio_system> get_key();
	};

}
