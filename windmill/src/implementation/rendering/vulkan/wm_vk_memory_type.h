#pragma once

namespace wm {

	struct wm_vk_memory_type {
		uint32_t memory_type_index;
		VkMemoryPropertyFlags memory_property_flags;

		bool is_memory_type(const VkMemoryPropertyFlags flags) const;
	};

}
