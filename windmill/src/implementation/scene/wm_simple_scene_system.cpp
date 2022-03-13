#include "wm_simple_scene_system.h"

#include "defines/log_defines.h"
#include "core/utility.h"

namespace wm {

	wm_simple_scene_system::wm_simple_scene_system() {
		WM_LOG_INFO_1("simple scene system created");
	}

	void wm_simple_scene_system::update() {
		WM_LOG_INFO_3("simple scene system updated");
	}

	ptr<node> wm_simple_scene_system::get_parent(const ptr<node> node) const {
		auto parent = child_parent.find(node.get_id());
		return parent == child_parent.end() ? ptr<wm::node>(nullptr) : parent->second;
	}

	void wm_simple_scene_system::set_parent(const ptr<node> child, const ptr<node> parent) {
		WM_ASSERT(child.is_valid());
		if(!child.is_valid() || child.get_id() == parent.get_id() || is_ancestor_of(parent, child)) {
			WM_THROW_ERROR("invalid parent-child relationship");
		}
		child->get_transform()->invalidate();
		auto old_parent = get_parent(child);
		if(parent.is_valid()) {
			if(parent.get_id() != old_parent.get_id()) {
				set_parent_to_null(child, old_parent);
				child_parent.insert_or_assign(child.get_id(), parent);
				auto children = parent_children.find(parent.get_id());
				if(children == parent_children.end()) {
					parent_children.insert_or_assign(parent.get_id(), std::vector<ptr<node>>{ child });
				} else {
					children->second.push_back(child);
				}
			}
		} else {
			set_parent_to_null(child, old_parent);
		}
	}

	void wm_simple_scene_system::set_parent_to_null(const ptr<node> child, const ptr<node> old_parent) {
		if(old_parent.is_valid()) {
			child_parent.erase(child.get_id());
			auto children = parent_children.at(old_parent.get_id());
			for(int32_t i = 0; i < children.size(); i++) {
				if(children.at(i).get_id() == child.get_id()) {
					children.erase(children.begin() + i - 1);
					break;
				}
			}
			if(children.size() == 0) {
				parent_children.erase(old_parent.get_id());
			}
		}
	}

	bool wm_simple_scene_system::is_ancestor_of(const ptr<node> node, const ptr<wm::node> ancestor) const {
		if(!node.is_valid() || !ancestor.is_valid()) {
			return false;
		}
		ptr<wm::node> parent = get_parent(node);
		while(parent.is_valid()) {
			if(parent.get_id() == ancestor.get_id()) {
				return true;
			}
			parent = get_parent(parent);
		}
		return false;
	}

	ptr<node> wm_simple_scene_system::get_child(const ptr<node> node, const int32_t index) const {
		auto children = parent_children.find(node.get_id());
		return children == parent_children.end() ? ptr<wm::node>(nullptr) : children->second.at(index);
	}

	int32_t wm_simple_scene_system::get_child_count(const ptr<node> node) const {
		auto children = parent_children.find(node.get_id());
		return children == parent_children.end() ? 0 : children->second.size();
	}

	ptr<node> wm_simple_scene_system::get_node(const ptr<transform> transform) const {
		auto node = transform_node.find(transform.get_id());
		return node == transform_node.end() ? ptr<wm::node>(nullptr) : node->second;
	}

	ptr<transform> wm_simple_scene_system::get_transform(const ptr<node> node) const {
		auto transform = node_transform.find(node.get_id());
		return transform == node_transform.end() ? ptr<wm::transform>(nullptr) : transform->second;
	}

	void wm_simple_scene_system::set_transform(const ptr<node> node, const ptr<transform> transform) {
		WM_ASSERT(node.is_valid() || transform.is_valid());
		if(node.is_valid() && transform.is_valid()) {
			transform_node.insert_or_assign(transform.get_id(), node);
			node_transform.insert_or_assign(node.get_id(), transform);
		} else if(node.is_valid()) {
			auto transform = node_transform.at(node.get_id());
			node_transform.erase(node.get_id());
			transform_node.erase(transform.get_id());
		} else {
			auto node = transform_node.at(transform.get_id());
			node_transform.erase(node.get_id());
			transform_node.erase(transform.get_id());
		}
	}

	void wm_simple_scene_system::add_tag(const ptr<node> node, const std::string& tag) {
		int32_t tag_hash = utility::hash(tag);
		int32_t node_hash = node.get_id();
		if(contains_tags(node)) {
			node_tags.at(node_hash).insert(tag);
			tag_nodes.at(tag_hash).insert(node_hash);
		} else {
			std::unordered_set<std::string> tags = {tag};
			node_tags.insert_or_assign(node_hash, tags);
			std::unordered_set<int32_t> nodes = {node_hash};
			tag_nodes.insert_or_assign(tag_hash, nodes);
		}
	}

	void wm_simple_scene_system::remove_tag(const ptr<node> node, const std::string& tag) {
		int32_t node_hash = node.get_id();
		if(contains_tags(node)) {
			int32_t tag_hash = utility::hash(tag);
			node_tags.at(node_hash).erase(tag);
			tag_nodes.at(tag_hash).erase(node_hash);
			if(node_tags.at(node_hash).size() == 0) {
				node_tags.erase(node_hash);
				tag_nodes.erase(tag_hash);
			}
		}
	}

	bool wm_simple_scene_system::contains_tags(const ptr<node> node) const {
		return node_tags.find(node.get_id()) != node_tags.end();
	}

	bool wm_simple_scene_system::contains_tag(const ptr<node> node, const std::string& tag) const {
		auto tags = get_tags(node);
		for(auto& t : tags) {
			if(t == tag) {
				return true;
			}
		}
		return false;
	}

	std::vector<std::string> wm_simple_scene_system::get_tags(const ptr<node> node) const {
		std::vector<std::string> result;
		if(contains_tags(node)) {
			auto& tags = node_tags.at(node.get_id());
			for(auto& tag : tags) {
				result.push_back(tag);
			}
		}
		return result;
	}

	bool wm_simple_scene_system::contains_nodes(const std::string& tag) const {
		int32_t tag_hash = utility::hash(tag);
		return tag_nodes.find(tag_hash) != tag_nodes.end();
	}

	std::vector<ptr<node>> wm_simple_scene_system::get_nodes(const std::string& tag) const {
		std::vector<ptr<node>> result;
		if(contains_nodes(tag)) {
			int32_t tag_hash = utility::hash(tag);
			auto& node_ids = tag_nodes.at(tag_hash);
			for(auto node_id : node_ids) {
				result.push_back(ptr<wm::node>(node_id));
			}
		}
		return result;
	}

	std::vector<ptr<node>> wm_simple_scene_system::get_nodes(std::function<bool(const std::unordered_set<std::string>& tags)> func) const {
		std::vector<ptr<node>> result;
		for(auto& a : node_tags) {
			if(func(a.second)) {
				result.push_back(ptr<node>(a.first));
			}
		}
		return result;
	}

	ptr<node> wm_simple_scene_system::get_node(const std::string& tag) const {
		if(contains_nodes(tag)) {
			int32_t tag_hash = utility::hash(tag);
			auto& node_ids = tag_nodes.at(tag_hash);
			for(auto node_id : node_ids) {
				return ptr<wm::node>(node_id);
			}
		}
		return ptr<wm::node>(nullptr);
	}

	ptr<node> wm_simple_scene_system::get_node(std::function<bool(const std::unordered_set<std::string>& tags)> func) const {
		for(auto& a : node_tags) {
			if(func(a.second)) {
				return ptr<node>(a.first);
			}
		}
		return ptr<node>(nullptr);
	}

	wm_simple_scene_system::~wm_simple_scene_system() {
		WM_LOG_INFO_1("simple scene system destroyed");
	}

}
