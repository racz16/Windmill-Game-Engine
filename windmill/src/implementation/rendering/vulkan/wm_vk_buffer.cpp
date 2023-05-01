#include "wm_vk_buffer.h"

#include "defines/log_defines.h"

#include "wm_vulkan_rendering_context.h"

#include "wm_vk_defines.h"

namespace wm {

	wm_vk_buffer::wm_vk_buffer(const gpu_buffer_descriptor descriptor, const bool is_staging_buffer)
		: type_flags(descriptor.type_flags), cpu_write_frequency(descriptor.cpu_write_frequency), data_size(descriptor.data_size), stride(descriptor.stride) {

		VkDevice device = wm_vulkan_rendering_context::device;
		create_buffer(is_staging_buffer);
		create_device_memory();
		VK_CALL(vkBindBufferMemory(device, native_handle, device_memory, 0));

		if(memory_type.is_memory_type(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) && (cpu_write_frequency != usage_frequency::NEVER || descriptor.data)) {
			VK_CALL(vkMapMemory(device, device_memory, 0, data_size, 0, &mapped_memory));
		}
		if(descriptor.data) {
			set_data(descriptor.data, data_size);
			if(mapped_memory && cpu_write_frequency == usage_frequency::NEVER) {
				vkUnmapMemory(device, device_memory);
				mapped_memory = VK_NULL_HANDLE;
			}
		}

		WM_LOG_INFO_2("Vulkan buffer created");
	}

	void wm_vk_buffer::create_buffer(const bool is_staging_buffer) {
		// TODO: read back data to the RAM
		memory_type = wm_vulkan_rendering_context::get_memory_type(cpu_write_frequency, is_staging_buffer);

		VkDevice device = wm_vulkan_rendering_context::device;
		VkBufferCreateInfo buffer_create_info;
		buffer_create_info.flags = 0;
		buffer_create_info.pNext = VK_NULL_HANDLE;
		buffer_create_info.pQueueFamilyIndices = VK_NULL_HANDLE;
		buffer_create_info.queueFamilyIndexCount = 0;
		buffer_create_info.sharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
		buffer_create_info.size = static_cast<VkDeviceSize>(data_size);
		buffer_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buffer_create_info.usage = get_usage(is_staging_buffer);

		VK_CALL(vkCreateBuffer(device, &buffer_create_info, VK_NULL_HANDLE, &native_handle));
	}

	void wm_vk_buffer::create_device_memory() {
		VkDevice device = wm_vulkan_rendering_context::device;
		VkMemoryRequirements memory_requirements;
		vkGetBufferMemoryRequirements(device, native_handle, &memory_requirements);

		VkMemoryAllocateInfo memory_allocation_info{};
		memory_allocation_info.allocationSize = memory_requirements.size;
		memory_allocation_info.memoryTypeIndex = memory_type.memory_type_index;
		memory_allocation_info.pNext = VK_NULL_HANDLE;
		memory_allocation_info.sType = VkStructureType::VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

		// TODO: alignment and memoryTypeBits
		// TODO: suballocate one memory allocation to multiple buffers

		VK_CALL(vkAllocateMemory(device, &memory_allocation_info, nullptr, &device_memory));
	}

	VkBufferUsageFlags wm_vk_buffer::get_usage(const bool is_staging_buffer) const {
		VkBufferUsageFlags usage = 0;
		if(WM_CONTAINS_FLAG(type_flags, gpu_buffer_type::VERTEX_BUFFER)) {
			usage |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		}
		if(WM_CONTAINS_FLAG(type_flags, gpu_buffer_type::INDEX_BUFFER)) {
			usage |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		}
		if(WM_CONTAINS_FLAG(type_flags, gpu_buffer_type::UNIFORM_BUFFER)) {
			usage |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		}

		if(is_staging_buffer) {
			usage |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		} else if(!memory_type.is_memory_type(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)) {
			usage |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		}
		return usage;
	}

	void wm_vk_buffer::set_data_with_staging_buffer(void* data, const int32_t data_size, const int32_t offset) {
		gpu_buffer_descriptor staging_descriptor;
		staging_descriptor.cpu_write_frequency = usage_frequency::FREQUENTLY;
		staging_descriptor.data = data;
		staging_descriptor.data_size = data_size;
		staging_descriptor.stride = stride;
		staging_descriptor.type_flags = type_flags;
		wm_vk_buffer staging_buffer(staging_descriptor, true);

		VkCommandBuffer command_buffer = wm_vulkan_rendering_context::begin_single_time_commands();
		VkBufferCopy buffer_copy;
		buffer_copy.dstOffset = offset;
		buffer_copy.size = data_size;
		buffer_copy.srcOffset = offset;
		vkCmdCopyBuffer(command_buffer, std::any_cast<VkBuffer>(staging_buffer.get_native_handle()), native_handle, 1, &buffer_copy);
		wm_vulkan_rendering_context::end_single_time_commands(command_buffer);
	}

	std::any wm_vk_buffer::get_native_handle() const {
		return native_handle;
	}

	gpu_buffer_type wm_vk_buffer::get_type_flags() const {
		return type_flags;
	}

	usage_frequency wm_vk_buffer::get_cpu_write_frequency() const {
		return cpu_write_frequency;
	}

	void wm_vk_buffer::set_data(void* data, const int32_t data_size, const int32_t offset) {
		if(mapped_memory) {
			std::memcpy(static_cast<char*>(mapped_memory) + offset, data, data_size);
			if(!memory_type.is_memory_type(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
				flush_mapped_memory(data_size, offset);
			}
		} else {
			set_data_with_staging_buffer(data, data_size, offset);
		}
	}

	void wm_vk_buffer::flush_mapped_memory(const int32_t data_size, const int32_t offset) const {
		VkDevice device = wm_vulkan_rendering_context::device;
		VkMappedMemoryRange mapped_memory_range;
		mapped_memory_range.memory = device_memory;
		mapped_memory_range.offset = static_cast<VkDeviceSize>(offset);
		mapped_memory_range.pNext = VK_NULL_HANDLE;
		mapped_memory_range.size = static_cast<VkDeviceSize>(data_size);
		mapped_memory_range.sType = VkStructureType::VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;

		vkFlushMappedMemoryRanges(device, 1, &mapped_memory_range);
	}

	int32_t wm_vk_buffer::get_data_size() const {
		return data_size;
	}

	int32_t wm_vk_buffer::get_stride() const {
		return stride;
	}

#ifdef WM_BUILD_DEBUG

	void wm_vk_buffer::set_debug_label(const std::string& label) {
		VkDebugUtilsObjectNameInfoEXT debug_utils_object_name_info;
		debug_utils_object_name_info.objectHandle = reinterpret_cast<uint64_t>(native_handle);
		debug_utils_object_name_info.objectType = VkObjectType::VK_OBJECT_TYPE_BUFFER;
		debug_utils_object_name_info.pNext = VK_NULL_HANDLE;
		debug_utils_object_name_info.pObjectName = label.c_str();
		debug_utils_object_name_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;

		PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT = wm_vulkan_rendering_context::vkSetDebugUtilsObjectNameEXT;
		VkDevice device = wm_vulkan_rendering_context::device;
		VK_CALL(vkSetDebugUtilsObjectNameEXT(device, &debug_utils_object_name_info));
	}

#endif

	wm_vk_buffer::~wm_vk_buffer() {
		VkDevice device = wm_vulkan_rendering_context::device;
		if(mapped_memory) {
			vkUnmapMemory(device, device_memory);
		}
		vkDestroyBuffer(device, native_handle, VK_NULL_HANDLE);
		vkFreeMemory(device, device_memory, VK_NULL_HANDLE);
		WM_LOG_INFO_2("Vulkan buffer destroyed");
	}

}
