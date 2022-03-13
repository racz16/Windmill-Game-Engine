#include "transform.h"
#include "../core/engine.h"

#include "../../implementation/scene/wm_transform.h"

namespace wm {

	bool transform::type_added = false;

	ptr<transform> transform::create() {
		auto array_allocator_system = engine::get_array_allocator_system();
		if(!type_added) {
			array_allocator_system->add_type<transform, wm_transform>(get_key());
			type_added = true;
		}
		return array_allocator_system->create<transform>(get_key());
	}

	key<transform> transform::get_key() {
		static const key<transform> key("WM_TRANSFORM");
		return key;
	}

}
