#pragma once

#include "engine.h"

#include <GLFW/glfw3.h>

namespace wm {

	class wm_engine: public engine {
	private:
		GLFWwindow* window;
	public:
		void initialize() override;
		void run() override;
		void destroy() override;
	};

}
