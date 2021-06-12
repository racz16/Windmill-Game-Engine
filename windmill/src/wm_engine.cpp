#include <glm/vec3.hpp>

#include "wm_engine.h"

namespace wm {

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
		glm::vec3 vector = glm::vec3(1.0f, 0.5f, 0.0f);
		std::cout << "vector x: " << vector.x << ", y: " << vector.y << ", z: " << vector.z << std::endl;
	}

}
