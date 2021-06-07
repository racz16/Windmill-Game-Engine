#pragma once

#include <GLFW/glfw3.h>

#include "interface/engine.h"

class wm_engine : public wm::engine {
private:
	GLFWwindow* window;
public:
	void run() override;
};