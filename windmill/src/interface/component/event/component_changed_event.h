#pragma once

#include "component_event.h"
#include "../../scene/node.h"

namespace wm {

	class WM_PUBLIC component_changed_event: public component_event {
		private:
			ptr<wm::node> node{nullptr};
			key<wm::component> key;
			bool added;
	public:
		static wm::key<component_changed_event> get_key();

		component_changed_event(const ptr<wm::component> component, const bool added, const ptr<wm::node> node, const wm::key<wm::component> key);
		bool is_added() const;
		bool is_removed() const;
		ptr<wm::node> get_node() const;
		wm::key<wm::component> get_component_key() const;
	};

}
