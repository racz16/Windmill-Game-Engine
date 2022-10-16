#include "event_system.h"
#include "defines/code_generation_defines.h"

#include "../implementation/event/wm_event_system.h"

namespace wm {

	WM_CREATE(event_system, wm_event_system);

	WM_GET_KEY(event_system, "WM_EVENT_SYSTEM");

}
