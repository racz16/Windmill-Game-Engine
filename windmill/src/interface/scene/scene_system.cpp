#include "scene_system.h"
#include "defines/code_generation_defines.h"

#include "../../implementation/scene/wm_simple_scene_system.h"

namespace wm {

	WM_CREATE(scene_system, wm_simple_scene_system);

	WM_GET_KEY(scene_system, "WM_SCENE_SYSTEM");

}
