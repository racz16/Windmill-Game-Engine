#pragma once

namespace wm {

	enum class transform_changed_type {
		parent_changed = 1,
		position_changed = 2,
		rotation_changed = 3,
		scale_changed = 4
	};

}
