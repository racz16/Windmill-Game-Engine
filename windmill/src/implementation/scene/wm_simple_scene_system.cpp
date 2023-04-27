#include <algorithm>

#include "wm_simple_scene_system.h"

#include "defines/log_defines.h"
#include "core/utility.h"
#include "scene/event/transform/transform_changed_event.h"
#include "scene/event/transform/transform_destroyed_event.h"
#include "scene/event/node/node_child_changed_event.h"
#include "scene/event/node/node_parent_changed_event.h"
#include "scene/event/node/node_tag_changed_event.h"
#include "component/event/component_changed_event.h"

namespace wm {

	wm_simple_scene_system::wm_simple_scene_system() {
		WM_LOG_INFO_1("simple scene system created");
	}

	bool wm_simple_scene_system::is_descendant_of(const ptr_view<node> descendant, const ptr_view<node> ancestor) const {
		auto parent = get_parent(descendant);
		while(parent.is_valid()) {
			if(parent == ancestor) {
				return true;
			}
			parent = get_parent(parent);
		}
		return false;
	}

	ptr<node> wm_simple_scene_system::get_parent(const ptr_view<node> node) const {
		auto find_parent = child_parent.find(node.get_id());
		return find_parent == child_parent.end() ? ptr<wm::node>(nullptr) : find_parent->second;
	}

	std::vector<ptr<node>> wm_simple_scene_system::get_children(const ptr_view<node> node) const {
		auto find_children = parent_children.find(node.get_id());
		return find_children == parent_children.end() ? std::vector<ptr<wm::node>>{} : find_children->second;
	}

	ptr<node> wm_simple_scene_system::get_node_by_transform(const ptr_view<transform> transform) const {
		auto find_node = transform_node.find(transform.get_id());
		return find_node == transform_node.end() ? ptr<node>(nullptr) : find_node->second;
	}

	std::vector<ptr<node>> wm_simple_scene_system::get_nodes() const {
		std::vector<ptr<node>> nodes{};
		for(auto& node_and_transform : node_transform) {
			nodes.push_back(node_and_transform.first);
		}
		return nodes;
	}

	void wm_simple_scene_system::add_child(const ptr<node> parent, const ptr<node> child) {
		WM_ASSERT(parent.is_valid() && child.is_valid() && !get_parent(child).is_valid() && parent != child && !is_descendant_of(parent, child));

		if(parent_children.find(parent) == parent_children.end()) {
			std::vector<ptr<node>> children{};
			parent_children.insert_or_assign(parent, children);
		}
		auto& children = parent_children.at(parent);
		children.push_back(child);

		child_parent.insert_or_assign(child, parent);

		node_parent_changed_event parent_event(child, nullptr, parent);
		engine::get_event_system()->emit_event(node_parent_changed_event::get_key(), parent_event, child);
		node_child_changed_event child_event(parent, true, child);
		engine::get_event_system()->emit_event(node_child_changed_event::get_key(), child_event, parent);
		WM_LOG_INFO_3("child added to node");
	}

	void wm_simple_scene_system::remove_child(const ptr<node> parent, const ptr<node> child) {
		WM_ASSERT(get_parent(child) == parent);

		auto& children = parent_children.at(parent);
		auto find_child = std::find(children.begin(), children.end(), child);
		children.erase(find_child);
		if(children.empty()) {
			parent_children.erase(parent);
		}

		child_parent.erase(child);

		node_parent_changed_event parent_event(child, parent, nullptr);
		engine::get_event_system()->emit_event(node_parent_changed_event::get_key(), parent_event, child);
		node_child_changed_event child_event(parent, false, child);
		engine::get_event_system()->emit_event(node_child_changed_event::get_key(), child_event, parent);
		WM_LOG_INFO_3("child removed from node");
	}

	// transform
	ptr_view<transform> wm_simple_scene_system::get_transform(const ptr_view<node> node) const {
		return get_transform_impl(node);
	}

	ptr<transform> wm_simple_scene_system::get_transform_impl(const ptr_view<node> node) const {
		auto find_transform = node_transform.find(node.get_id());
		return find_transform == node_transform.end() ? ptr<wm::transform>(nullptr) : find_transform->second;
	}

	void wm_simple_scene_system::initialize_node(const ptr<node> node, const ptr<transform> transform) {
		WM_ASSERT(node.is_valid() && transform.is_valid() && node_transform.find(node) == node_transform.end() && transform_node.find(transform) == transform_node.end());
		node_transform.insert_or_assign(node, transform);
		transform_node.insert_or_assign(transform, node);
	}

	void wm_simple_scene_system::cleanup(const ptr<node> node) {
		// parent
		const auto parent = get_parent(node);
		if(parent.is_valid()) {
			remove_child(parent, node);
		}

		// tags
		if(contains_any_tags(node)) {
			for(auto& tag : node_tags.at(node)) {
				remove_tag(node, tag);
			}
		}

		// components
		for(auto component : get_components(node)) {
			component.destroy();
		}

		// children
		for(auto child : get_children(node)) {
			child.destroy();
		}

		auto transform = get_transform_impl(node);
		node_transform.erase(node);
		transform_node.erase(transform);
		transform.destroy();
	}

	void wm_simple_scene_system::cleanup(const ptr<component> component) {
		auto node = get_node_by_component(component);
		if(node.is_valid()) {
			remove_component(node, component);
		}
	}

	// tags
	bool wm_simple_scene_system::contains_any_tags(const ptr_view<node> node) const {
		return node_tags.find(node.get_id()) != node_tags.end();
	}

	bool wm_simple_scene_system::contains_tag(const ptr_view<node> node, const tag& tag) const {
		auto tags = get_tags(node);
		return std::find(tags.begin(), tags.end(), tag) == tags.end();
	}

	std::vector<tag> wm_simple_scene_system::get_tags(const ptr_view<node> node) const {
		if(contains_any_tags(node)) {
			return node_tags.at(node.get_id());
		} else {
			return std::vector<tag>{};
		}
	}

	void wm_simple_scene_system::add_tag(const ptr<node> node, const tag& tag) {
		if(node_tags.find(node) == node_tags.end()) {
			std::vector<wm::tag> tags;
			node_tags.insert_or_assign(node, tags);
		}
		auto& tags = node_tags.at(node);
		if(std::find(tags.begin(), tags.end(), tag) == tags.end()) {
			tags.push_back(tag);
		}
		if(tag_nodes.find(tag) == tag_nodes.end()) {
			std::vector<ptr<wm::node>> nodes;
			tag_nodes.insert_or_assign(tag, nodes);
		}
		auto& nodes = tag_nodes.at(tag);
		if(std::find(nodes.begin(), nodes.end(), node) == nodes.end()) {
			nodes.push_back(node);
			node_tag_changed_event event(node, true, tag);
			engine::get_event_system()->emit_event(node_tag_changed_event::get_key(), event, node);
			WM_LOG_INFO_3("tag " + tag.get_name() + " added to node");
		}
	}

	void wm_simple_scene_system::remove_tag(const ptr<node> node, const tag& tag) {
		if(node_tags.find(node) != node_tags.end()) {
			auto& tags = node_tags.at(node);
			auto find_tag = std::find(tags.begin(), tags.end(), tag);
			if(find_tag != tags.end()) {
				tags.erase(find_tag);
				if(tags.empty()) {
					node_tags.erase(node);
				}
			}
		}
		if(tag_nodes.find(tag) != tag_nodes.end()) {
			auto& nodes = tag_nodes.at(tag);
			auto find_node = std::find(nodes.begin(), nodes.end(), node);
			if(find_node != nodes.end()) {
				nodes.erase(find_node);
				if(nodes.empty()) {
					tag_nodes.erase(tag);
				}
				node_tag_changed_event event(node, false, tag);
				engine::get_event_system()->emit_event(node_tag_changed_event::get_key(), event, node);
				WM_LOG_INFO_3("tag " + tag.get_name() + " removed from node");
			}
		}
	}

	bool wm_simple_scene_system::contains_any_nodes_by_tag(const tag& tag) const {
		return tag_nodes.find(tag) != tag_nodes.end();
	}

	bool wm_simple_scene_system::contains_node_by_tag(const ptr_view<node> node, const tag& tag) const {
		if(contains_any_nodes_by_tag(tag)) {
			auto& nodes = tag_nodes.at(tag);
			return std::find(nodes.begin(), nodes.end(), node) != nodes.end();
		} else {
			return false;
		}
	}

	std::vector<ptr<node>> wm_simple_scene_system::get_nodes_by_tag(const tag& tag) const {
		if(contains_any_nodes_by_tag(tag)) {
			return tag_nodes.at(tag);
		} else {
			return std::vector<ptr<node>>{};
		}
	}

	std::vector<ptr<node>> wm_simple_scene_system::get_nodes_by_tags(std::function<bool(const std::vector<tag>& tags)> func) const {
		std::vector<ptr<node>> result;
		for(auto& nt : node_tags) {
			if(func(nt.second)) {
				result.push_back(nt.first);
			}
		}
		return result;
	}

	ptr<node> wm_simple_scene_system::get_node_by_tag(const tag& tag) const {
		if(contains_any_nodes_by_tag(tag)) {
			return tag_nodes.at(tag).at(0);
		} else {
			return ptr<node>(nullptr);
		}
	}

	ptr<node> wm_simple_scene_system::get_node_by_tags(std::function<bool(const std::vector<tag>& tags)> func) const {
		for(auto& tn : node_tags) {
			if(func(tn.second)) {
				return tn.first;
			}
		}
		return ptr<node>(nullptr);
	}

	// components
	ptr<component> wm_simple_scene_system::get_component_impl(const ptr_view<node> node, const key<component>& key) const {
		if(node_components.find(node.get_id()) != node_components.end()) {
			auto& key_components = node_components.at(node.get_id());
			if(key_components.find(key) != key_components.end()) {
				return key_components.at(key).at(0);
			}
		}
		return ptr<component>(nullptr);
	}

	std::vector<ptr<component>> wm_simple_scene_system::get_components_impl(const ptr_view<node> node, const key<component>& key) const {
		if(node_components.find(node.get_id()) != node_components.end()) {
			auto& key_components = node_components.at(node.get_id());
			if(key_components.find(key) != key_components.end()) {
				return key_components.at(key);
			}
		}
		return std::vector<ptr<component>>{};
	}

	ptr<node> wm_simple_scene_system::get_node_by_component_impl(const ptr_view<component> component) const {
		auto find_node = component_node_and_key.find(component.get_id());
		return find_node == component_node_and_key.end() ? ptr<node>(nullptr) : find_node->second.first;
	}

	std::vector<ptr<component>> wm_simple_scene_system::get_components(const ptr_view<node> node) const {
		std::vector<ptr<component>> components;
		if(node_components.find(node.get_id()) != node_components.end()) {
			auto& key_components = node_components.at(node.get_id());
			for(auto& key_component : key_components) {
				components.insert(components.end(), key_component.second.begin(), key_component.second.end());
			}
		}
		return components;
	}

	void wm_simple_scene_system::add_component_impl(const ptr<node> node, const ptr<component> component, const key<wm::component>& key) {
		WM_ASSERT(node.is_valid() && component.is_valid() && component_node_and_key.find(component) == component_node_and_key.end());

		if(node_components.find(node) == node_components.end()) {
			std::unordered_map<wm::key<wm::component>, std::vector<ptr<wm::component>>> key_components{};
			node_components.insert_or_assign(node, key_components);
		}
		auto& key_components = node_components.at(node);
		if(key_components.find(key) == key_components.end()) {
			std::vector<ptr<wm::component>> components{};
			key_components.insert_or_assign(key, components);
		}
		auto& components = key_components.at(key);
		components.push_back(component);

		component_node_and_key.insert_or_assign(component, std::make_pair(node, key));

		component_changed_event event(component, true, node, key);
		engine::get_event_system()->emit_event(component_changed_event::get_key(), event, component);
		WM_LOG_INFO_3("component with key " + key.get_name() + ", added to node");
	}

	void wm_simple_scene_system::remove_component_impl(const ptr<node> node, const ptr<component> component) {
		WM_ASSERT(component_node_and_key.find(component) != component_node_and_key.end());

		auto& node_and_key = component_node_and_key.at(component);
		auto key = node_and_key.second;
		auto& key_components = node_components.at(node);
		auto& components = key_components.at(key);
		auto find_component = std::find(components.begin(), components.end(), component);
		components.erase(find_component);
		if(components.empty()) {
			key_components.erase(key);
			if(key_components.empty()) {
				node_components.erase(node);
			}
		}

		component_node_and_key.erase(component);

		component_changed_event event(component, false, node, key);
		engine::get_event_system()->emit_event(component_changed_event::get_key(), event, component);
		WM_LOG_INFO_3("component removed from node");
	}

	wm_simple_scene_system::~wm_simple_scene_system() {
		for(auto node : get_nodes()) {
			if(node.is_valid() && !node->get_parent().is_valid()) {
				node.destroy();
			}
		}
		WM_LOG_INFO_1("simple scene system destroyed");
	}

}
