#pragma once

#include "../scene/node.h"
#include "../event/event_listener.h"
#include "../event/event.h"

namespace wm {

	template<class>
	class event_listener;

	class WM_PUBLIC component {
	protected:
		int32_t id = -1;
		bool active = true;
		ptr<event_listener<event>> component_changed_event_listener{nullptr}; // TODO

		component();
		virtual void initialize(const int32_t id);
		virtual void added_to_node(const ptr<node> node);
		virtual void removed_from_node(const ptr<node> node);
		virtual const ptr<component> get_ptr_impl() const;
	public:
		virtual ptr<node> get_node() const;
		virtual bool is_active() const;
		virtual void set_active(const bool active);
		virtual ~component();
	};

}
