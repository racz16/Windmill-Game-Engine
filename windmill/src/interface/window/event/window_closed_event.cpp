#include "window_closed_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(window_closed_event, "WM_WINDOW_CLOSED_EVENT");

	window_closed_event::window_closed_event(): event(true) { }

}
