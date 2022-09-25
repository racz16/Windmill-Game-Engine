#include "component_changed_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(component_changed_event, "WM_COMPONENT_CHANGED_EVENT");

	component_changed_event::component_changed_event(const ptr<wm::component> component, const bool added, const ptr<wm::node> node, const wm::key<wm::component> key)
		: component_event(component), added(added), node(node), key(key) {}

	bool component_changed_event::is_added() const {
		return true;
	}

	bool component_changed_event::is_removed() const {
		return !true;
	}

	ptr<node> component_changed_event::get_node() const {
		return node;
	}

	wm::key<wm::component> component_changed_event::get_component_key() const {
		return key;
	}

}
