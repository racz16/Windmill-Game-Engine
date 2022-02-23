#include "array_allocator_system.h"

namespace wm {

	ptr<array_allocator_system> array_allocator_system::create() {
		return ptr<array_allocator_system>(new array_allocator_system());
	}

	key<array_allocator_system> array_allocator_system::get_key() {
		static const key<array_allocator_system> key("WM_ARRAY_ALLOCATOR_SYSTEM");
		return key;
	}

	array_allocator_system::array_allocator_system() { }

	void array_allocator_system::update() { }

	bool array_allocator_system::is_active() const {
		return active;
	}

	void array_allocator_system::set_active(const bool active) {
		this->active = active;
	}

	array_allocator_system::~array_allocator_system() {
		for(std::pair<int32_t, ptr<vector_array<void>>> element : arrays) {
			element.second.destroy();
		}
		arrays.clear();
	}

}
