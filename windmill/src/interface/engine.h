#pragma once

#include "../wm_defines.h"

namespace wm {

	class WM_PUBLIC engine {
	public:
		virtual void run() = 0;
		static std::shared_ptr<engine> create();
	};

}
