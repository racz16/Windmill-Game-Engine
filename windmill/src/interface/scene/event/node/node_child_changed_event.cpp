#include "node_child_changed_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(node_child_changed_event, "WM_NODE_CHILD_CHANGED_EVENT");

	node_child_changed_event::node_child_changed_event(const ptr<wm::node> node, const bool added, const ptr<wm::node> child)
		: node_event(node), added(added), child(child) {}

	bool node_child_changed_event::is_child_added() const {
		return added;
	}

	bool node_child_changed_event::is_child_removed() const {
		return !added;
	}

	ptr<wm::node> node_child_changed_event::get_child() const {
		return child;
	}


}
