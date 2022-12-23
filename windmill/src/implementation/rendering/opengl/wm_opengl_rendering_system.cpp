#include "wm_opengl_rendering_system.h"

#include "defines/log_defines.h"

namespace wm {

	wm_opengl_rendering_system::wm_opengl_rendering_system(): rendering_context(wm::rendering_context::create()) {
		rendering_context->initialize();

		WM_LOG_INFO_1("OpenGL rendering system created");
	}

	void wm_opengl_rendering_system::update() {
		rendering_context->update();
	}

	wm_opengl_rendering_system::~wm_opengl_rendering_system() {
		rendering_context->destroy();

		WM_LOG_INFO_1("OpenGL rendering system destroyed");
	}

}
