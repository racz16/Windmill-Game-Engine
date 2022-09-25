#pragma once

#include "scene/node.h"

namespace wm {

	class wm_node: public node {
	protected:
		const std::vector<ptr<component>> get_components_impl(const key<component>& key) const override;
		ptr<component> get_component_impl(const key<component>& key) const override;
		void add_component_impl(const ptr<component> component, const key<wm::component>& key) const override;
		void remove_component_impl(const ptr<component> component) const override;
	public:
		void initialize(const int32_t id) override;

		const ptr<wm::transform> get_transform() const override;

		bool is_ancestor_of(const ptr<node> node) const override;
		bool is_descendant_of(const ptr<node> node) const override;
		ptr<node> get_root() const override;
		ptr<node> get_parent() const override;
		const std::vector<ptr<node>> get_children() const override;
		void add_child(const ptr<node> child) override;
		void remove_child(const ptr<node> child) override;
		void set_parent(const ptr<node> parent) override;

		void add_tag(const tag& tag) override;
		void remove_tag(const tag& tag) override;
		bool contains_tag(const tag& tag) const override;
		const std::vector<tag> get_tags() const override;

		~wm_node();
	};

}
