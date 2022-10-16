#include "node_parent_changed_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(node_parent_changed_event, "WM_NODE_PARENT_CHANGED_EVENT");

	node_parent_changed_event::node_parent_changed_event(const ptr<wm::node> node, const ptr<wm::node> old_parent, const ptr<wm::node> new_parent)
		: node_event(node), old_parent(old_parent), new_parent(new_parent) {}

	ptr<wm::node> node_parent_changed_event::get_old_parent() const {
		return old_parent;
	}

	ptr<wm::node> node_parent_changed_event::get_new_parent() const {
		return new_parent;
	}

}
