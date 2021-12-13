#include "event_system.h"

#include "../implementation/event/wm_event_system.h"

namespace wm {

	ptr<event_system> event_system::create() {
		return ptr<event_system>(new wm_event_system());
	}

	key<event_system> event_system::get_key() {
		static const key<event_system> key("WM_EVENT_SYSTEM");
		return key;
	}
}
