#include "transform_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(transform_event, "WM_TRANSFORM_EVENT");

	transform_event::transform_event(const ptr<wm::transform> transform): event(true), transform(transform) {}

	ptr<wm::transform> transform_event::get_transform() const {
		return transform;
	}

}
