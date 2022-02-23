#pragma once

#include "scene/node.h"

namespace wm {

	class wm_node: public node {
	private:
		std::string name;
	public:
		std::string get_name() const override;
		void set_name(const std::string& name) override;
	};

}
