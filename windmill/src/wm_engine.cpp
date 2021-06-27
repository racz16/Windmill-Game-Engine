#include "wm_engine.h"

namespace wm {

	void wm_engine::initialize() {
		glfwInit();
		WM_LOG_INFO_1("GLFW initialized");
		window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
		WM_LOG_INFO_1("window created");
	}

	void wm_engine::run() {
		try {
			while(!glfwWindowShouldClose(window)) {
				glfwPollEvents();
			}
		} catch(const std::exception& ex) {
			WM_LOG_ERROR(ex.what());
			WM_BREAKPOINT();
		} catch(...) {
			WM_LOG_ERROR("Unknown exception");
			WM_BREAKPOINT();
		}
	}

	void wm_engine::destroy() {
		glfwDestroyWindow(window);
		WM_LOG_INFO_1("window destroyed");
		glfwTerminate();
		WM_LOG_INFO_1("GLFW terminated");
	}

}
