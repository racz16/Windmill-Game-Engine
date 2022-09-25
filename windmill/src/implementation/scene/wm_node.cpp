#include "wm_node.h"
#include "wm_transform.h"

#include "core/engine.h"
#include "scene/event/node/node_destroyed_event.h"
#include "defines/code_generation_defines.h"
#include "defines/log_defines.h"

namespace wm {

	void wm_node::initialize(const int32_t id) {
		node::initialize(id);
		auto transform_raw_pointer = new wm_transform();
		ptr<transform> transform_ptr{transform_raw_pointer};
		int32_t transform_id = transform_ptr.get_id();
		transform_raw_pointer->initialize(transform_id);
		initialize_node(transform_ptr);
		transform_raw_pointer->add_event_listeners();
		WM_LOG_INFO_2("node created");
	}

	bool wm_node::is_ancestor_of(const ptr<node> node) const {
		return engine::get_scene_system()->is_descendant_of(node, get_ptr());
	}

	bool wm_node::is_descendant_of(const ptr<node> node) const {
		return engine::get_scene_system()->is_descendant_of(get_ptr(), node);
	}

	ptr<node> wm_node::get_root() const {
		auto node = get_ptr();
		auto parent = get_parent();
		while(parent.is_valid()) {
			node = parent;
			parent = engine::get_scene_system()->get_parent(parent);
		}
		return node;
	}

	ptr<node> wm_node::get_parent() const {
		return engine::get_scene_system()->get_parent(get_ptr());
	}

	const std::vector<ptr<node>> wm_node::get_children() const {
		return engine::get_scene_system()->get_children(get_ptr());
	}

	void wm_node::add_child(const ptr<node> child) {
		engine::get_scene_system()->add_child(get_ptr(), child);
	}

	void wm_node::remove_child(const ptr<node> child) {
		engine::get_scene_system()->remove_child(get_ptr(), child);
	}

	void wm_node::set_parent(const ptr<node> parent) {
		if(parent.is_valid()) {
			engine::get_scene_system()->add_child(parent, get_ptr());
		} else {
			engine::get_scene_system()->remove_child(parent, get_ptr());
		}
	}

	const ptr<wm::transform> wm_node::get_transform() const {
		return engine::get_scene_system()->get_transform(get_ptr());
	}

	bool wm_node::contains_tag(const tag& tag) const {
		return engine::get_scene_system()->contains_tag(get_ptr(), tag);
	}

	const std::vector<tag> wm_node::get_tags() const {
		return engine::get_scene_system()->get_tags(get_ptr());
	}

	void wm_node::add_tag(const tag& tag) {
		engine::get_scene_system()->add_tag(get_ptr(), tag);
	}

	void wm_node::remove_tag(const tag& tag) {
		return engine::get_scene_system()->remove_tag(get_ptr(), tag);
	}

	const std::vector<ptr<component>> wm_node::get_components_impl(const key<component>& key) const {
		return engine::get_scene_system()->get_components(get_ptr(), key);
	}

	ptr<component> wm_node::get_component_impl(const key<wm::component>& key) const {
		return engine::get_scene_system()->get_component(get_ptr(), key);
	}

	void wm_node::add_component_impl(const ptr<component> component, const key<wm::component>& key) const {
		engine::get_scene_system()->add_component(get_ptr(), component, key);
	}

	void wm_node::remove_component_impl(const ptr<component> component) const {
		engine::get_scene_system()->remove_component(get_ptr(), component);
	}

	wm_node::~wm_node() {
		auto node = get_ptr();
		node_destroyed_event event(node);
		engine::get_event_system()->emit_event(node_destroyed_event::get_key(), event, node);
		WM_LOG_INFO_2("node destroyed");
	}

}
