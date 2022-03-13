#pragma once

#include "../ptr/ptr.h"
#include "../core/key.h"
#include "transform.h"

namespace wm {

	class WM_PUBLIC node {
	private:
		static bool type_added;
	public:
		static ptr<node> create();
		static key<node> get_key();

		virtual ptr<node> get_ptr() const = 0;
		virtual ptr_view<transform> get_transform() const = 0;

		virtual ptr<node> get_root() const = 0;
		virtual ptr<node> get_parent() const = 0;
		virtual void set_parent(const ptr<node> parent) = 0;
		virtual int32_t get_children_count() const = 0;
		virtual ptr<node> get_child(const int32_t index) const = 0;
		virtual void add_child(const ptr<node> child) = 0;

		virtual void add_tag(const std::string& tag) = 0;
		virtual void remove_tag(const std::string& tag) = 0;
		virtual bool contains_tag(const std::string& tag) const = 0;
		virtual std::vector<std::string> get_tags() const = 0;

		virtual ~node() {}
	};

}
