#include "wm_engine.h"

void wm_engine::run() {
	glfwInit();
	std::cout << "GLFW initialized" << std::endl;
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	std::cout << "window created" << std::endl;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	std::cout << "window destroyed" << std::endl;
	glfwTerminate();
	std::cout << "GLFW terminated" << std::endl;
}
