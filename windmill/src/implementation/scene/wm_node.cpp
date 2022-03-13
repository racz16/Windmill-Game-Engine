#include "wm_node.h"

#include "core/engine.h"

namespace wm {

	wm_node::wm_node() {}

	wm_node::wm_node(const int32_t id): id(id) {
		engine::get_scene_system()->set_transform(get_ptr(), transform::create());
	}

	wm_node& wm_node::operator=(const wm_node& other_node) {
		id = other_node.id;
		destructible = true;
		return *this;
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

	void wm_node::set_parent(const ptr<node> parent) {
		engine::get_scene_system()->set_parent(get_ptr(), parent);
	}

	int32_t wm_node::get_children_count() const {
		return engine::get_scene_system()->get_child_count(get_ptr());
	}

	ptr<node> wm_node::get_child(const int32_t index) const {
		return engine::get_scene_system()->get_child(get_ptr(), index);
	}

	void wm_node::add_child(const ptr<node> child) {
		engine::get_scene_system()->set_parent(child, get_ptr());
	}

	ptr_view<wm::transform> wm_node::get_transform() const {
		return engine::get_scene_system()->get_transform(get_ptr());
	}

	ptr<node> wm_node::get_ptr() const {
		if(id == -1) {
			return ptr<node>(nullptr);
		} else {
			return ptr<node>(id);
		}
	}

	void wm_node::add_tag(const std::string& tag) {
		engine::get_scene_system()->add_tag(get_ptr(), tag);
	}

	void wm_node::remove_tag(const std::string& tag) {
		return engine::get_scene_system()->remove_tag(get_ptr(), tag);
	}

	bool wm_node::contains_tag(const std::string& tag) const {
		return engine::get_scene_system()->contains_tag(get_ptr(), tag);
	}

	std::vector<std::string> wm_node::get_tags() const {
		return engine::get_scene_system()->get_tags(get_ptr());
	}

	wm_node::~wm_node() {
		if(destructible) {
			auto ss = engine::get_scene_system();
			if(ss.is_valid()) {
				set_parent(ptr<node>(nullptr));
				auto transform = ss->get_transform(get_ptr());
				ss->set_transform(get_ptr(), ptr<wm::transform>(nullptr));
				transform.destroy();
				while(get_children_count() > 0) {
					get_child(0).destroy();
				}
			}
		}
	}

}
