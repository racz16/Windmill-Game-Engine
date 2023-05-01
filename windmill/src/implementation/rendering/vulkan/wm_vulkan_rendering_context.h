#pragma once

#include "rendering/rendering_context.h"
#include "rendering/gpu_mesh.h"

#include "../wm_gpu_vertex.h"
#include "wm_vk_memory_type.h"

namespace wm {

	class wm_vulkan_rendering_context: public rendering_context {
	public:
		static VkInstance instance;
		static VkDevice device;
		static VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
		static PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;

	private:

		struct wm_vk_memory_profile {
			VkMemoryPropertyFlags positive;
			VkMemoryPropertyFlags negative;
		};

		static const int32_t MAX_FRAMES_IN_FLIGHT = 2;

		static wm_vk_memory_type never_changing_memory;
		static wm_vk_memory_type frequently_changing_memory;

		static std::vector<wm_gpu_vertex> vertices;
		static std::vector<uint32_t> indices;
		VkDebugUtilsMessengerEXT debug_utils_messenger = VK_NULL_HANDLE;
		//device
		VkPhysicalDevice physical_device = VK_NULL_HANDLE;
		bool anisotropy = false;
		float max_anisotropy = -1.0;
		static int32_t graphics_queue_family_index;
		int32_t presentation_queue_family_index = -1;
		static VkQueue graphics_queue;
		VkQueue presentation_queue = VK_NULL_HANDLE;
		//swap chain
		VkSurfaceKHR surface = VK_NULL_HANDLE;
		VkSwapchainKHR swap_chain = VK_NULL_HANDLE;
		VkSurfaceFormatKHR surface_format;
		VkExtent2D swap_chain_extent;
		std::vector<VkImage> swap_chain_images;
		std::vector<VkImageView> swap_chain_image_views;
		std::vector<VkFramebuffer> swap_chain_framebuffers;
		bool framebuffer_resized = false;
		bool minimized = false;
		//render pass
		VkRenderPass render_pass = VK_NULL_HANDLE;
		//pipeline
		VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
		VkPipeline pipeline = VK_NULL_HANDLE;
		//buffers
		ptr<gpu_mesh> mesh = nullptr;
		std::vector<ptr<gpu_buffer>> uniform_buffers;
		VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;
		std::vector<VkDescriptorSet> descriptor_sets;
		VkDescriptorSetLayout descriptor_set_layout = VK_NULL_HANDLE;
		//texture
		VkImage texture_image = VK_NULL_HANDLE;
		uint32_t texture_mipmap_level_count = 1;
		VkImageView texture_image_view = VK_NULL_HANDLE;
		VkSampler texture_sampler = VK_NULL_HANDLE;
		VkDeviceMemory texture_image_device_memory = VK_NULL_HANDLE;
		//color texture
		VkImage color_image = VK_NULL_HANDLE;
		VkImageView color_image_view = VK_NULL_HANDLE;
		VkDeviceMemory color_image_device_memory = VK_NULL_HANDLE;
		VkSampleCountFlagBits msaa_sample_count = VK_SAMPLE_COUNT_1_BIT;
		//depth texture
		VkImage depth_image = VK_NULL_HANDLE;
		VkImageView depth_image_view = VK_NULL_HANDLE;
		VkDeviceMemory depth_image_device_memory = VK_NULL_HANDLE;
		//command buffers
		static VkCommandPool command_pool;
		std::vector<VkCommandBuffer> command_buffers;
		//sync
		std::vector<VkSemaphore> image_is_available;
		std::vector<VkSemaphore> rendering_is_finished;
		std::vector<VkFence> in_flight;
		std::vector<VkFence> images_in_flight;
		int32_t frame_index;
		//imgui
		VkDescriptorPool imgui_descriptor_pool = VK_NULL_HANDLE;

		//instance
		void create_instance();
		//layers
		std::vector<const char*> get_layers() const;
		std::vector<const char*> get_required_layers() const;
		std::vector<VkLayerProperties> get_available_layers() const;
		void log_available_layers(const std::vector<VkLayerProperties>& available_layers) const;
		void validate_layers(const std::vector<const char*>& required_layers, const std::vector<VkLayerProperties>& available_layers) const;
		//instance extensions
		std::vector<const char*> get_extensions() const;
		std::vector<const char*> get_required_extensions() const;
		std::vector<VkExtensionProperties> get_available_extensions() const;
		void log_available_extensions(const std::vector<VkExtensionProperties>& available_extensions) const;
		void validate_extensions(const std::vector<const char*>& required_extensions, const std::vector<VkExtensionProperties>& available_extensions) const;
		//debug callback
		VkDebugUtilsMessengerCreateInfoEXT create_debug_utils_messenger_create_info() const;
		void create_debug_utils_messenger();
		void destroy_debug_utils_messenger();
		//debug
		std::string get_object_type(const VkObjectType object_type) const;
		void set_object_label(const VkObjectType object_type, const uint64_t id, const std::string& name) const;
		void start_group(const std::string& name, const VkCommandBuffer command_buffer) const;
		void stop_group(const VkCommandBuffer command_buffer) const;
		void set_marker(const std::string& name, const VkCommandBuffer command_buffer) const;
		//device
		void create_device();
		void get_physical_device();
		void update_memory_infos();
		void update_memory_info(wm_vk_memory_type& memory_info, int32_t& selected, const uint32_t memory_index, const std::vector<wm_vk_memory_profile>& profiles);
		//device extensions
		std::vector<const char*> get_required_device_extensions() const;
		std::vector<VkExtensionProperties> get_available_device_extensions(const VkPhysicalDevice physical_device) const;
		bool are_device_extensions_supported(const std::vector<const char*>& required_device_extensions, const std::vector<VkExtensionProperties>& available_device_extensions) const;
		//swap chain
		void register_event_handlers();
		void create_swap_chain();
		void recreate_swap_chain();
		VkSurfaceFormatKHR get_surface_format() const;
		VkPresentModeKHR get_surface_presentation_mode() const;
		VkSurfaceCapabilitiesKHR get_surface_capabilities() const;
		VkExtent2D get_swap_chain_extent(const VkSurfaceCapabilitiesKHR surface_capabilities) const;
		void create_swap_chain_image_views();
		void cleanup_swap_chain();
		//pipeline
		void create_descriptor_set_layout();
		void create_pipeline();
		void create_render_pass();
		void create_framebuffers();
		//shader
		std::vector<char> read_file(const std::string& file_name) const;
		VkShaderModule create_shader_module(const std::vector<char>& code) const;
		//vertex buffer
		void create_mesh(const std::string& path);
		void load_mesh(const std::string& path);
		//texture
		void create_texture_image();
		void create_texture_image_view();
		void create_image(const glm::ivec2& size, const uint32_t mipmap_level_count, const VkSampleCountFlagBits sample_count, const VkFormat format, const VkImageTiling image_tiling, const VkImageUsageFlags image_usage, const VkMemoryPropertyFlags properties, VkImage& texture_image, VkDeviceMemory& texture_image_device_memory);
		VkImageView create_image_view(const VkImage image, const VkFormat format, const VkImageAspectFlags image_aspect_flags, const uint32_t mipmap_level_count);
		void create_texture_sampler();
		void create_sampler(const uint32_t mipmap_level_count, const float max_anisotropy, VkSampler& sampler);
		void transition_image_layout(VkImage image, VkImageLayout old_layout, VkImageLayout new_layout, const uint32_t mipmap_level_count);
		void copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		void generate_mipmaps(VkImage image, const VkFormat format, const glm::vec2& size, const uint32_t mipmap_level_count);
		//color texture
		void create_color_resources();
		VkSampleCountFlagBits get_msaa_sample_count(const VkPhysicalDeviceProperties physical_device_properties) const;
		//depth texture
		void create_depth_resources();
		VkFormat get_depth_format() const;
		VkFormat get_depth_format(const std::vector<VkFormat>& formats, const VkImageTiling image_tiling, const VkFormatFeatureFlags format_features) const;
		bool depth_has_stencil_component(const VkFormat format) const;
		//uniform buffers
		void create_uniform_buffers();
		void update_uniform_buffer(const uint32_t image_index) const;
		void create_descriptor_pool();
		void create_descriptor_sets();
		//command buffers
		static void create_command_pool();
		void create_command_buffers();
		void update_command_buffer(const uint32_t image_index);
		//sync
		void create_semaphores();
		//imgui
		void initialize_imgui();
		void create_imgui_descriptor_pool();
		void create_imgui_font_texture() const;
		void destroy_imgui() const;
	public:
		static VkCommandBuffer begin_single_time_commands();
		static void end_single_time_commands(const VkCommandBuffer command_buffer);
		static wm_vk_memory_type get_memory_type(const usage_frequency usage, const bool is_staging);

		void initialize() override;
		void update() override;
		void destroy() override;
	};

}
