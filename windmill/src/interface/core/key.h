#pragma once

#include "tag.h"

namespace wm {

	template<class T>
	class key: public tag {
		using tag::tag;
	};

}

template<class T>
class WM_PUBLIC std::hash<wm::key<T>> {
public:

	size_t operator()(const wm::key<T>& key) const {
		return key.get_hash();
	}

};
