#include "wm_simple_scene_system.h"

#include "defines/log_defines.h"

namespace wm {

	wm_simple_scene_system::wm_simple_scene_system() {
		WM_LOG_INFO_1("simple scene system created");
	}

	void wm_simple_scene_system::update() {
		WM_LOG_INFO_3("simple scene system updated");
	}

	wm_simple_scene_system::~wm_simple_scene_system() {
		WM_LOG_INFO_1("simple scene system destroyed");
	}

}
