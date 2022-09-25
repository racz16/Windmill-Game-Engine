#pragma once

#include "../../../event/event.h"
#include "../../../scene/node.h"

namespace wm {

	class WM_PUBLIC node_event: public event {
	private:
		ptr<node> node;
	public:
		static key<node_event> get_key();

		node_event(const ptr<wm::node> node);
		ptr<wm::node> get_node() const;
	};

}
