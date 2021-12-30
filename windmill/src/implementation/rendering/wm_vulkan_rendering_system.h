#pragma once

#include "rendering/rendering_system.h"

#include "../core/wm_base_system.h"

namespace wm {

	struct gpu_vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texture_coordinates;

		static std::array<VkVertexInputBindingDescription, 1> get_binding_descriptions() {
			std::array<VkVertexInputBindingDescription, 1> binding_descriptions {};

			binding_descriptions.at(0).binding = 0;
			binding_descriptions.at(0).stride = sizeof(gpu_vertex);
			binding_descriptions.at(0).inputRate = VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;

			return binding_descriptions;
		}

		static std::array<VkVertexInputAttributeDescription, 3> get_attribute_descriptions() {
			std::array<VkVertexInputAttributeDescription, 3> attribute_descriptions {};

			attribute_descriptions.at(0).binding = 0;
			attribute_descriptions.at(0).location = 0;
			attribute_descriptions.at(0).format = VkFormat::VK_FORMAT_R32G32B32_SFLOAT;
			attribute_descriptions.at(0).offset = offsetof(gpu_vertex, position);

			attribute_descriptions.at(1).binding = 0;
			attribute_descriptions.at(1).location = 1;
			attribute_descriptions.at(1).format = VkFormat::VK_FORMAT_R32G32B32_SFLOAT;
			attribute_descriptions.at(1).offset = offsetof(gpu_vertex, normal);

			attribute_descriptions.at(2).binding = 0;
			attribute_descriptions.at(2).location = 2;
			attribute_descriptions.at(2).format = VkFormat::VK_FORMAT_R32G32_SFLOAT;
			attribute_descriptions.at(2).offset = offsetof(gpu_vertex, texture_coordinates);

			return attribute_descriptions;
		}

	};

	struct uniform_buffer_object {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
	};

	class wm_vulkan_rendering_system: public wm_base_system, public rendering_system {
	private:
		static const int32_t MAX_FRAMES_IN_FLIGHT = 2;

		static std::vector<gpu_vertex> vertices;
		static std::vector<uint32_t> indices;

		VkInstance instance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT debug_utils_messenger = VK_NULL_HANDLE;
		//device
		VkPhysicalDevice physical_device = VK_NULL_HANDLE;
		bool anisotropy = false;
		float max_anisotropy = -1.0;
		VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
		int32_t graphics_queue_family_index = -1;
		int32_t presentation_queue_family_index = -1;
		VkDevice device = VK_NULL_HANDLE;
		VkQueue graphics_queue = VK_NULL_HANDLE;
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
		VkDescriptorSetLayout descriptor_set_layout = VK_NULL_HANDLE;
		VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
		VkPipeline pipeline = VK_NULL_HANDLE;
		//buffers
		VkBuffer vertex_buffer = VK_NULL_HANDLE;
		VkDeviceMemory vertex_buffer_device_memory = VK_NULL_HANDLE;
		VkBuffer index_buffer = VK_NULL_HANDLE;
		VkDeviceMemory index_buffer_device_memory = VK_NULL_HANDLE;
		std::vector<VkBuffer> uniform_buffers;
		std::vector<VkDeviceMemory> uniform_buffers_device_memory;
		VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;
		std::vector<VkDescriptorSet> descriptor_sets;
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
		VkCommandPool command_pool = VK_NULL_HANDLE;
		std::vector<VkCommandBuffer> command_buffers;
		//sync
		std::vector<VkSemaphore> image_is_available;
		std::vector<VkSemaphore> rendering_is_finished;
		std::vector<VkFence> in_flight;
		std::vector<VkFence> images_in_flight;
		int32_t frame_index;

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
		//device
		void create_device();
		void get_physical_device();
		//device extensions
		std::vector<const char*> get_required_device_extensions() const;
		std::vector<VkExtensionProperties> get_available_device_extensions(const VkPhysicalDevice physical_device) const;
		bool are_device_extensions_supported(const std::vector<const char*>& required_device_extensions, const std::vector<VkExtensionProperties>& available_device_extensions) const;
		//swap chain
		void register_window_resize_event_handler();
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
		void load_mesh();
		void create_vertex_buffer();
		void create_index_buffer();
		void create_buffer(const size_t size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& buffer_memory);
		void copy_buffer(const VkBuffer source_buffer, const VkBuffer destination_buffer, const size_t size);
		//texture
		void create_texture_image();
		void create_texture_image_view();
		void create_image(const glm::ivec2& size, const uint32_t mipmap_level_count, const VkSampleCountFlagBits sample_count, const VkFormat format, const VkImageTiling image_tiling, const VkImageUsageFlags image_usage, const VkMemoryPropertyFlags properties, VkImage& texture_image, VkDeviceMemory& texture_image_device_memory);
		VkImageView create_image_view(const VkImage image, const VkFormat format, const VkImageAspectFlags image_aspect_flags, const uint32_t mipmap_level_count);
		void create_texture_sampler();
		void transition_image_layout(VkImage image, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout, const uint32_t mipmap_level_count);
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
		void create_command_pool();
		void create_command_buffers();
		VkCommandBuffer begin_single_time_commands();
		void end_single_time_commands(const VkCommandBuffer command_buffer);
		//sync
		void create_semaphores();
	public:
		wm_vulkan_rendering_system();
		void update() override;
		~wm_vulkan_rendering_system();

		bool is_active() const override { return wm_base_system::is_active(); }
		void set_active(const bool active) override { wm_base_system::set_active(active); }
	};

}
