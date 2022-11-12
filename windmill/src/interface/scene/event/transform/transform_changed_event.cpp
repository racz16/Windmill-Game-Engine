#include "transform_changed_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(transform_changed_event, "WM_TRANSFORM_CHANGED_EVENT");

	transform_changed_event::transform_changed_event(const transform_changed_type type, const ptr_view<wm::transform> transform)
		: transform_event(transform), type(type) {}

	transform_changed_type transform_changed_event::get_type() const {
		return type;
	}

}
