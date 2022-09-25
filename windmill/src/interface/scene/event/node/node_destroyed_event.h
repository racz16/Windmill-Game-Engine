#pragma once

#include "node_event.h"

namespace wm {

	class WM_PUBLIC node_destroyed_event: public node_event {
	public:
		static key<node_destroyed_event> get_key();

		using node_event::node_event;
	};

}
