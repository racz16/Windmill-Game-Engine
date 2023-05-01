#include "wm_vk_memory_type.h"

namespace wm {

	bool wm_vk_memory_type::is_memory_type(const VkMemoryPropertyFlags flags) const {
		return (memory_property_flags & flags) == flags;
	}

}
