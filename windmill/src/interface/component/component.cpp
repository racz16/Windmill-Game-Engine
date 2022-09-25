#include "component.h"
#include "event/component_destroyed_event.h"
#include "event/component_changed_event.h"
#include "defines/log_defines.h"

namespace wm {

	component::component() {}

	void component::initialize(const int32_t id) {
		this->id = id;
		component_changed_event_listener = engine::get_event_system()->add_event_listener<component_changed_event>(component_changed_event::get_key(), [this](const component_changed_event event) {
			if(event.is_added()) {
				this->added_to_node(event.get_node());
			} else {
				this->removed_from_node(event.get_node());
			}
		}, get_ptr_impl()).convert<event_listener<event>>();
	}

	const ptr<component> component::get_ptr_impl() const {
		if(id == -1) {
			return ptr<component>(nullptr);
		} else {
			return ptr<component>(id);
		}
	}

	ptr<node> component::get_node() const {
		return engine::get_scene_system()->get_node_by_component<component>(get_ptr_impl());
	}

	void component::added_to_node(const ptr<node> node) {}

	void component::removed_from_node(const ptr<node> node) {}

	bool component::is_active() const {
		return active;
	}

	void component::set_active(const bool active) {
		this->active = active;
		WM_LOG_INFO_3("component activeness changed to " + std::to_string(active));
	}

	component::~component() {
		component_changed_event_listener.destroy();
		auto component = get_ptr_impl();
		engine::get_scene_system()->cleanup(component);
		component_destroyed_event event(component);
		engine::get_event_system()->emit_event<component_destroyed_event>(component_destroyed_event::get_key(), event, component);
	}

}
