#pragma once

#include "../wm_utility.h"

namespace wm {

	class WM_PUBLIC engine {
	public:
		virtual void run() = 0;
		static std::shared_ptr<engine> create();
	};

}