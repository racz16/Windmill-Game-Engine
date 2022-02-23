#pragma once

#include "../ptr/ptr.h"
#include "../core/key.h"

namespace wm {

	class WM_PUBLIC node {
	private:
		static bool type_added;
	public:
		static ptr<node> create();
		static key<node> get_key();

		virtual std::string get_name() const = 0;
		virtual void set_name(const std::string& name) = 0;
	};

}
