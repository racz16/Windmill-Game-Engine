#pragma once

#include "scene/node.h"

namespace wm {

	class wm_node: public node {
	private:
		int32_t id = -1;
		bool destructible = false;
	public:
		wm_node();
		wm_node(const int32_t id);

		wm_node& operator=(const wm_node& other_node);

		ptr_view<wm::transform> get_transform() const override;
		ptr<node> get_ptr() const override;

		ptr<node> get_root() const override;
		ptr<node> get_parent() const override;
		void set_parent(const ptr<node> parent) override;
		int32_t get_children_count() const override;
		ptr<node> get_child(const int32_t index) const override;
		void add_child(const ptr<node> child) override;

		void add_tag(const std::string& tag) override;
		void remove_tag(const std::string& tag) override;
		bool contains_tag(const std::string& tag) const override;
		std::vector<std::string> get_tags() const override;

		~wm_node();
	};

}
