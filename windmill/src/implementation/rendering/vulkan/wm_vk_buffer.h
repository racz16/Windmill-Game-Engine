#pragma once

#include "rendering/gpu_buffer.h"

#include "wm_vk_memory_type.h"

namespace wm {

	class wm_vk_buffer: public gpu_buffer {
	private:
		VkBuffer native_handle = VK_NULL_HANDLE;
		VkDeviceMemory device_memory = VK_NULL_HANDLE;
		gpu_buffer_type type_flags;
		usage_frequency cpu_write_frequency;
		int32_t data_size;
		void* mapped_memory = VK_NULL_HANDLE;
		int32_t stride;
		wm_vk_memory_type memory_type;

		void create_buffer(const bool is_staging_buffer);
		void create_device_memory();
		VkBufferUsageFlags get_usage(const bool is_staging_buffer) const;
		void set_data_with_staging_buffer(void* data, const int32_t data_size, const int32_t offset);
		void flush_mapped_memory(const int32_t data_size, const int32_t offset) const;
	public:
		wm_vk_buffer(const gpu_buffer_descriptor descriptor, const bool is_staging_buffer = false);
		std::any get_native_handle() const override;
		int32_t get_data_size() const override;
		gpu_buffer_type get_type_flags() const override;
		usage_frequency get_cpu_write_frequency() const override;
	#ifdef WM_BUILD_DEBUG
		void set_debug_label(const std::string& label) override;
	#endif
		void set_data(void* data, const int32_t data_size, const int32_t offset = 0) override;
		int32_t get_stride() const override;
		~wm_vk_buffer();
	};

}
