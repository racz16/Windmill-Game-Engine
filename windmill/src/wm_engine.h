#pragma once

#include <GLFW/glfw3.h>

#include "interface/engine.h"

namespace wm {

	class wm_engine : public engine {
	private:
		GLFWwindow* window;
	public:
		void run() override;
	};

}
