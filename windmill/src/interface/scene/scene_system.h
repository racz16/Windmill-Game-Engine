#pragma once

#include "../core/system.h"
#include "../scene/node.h"

namespace wm {

	class WM_PUBLIC scene_system: public system {
	public:
		static ptr<scene_system> create();
		static key<scene_system> get_key();

		virtual ptr<node> get_parent(const ptr<node> node) const = 0;
		virtual void set_parent(const ptr<node> child, const ptr<node> parent) = 0;
		virtual ptr<node> get_child(const ptr<node> node, const int32_t index) const = 0;
		virtual int32_t get_child_count(const ptr<node> node) const = 0;
		virtual ptr<node> get_node(const ptr<transform> transform) const = 0;
		virtual ptr<transform> get_transform(const ptr<node> node) const = 0;
		virtual void set_transform(const ptr<node> node, const ptr<transform> transform) = 0;

		virtual void add_tag(const ptr<node> node, const std::string& tag) = 0;
		virtual void remove_tag(const ptr<node> node, const std::string& tag) = 0;
		virtual bool contains_tags(const ptr<node> node) const = 0;
		virtual bool contains_tag(const ptr<node> node, const std::string& tag) const = 0;
		virtual std::vector<std::string> get_tags(const ptr<node> node) const = 0;
		virtual bool contains_nodes(const std::string& tag) const = 0;
		virtual std::vector<ptr<node>> get_nodes(const std::string& tag) const = 0;
		virtual std::vector<ptr<node>> get_nodes(std::function<bool(const std::unordered_set<std::string>& tags)> func) const = 0;
		virtual ptr<node> get_node(const std::string& tag) const = 0;
		virtual ptr<node> get_node(std::function<bool(const std::unordered_set<std::string>& tags)> func) const = 0;
	};

}
