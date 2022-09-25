#pragma once

#include "../../../event/event.h"
#include "../../transform.h"

namespace wm {

	class WM_PUBLIC transform_event: public event {
	private:
		ptr<transform> transform;
	public:
		static key<transform_event> get_key();

		transform_event(const ptr<wm::transform> transform);
		ptr<wm::transform> get_transform() const;
	};

}
