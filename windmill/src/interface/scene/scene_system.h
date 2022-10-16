#pragma once

#include "../core/system.h"
#include "../scene/node.h"

namespace wm {

	class transform;
	class component;
	class node;

	class WM_PUBLIC scene_system: public system {
	protected:
		friend class node;
		friend class component;

		virtual void initialize_node(const ptr<node> node, const ptr<transform> transform) = 0;
		virtual void cleanup(const ptr<node> node) = 0;

		//component
		virtual ptr<node> get_node_by_component_impl(const ptr<wm::component> component) const = 0;
		virtual ptr<component> get_component_impl(const ptr<node> node, const key<component>& key) const = 0;
		virtual const std::vector<ptr<component>> get_components_impl(const ptr<node> node, const key<component>& key) const = 0;
		virtual void add_component_impl(const ptr<node> node, const ptr<wm::component> component, const key<wm::component>& key) = 0;
		virtual void remove_component_impl(const ptr<node> node, const ptr<wm::component> component) = 0;
		virtual void cleanup(const ptr<wm::component> component) = 0;
	public:
		static ptr<scene_system> create();
		static key<scene_system> get_key();

		//parent-child
		virtual bool is_descendant_of(const ptr<node> descendant, const ptr<node> ancestor) const = 0;
		virtual ptr<node> get_parent(const ptr<node> node) const = 0;
		virtual const std::vector<ptr<node>> get_children(const ptr<node> node) const = 0;
		virtual void add_child(const ptr<node> parent, const ptr<node> child) = 0;
		virtual void remove_child(const ptr<node> parent, const ptr<node> child) = 0;

		//transform
		virtual ptr<node> get_node_by_transform(const ptr<transform> transform) const = 0;
		virtual ptr<transform> get_transform(const ptr<node> node) const = 0;

		//tags
		virtual bool contains_any_tags(const ptr<node> node) const = 0;
		virtual bool contains_tag(const ptr<node> node, const tag& tag) const = 0;
		virtual const std::vector<tag> get_tags(const ptr<node> node) const = 0;
		virtual void add_tag(const ptr<node> node, const tag& tag) = 0;
		virtual void remove_tag(const ptr<node> node, const tag& tag) = 0;

		virtual bool contains_any_nodes_by_tag(const tag& tag) const = 0;
		virtual bool contains_node_by_tag(const ptr<node> node, const tag& tag) const = 0;
		virtual const std::vector<ptr<node>> get_nodes_by_tag(const tag& tag) const = 0;
		virtual const std::vector<ptr<node>> get_nodes_by_tags(std::function<bool(const std::vector<tag>& tags)> func) const = 0;
		virtual ptr<node> get_node_by_tag(const tag& tag) const = 0;
		virtual ptr<node> get_node_by_tags(std::function<bool(const std::vector<tag>& tags)> func) const = 0;

		//components
		template<class T>
		ptr<node> get_node_by_component(const ptr<T> component) const {
			return get_node_by_component_impl(component.template convert<wm::component>());
		}

		template<class T>
		std::vector<ptr<T>> get_components(const ptr<node> node, const key<T>& key) const {
			wm::key<wm::component> new_key{key.get_name()};
			auto components = get_components_impl(node, new_key);
			std::vector<ptr<T>> results;
			for(auto component : components) {
				results.push_back(component.template convert<T>());
			}
			return results;
		}

		template<class T>
		ptr<T> get_component(const ptr<node> node, const key<T>& key) const {
			wm::key<wm::component> new_key{key.get_name()};
			return get_component_impl(node, new_key).template convert<T>();
		}

		template<class T>
		void add_component(const ptr<node> node, const ptr<T> component, const key<T>& key) {
			wm::key<wm::component> new_key{key.get_name()};
			add_component_impl(node, component.template convert<wm::component>(), new_key);
		}

		template<class T>
		void remove_component(const ptr<node> node, const ptr<T> component) {
			remove_component_impl(node, component.template convert<wm::component>());
		}

	};

}
