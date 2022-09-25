#pragma once

#include "transform_event.h"

namespace wm {

	class WM_PUBLIC transform_destroyed_event: public transform_event {
	public:
		static key<transform_destroyed_event> get_key();

		using transform_event::transform_event;
	};

}
