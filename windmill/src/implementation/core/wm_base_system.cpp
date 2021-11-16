#include "wm_base_system.h"

namespace wm {

	void wm_base_system::update() { }

	bool wm_base_system::is_active() const {
		return active;
	}

	void wm_base_system::set_active(const bool active) {
		this->active = active;
	}

}
