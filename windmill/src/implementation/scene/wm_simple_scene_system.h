#pragma once

#include "scene/scene_system.h"

#include "../core/wm_base_system.h"

namespace wm {

	class wm_simple_scene_system: public wm_base_system, public scene_system {
	private:
		std::unordered_map<int32_t, ptr<node>> child_parent;
		std::unordered_map<int32_t, std::vector<ptr<node>>> parent_children;
		std::unordered_map<int32_t, ptr<node>> transform_node;
		std::unordered_map<int32_t, ptr<transform>> node_transform;
		std::unordered_map<int32_t, std::unordered_set<std::string>> node_tags;
		std::unordered_map<int32_t, std::unordered_set<int32_t>> tag_nodes;

		void set_parent_to_null(const ptr<node> child, const ptr<node> old_parent);
		bool is_ancestor_of(const ptr<node> node, const ptr<wm::node> ancestor) const;
	public:
		wm_simple_scene_system();
		void update() override;

		ptr<node> get_parent(const ptr<node> node) const override;
		void set_parent(const ptr<node> child, const ptr<node> parent) override;
		ptr<node> get_child(const ptr<node> node, const int32_t index) const override;
		int32_t get_child_count(const ptr<node> node) const override;
		ptr<node> get_node(const ptr<transform> transform) const override;
		ptr<transform> get_transform(const ptr<node> node) const override;
		void set_transform(const ptr<node> node, const ptr<transform> transform) override;

		void add_tag(const ptr<node> node, const std::string& tag) override;
		void remove_tag(const ptr<node> node, const std::string& tag) override;
		bool contains_tags(const ptr<node> node) const override;
		bool contains_tag(const ptr<node> node, const std::string& tag) const override;
		std::vector<std::string> get_tags(const ptr<node> node) const override;
		bool contains_nodes(const std::string& tag) const override;
		std::vector<ptr<node>> get_nodes(const std::string& tag) const override;
		std::vector<ptr<node>> get_nodes(std::function<bool(const std::unordered_set<std::string>& tags)> func) const override;
		ptr<node> get_node(const std::string& tag) const override;
		ptr<node> get_node(std::function<bool(const std::unordered_set<std::string>& tags)> func) const override;

		~wm_simple_scene_system() override;

		bool is_active() const override {
			return wm_base_system::is_active();
		}
		void set_active(const bool active) override {
			wm_base_system::set_active(active);
		}
	};

}
