#include "system.h"

namespace wm {

	void system::update() {}

	bool system::is_active() const {
		return active;
	}

	void system::set_active(const bool active) {
		this->active = active;
	}

	system::~system() {}

}
