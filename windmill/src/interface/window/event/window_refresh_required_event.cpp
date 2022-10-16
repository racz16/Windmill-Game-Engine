#include "window_refresh_required_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(window_refresh_required_event, "WM_WINDOW_REFRESH_REQUIRED_EVENT");

	window_refresh_required_event::window_refresh_required_event(): event(true) { }

}
