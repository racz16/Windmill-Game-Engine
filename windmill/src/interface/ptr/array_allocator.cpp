#include "array_allocator.h"

namespace wm {

	std::unordered_map<int32_t, ptr<vector_array<void>>> array_allocator::arrays;

	void array_allocator::destroy() {
		for(std::pair<int32_t, ptr<vector_array<void>>> element : arrays) {
			element.second.destroy();
		}
		arrays.clear();
	}

}
