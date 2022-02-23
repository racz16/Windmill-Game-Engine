#include "wm_node.h"

namespace wm {

	std::string wm_node::get_name() const {
		return name;
	}

	void wm_node::set_name(const std::string& name) {
		this->name = name;
	}

}
