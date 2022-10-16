#include "camera_component.h"
#include "../../core/engine.h"
#include "defines/code_generation_defines.h"

#include "../../../implementation/component/camera/wm_camera_component.h"

namespace wm {

	WM_CREATE_WITH_INIT(camera_component, wm_camera_component);

	WM_GET_KEY(camera_component, "WM_CAMERA_COMPONENT");

}
