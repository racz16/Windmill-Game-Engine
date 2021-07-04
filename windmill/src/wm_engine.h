#pragma once

#include "engine.h"
#include "window/window_system.h"

namespace wm {

	class wm_engine: public engine {
	public:
		void initialize() override;
		void run() override;
	};

}
