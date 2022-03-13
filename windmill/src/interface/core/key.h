#pragma once

#include "utility.h"

namespace wm {

	template<class T>
	class key {
	private:
		int32_t hash;
		std::string name;
	public:

		key(const std::string& name) {
			hash = utility::hash(name);
			this->name = name;
		}

		int32_t get_hash() const {
			return hash;
		}

		std::string get_name() const {
			return name;
		}

	};

}
