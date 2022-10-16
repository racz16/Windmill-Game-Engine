#include "array_allocator_system.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_CREATE(array_allocator_system, array_allocator_system);

	WM_GET_KEY(array_allocator_system, "WM_ARRAY_ALLOCATOR_SYSTEM");

	array_allocator_system::array_allocator_system() { }

	array_allocator_system::~array_allocator_system() {
		for(std::pair<int32_t, ptr<vector_array<void>>> element : arrays) {
			element.second.destroy();
		}
		arrays.clear();
	}

}
