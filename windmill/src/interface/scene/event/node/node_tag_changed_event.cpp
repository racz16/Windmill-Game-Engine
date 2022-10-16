#include "node_tag_changed_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(node_tag_changed_event, "WM_NODE_TAG_CHANGED_EVENT");

	node_tag_changed_event::node_tag_changed_event(const ptr<wm::node> node, const bool added, const wm::tag& tag): node_event(node), added(added), tag(tag) {}

	bool node_tag_changed_event::is_tag_added() const {
		return added;
	}

	bool node_tag_changed_event::is_tag_removed() const {
		return !added;
	}

	wm::tag node_tag_changed_event::get_tag() const {
		return tag;
	}

}
