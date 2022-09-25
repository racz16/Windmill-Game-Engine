#pragma once

#include "scene/scene_system.h"

namespace wm {

	class wm_simple_scene_system: public scene_system {
	private:
		std::unordered_map<ptr<node>, ptr<node>> child_parent;
		std::unordered_map<ptr<node>, std::vector<ptr<node>>> parent_children;
		std::unordered_map<ptr<transform>, ptr<node>> transform_node;
		std::unordered_map<ptr<node>, ptr<transform>> node_transform;
		std::unordered_map<ptr<node>, std::vector<tag>> node_tags;
		std::unordered_map<tag, std::vector<ptr<node>>> tag_nodes;
		std::unordered_map<ptr<node>, std::unordered_map<key<component>, std::vector<ptr<component>>>> node_components;
		std::unordered_map<ptr<component>, std::pair<ptr<node>, key<component>>> component_node_and_key;
	protected:
		void initialize_node(const ptr<node> node, const ptr<transform> transform) override;
		void cleanup(const ptr<node> node) override;

		ptr<node> get_node_by_component_impl(const ptr<component> component) const override;
		ptr<component> get_component_impl(const ptr<node> node, const key<component>& key) const override;
		std::vector<ptr<component>> get_components(const ptr<node> node) const;
		const std::vector<ptr<component>> get_components_impl(const ptr<node> node, const key<component>& key) const override;
		void add_component_impl(const ptr<node> node, const ptr<component> component, const key<wm::component>& key) override;
		void remove_component_impl(const ptr<node> node, const ptr<component> component) override;
		void cleanup(const ptr<component> component) override;
	public:
		wm_simple_scene_system();

		bool is_descendant_of(const ptr<node> descendant, const ptr<node> ancestor) const override;
		ptr<node> get_parent(const ptr<node> node) const override;
		const std::vector<ptr<node>> get_children(const ptr<node> node) const override;
		ptr<node> get_node_by_transform(const ptr<transform> transform) const override;
		const std::vector<ptr<node>> get_nodes() const;
		void add_child(const ptr<node> parent, const ptr<node> child) override;
		void remove_child(const ptr<node> parent, const ptr<node> child) override;

		ptr<transform> get_transform(const ptr<node> node) const override;

		bool contains_any_tags(const ptr<node> node) const override;
		bool contains_tag(const ptr<node> node, const tag& tag) const override;
		const std::vector<tag> get_tags(const ptr<node> node) const override;
		void add_tag(const ptr<node> node, const tag& tag) override;
		void remove_tag(const ptr<node> node, const tag& tag) override;

		bool contains_any_nodes_by_tag(const tag& tag) const override;
		bool contains_node_by_tag(const ptr<node> node, const tag& tag) const override;
		const std::vector<ptr<node>> get_nodes_by_tag(const tag& tag) const override;
		const std::vector<ptr<node>> get_nodes_by_tags(std::function<bool(const std::vector<tag>& tags)> func) const override;
		ptr<node> get_node_by_tag(const tag& tag) const override;
		ptr<node> get_node_by_tags(std::function<bool(const std::vector<tag>& tags)> func) const override;

		~wm_simple_scene_system() override;
	};

}
