#pragma once

#include "transform_changed_type.h"
#include "transform_event.h"

namespace wm {

	class WM_PUBLIC transform_changed_event: public transform_event {
	private:
		transform_changed_type type;
	public:
		static key<transform_changed_event> get_key();

		transform_changed_event(const transform_changed_type type, const ptr<wm::transform> transform);
		transform_changed_type get_type() const;
	};

}
