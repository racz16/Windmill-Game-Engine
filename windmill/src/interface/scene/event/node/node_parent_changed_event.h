#pragma once

#include "node_event.h"

namespace wm {

	class WM_PUBLIC node_parent_changed_event: public node_event {
	private:
		ptr<wm::node> old_parent{nullptr};
		ptr<wm::node> new_parent{nullptr};
	public:
		static key<node_parent_changed_event> get_key();

		node_parent_changed_event(const ptr<wm::node> node, const ptr<wm::node> old_parent, const ptr<wm::node> new_parent);
		ptr<wm::node> get_old_parent() const;
		ptr<wm::node> get_new_parent() const;
	};

}
