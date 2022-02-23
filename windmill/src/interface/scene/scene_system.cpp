#include "scene_system.h"

#include "../../implementation/scene/wm_simple_scene_system.h"

namespace wm {

	ptr<scene_system> scene_system::create() {
		return ptr<scene_system>(new wm_simple_scene_system());
	}

	key<scene_system> scene_system::get_key() {
		static const key<scene_system> key("WM_SIMPLE_SCENE_SYSTEM");
		return key;
	}

}
