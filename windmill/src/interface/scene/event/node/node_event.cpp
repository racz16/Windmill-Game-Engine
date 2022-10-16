#include "node_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(node_event, "WM_NODE_EVENT");

	node_event::node_event(const ptr<wm::node> node): event(true), node(node) {}

	ptr<wm::node> node_event::get_node() const {
		return node;
	}

}
