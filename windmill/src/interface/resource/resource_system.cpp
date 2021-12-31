#include "resource_system.h"

#include "../../implementation/resource/wm_resource_system.h"

namespace wm {

	ptr<resource_system> resource_system::create() {
		return ptr<resource_system>(new wm_resource_system());
	}

	key<resource_system> resource_system::get_key() {
		static const key<resource_system> key("WM_RESOURCE_SYSTEM");
		return key;
	}

}
