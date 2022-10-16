#pragma once

#include "node_event.h"

namespace wm {

	class WM_PUBLIC node_tag_changed_event: public node_event {
	private:
		bool added;
		wm::tag tag{""};
	public:
		static key<node_tag_changed_event> get_key();

		node_tag_changed_event(const ptr<wm::node> node, const bool added, const wm::tag& tag);
		bool is_tag_added() const;
		bool is_tag_removed() const;
		wm::tag get_tag() const;
	};

}
