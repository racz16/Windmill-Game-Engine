#pragma once

#include "../ptr/ptr.h"
#include "../core/key.h"
#include "transform.h"
#include "../component/component.h"

namespace wm {

	class component;

	class WM_PUBLIC node {
	protected:
		int32_t id = -1;

		virtual const ptr<node> get_ptr() const;
		virtual void initialize(const int32_t id);
		virtual void initialize_node(const ptr<transform> transform);

		virtual const std::vector<ptr<component>> get_components_impl(const key<component>& key) const = 0;
		virtual ptr<component> get_component_impl(const key<component>& key) const = 0;
		virtual void add_component_impl(const ptr<component> component, const key<wm::component>& key) const = 0;
		virtual void remove_component_impl(const ptr<component> component) const = 0;
	public:
		static ptr<node> create();
		static key<node> get_key();

		virtual const ptr<transform> get_transform() const = 0;

		virtual bool is_ancestor_of(const ptr<node> node) const = 0;
		virtual bool is_descendant_of(const ptr<node> node) const = 0;
		virtual ptr<node> get_root() const = 0;
		virtual ptr<node> get_parent() const = 0;
		virtual const std::vector<ptr<node>> get_children() const = 0;
		virtual void add_child(const ptr<node> child) = 0;
		virtual void remove_child(const ptr<node> child) = 0;
		virtual void set_parent(const ptr<node> parent) = 0;

		virtual void add_tag(const tag& tag) = 0;
		virtual void remove_tag(const tag& tag) = 0;
		virtual bool contains_tag(const tag& tag) const = 0;
		virtual const std::vector<tag> get_tags() const = 0;

		template<class T>
		const std::vector<ptr<T>> get_components(const key<T>& key) const {
			return get_components_impl(key.get_name());
		}

		template<class T>
		ptr<T> get_component(const key<T>& key) const {
			return get_component_impl(key.get_name()).convert<T>();
		}

		template<class T>
		void add_component(const ptr<T> component, const key<T>& key) {
			wm::key<wm::component> new_key{key.get_name()};
			add_component_impl(component.convert<wm::component>(), new_key);
		}

		template<class T>
		void remove_component(const ptr<T> component) {
			remove_component_impl(component.convert<wm::component>());
		}

		virtual ~node();
	};

}
