#pragma once

#include "utility.h"
#include "../defines/general_defines.h"

namespace wm {

	class WM_PUBLIC tag {
	private:
		int32_t hash;
		std::string name;
	public:
		tag(const std::string& name);
		tag(const char* name);
		bool operator==(const tag& other_tag) const;
		bool operator!=(const tag& other_tag) const;
		int32_t get_hash() const;
		std::string get_name() const;
	};

}

template<>
class WM_PUBLIC std::hash<wm::tag> {
public:
	size_t operator()(const wm::tag& tag) const;
};
