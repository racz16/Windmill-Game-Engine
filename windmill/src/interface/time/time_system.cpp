#include "time_system.h"
#include "defines/code_generation_defines.h"

#include "../../implementation/time/wm_chrono_time_system.h"

namespace wm {

	WM_CREATE(time_system, wm_chrono_time_system);

	WM_GET_KEY(time_system, "WM_TIME_SYSTEM");

}
