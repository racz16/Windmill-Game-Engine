#pragma once

#include "component_event.h"

namespace wm {

	class WM_PUBLIC component_destroyed_event: public component_event {
	public:
		static key<component_destroyed_event> get_key();

		using component_event::component_event;
	};

}
