#pragma once

#include "node_event.h"

namespace wm {

	class WM_PUBLIC node_child_changed_event: public node_event {
	private:
		bool added;
		ptr<wm::node> child{nullptr};
	public:
		static key<node_child_changed_event> get_key();

		node_child_changed_event(const ptr<wm::node> node, const bool added, const ptr<wm::node> child);
		bool is_child_added() const;
		bool is_child_removed() const;
		ptr<wm::node> get_child() const;
	};

}
