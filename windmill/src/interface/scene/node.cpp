#include "node.h"
#include "../core/engine.h"

#include "../../implementation/scene/wm_node.h"

namespace wm {

	bool node::type_added = false;

	ptr<node> node::create() {
		auto array_allocator_system = engine::get_array_allocator_system();
		if(!type_added) {
			array_allocator_system->add_type<node, wm_node>(get_key());
			type_added = true;
		}
		return array_allocator_system->create<node>(get_key());
	}

	key<node> node::get_key() {
		static const key<node> key("WM_NODE");
		return key;
	}

}
