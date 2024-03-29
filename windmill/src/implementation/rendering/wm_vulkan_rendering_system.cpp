#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

#include "component/camera/camera_component.h"
#include "core/engine.h"
#include "core/utility.h"
#include "defines/log_defines.h"
#include "defines/general_defines.h"
#include "window/event/window_framebuffer_size_event.h"
#include "window/event/mouse_scroll_event.h"
#include "window/event/keyboard_character_event.h"

#include "wm_vulkan_rendering_system.h"
#include "../resource/wm_resource_system.h"

namespace wm {

	std::vector<gpu_vertex> wm_vulkan_rendering_system::vertices;
	std::vector<uint32_t> wm_vulkan_rendering_system::indices;

	wm_vulkan_rendering_system::wm_vulkan_rendering_system() {
		create_instance();
	#ifdef WM_BUILD_DEBUG
		create_debug_utils_messenger();
	#endif
		create_surface(&instance, &surface);
		create_device();
		register_event_handlers();
		create_swap_chain();
		create_swap_chain_image_views();
		create_render_pass();
		create_descriptor_set_layout();
		create_pipeline();
		create_command_pool();
		create_color_resources();
		create_depth_resources();
		create_framebuffers();
		create_texture_image();
		create_texture_image_view();
		create_texture_sampler();
		load_mesh();
		create_vertex_buffer();
		create_index_buffer();
		create_uniform_buffers();
		create_descriptor_pool();
		create_descriptor_sets();
		create_command_buffers();
		create_semaphores();
		initialize_imgui();

		WM_LOG_INFO_1("Vulkan rendering system created");
	}

	void wm_vulkan_rendering_system::create_instance() {
		const auto layers = get_layers();
		const auto extensions = get_extensions();

		VkApplicationInfo application_info{};
		application_info.sType = VkStructureType::VK_STRUCTURE_TYPE_APPLICATION_INFO;
		application_info.pEngineName = engine::get_engine_name().c_str();
		application_info.engineVersion = utility::to_vulkan_version(engine::get_engine_version());
		application_info.pApplicationName = engine::get_app_name().c_str();
		application_info.applicationVersion = utility::to_vulkan_version(engine::get_app_version());
		application_info.apiVersion = VK_API_VERSION_1_2;

		VkInstanceCreateInfo instance_create_info{};
		instance_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instance_create_info.pApplicationInfo = &application_info;
		instance_create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		instance_create_info.ppEnabledExtensionNames = extensions.data();
		instance_create_info.enabledLayerCount = static_cast<uint32_t>(layers.size());
		instance_create_info.ppEnabledLayerNames = layers.data();
	#ifdef WM_BUILD_DEBUG
		const auto debug_utils_messenger_create_info = create_debug_utils_messenger_create_info();
		instance_create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debug_utils_messenger_create_info;
	#endif

		WM_ASSERT_VULKAN(vkCreateInstance(&instance_create_info, nullptr, &instance));
		WM_LOG_INFO_2("Vulkan instance created");
	}


	//layers
	std::vector<const char*> wm_vulkan_rendering_system::get_layers() const {
		const auto required_layers = get_required_layers();
	#ifdef WM_BUILD_DEBUG
		const auto available_layers = get_available_layers();
		log_available_layers(available_layers);
		validate_layers(required_layers, available_layers);
	#endif
		return required_layers;
	}

	std::vector<const char*> wm_vulkan_rendering_system::get_required_layers() const {
	#ifdef WM_BUILD_DEBUG
		return std::vector<const char*>(1, "VK_LAYER_KHRONOS_validation");
	#else
		return std::vector<const char*>();
	#endif
	}

	std::vector<VkLayerProperties> wm_vulkan_rendering_system::get_available_layers() const {
		uint32_t layer_count;
		WM_ASSERT_VULKAN(vkEnumerateInstanceLayerProperties(&layer_count, nullptr));
		std::vector<VkLayerProperties> available_layers(layer_count);
		WM_ASSERT_VULKAN(vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data()));
		return available_layers;
	}

	void wm_vulkan_rendering_system::log_available_layers(const std::vector<VkLayerProperties>& available_layers) const {
		std::string result_string = "Available Vulkan layers:\r\n";
		for(uint32_t i = 0; i < available_layers.size(); i++) {
			result_string += std::string("\t") + available_layers.at(i).layerName;
			if(i != available_layers.size() - 1) {
				result_string += "\r\n";
			}
		}
		WM_LOG_INFO_3(result_string);
	}

	void wm_vulkan_rendering_system::validate_layers(const std::vector<const char*>& required_layers, const std::vector<VkLayerProperties>& available_layers) const {
		for(const auto& rl : required_layers) {
			bool layer_found = false;
			for(const auto& al : available_layers) {
				if(std::strcmp(rl, al.layerName) == 0) {
					layer_found = true;
					break;
				}
			}
			if(!layer_found) {
				WM_THROW_ERROR("Layer " + std::string(rl) + " is not available");
			}
		}
	}


	//instance extensions
	std::vector<const char*> wm_vulkan_rendering_system::get_extensions() const {
		const auto required_extensions = get_required_extensions();
	#ifdef WM_BUILD_DEBUG
		const auto available_extensions = get_available_extensions();
		log_available_extensions(available_extensions);
		validate_extensions(required_extensions, available_extensions);
	#endif
		return required_extensions;
	}

	std::vector<const char*> wm_vulkan_rendering_system::get_required_extensions() const {
		auto required_extensions = engine::get_window_system()->get_required_extensions();
	#ifdef WM_BUILD_DEBUG
		required_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	#endif
		return required_extensions;
	}

	std::vector<VkExtensionProperties> wm_vulkan_rendering_system::get_available_extensions() const {
		uint32_t extension_count;
		WM_ASSERT_VULKAN(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr));
		std::vector<VkExtensionProperties> available_extensions(extension_count);
		WM_ASSERT_VULKAN(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, available_extensions.data()));
		return available_extensions;
	}

	void wm_vulkan_rendering_system::log_available_extensions(const std::vector<VkExtensionProperties>& available_extensions) const {
		std::string result_string = "Available Vulkan instance extensions:\r\n";
		for(uint32_t i = 0; i < available_extensions.size(); i++) {
			result_string += std::string("\t") + available_extensions.at(i).extensionName;
			if(i != available_extensions.size() - 1) {
				result_string += "\r\n";
			}
		}
		WM_LOG_INFO_3(result_string);
	}

	void wm_vulkan_rendering_system::validate_extensions(const std::vector<const char*>& required_extensions, const std::vector<VkExtensionProperties>& available_extensions) const {
		for(const auto& re : required_extensions) {
			bool extension_found = false;
			for(const auto& ae : available_extensions) {
				if(std::strcmp(re, ae.extensionName) == 0) {
					extension_found = true;
					break;
				}
			}
			if(!extension_found) {
				WM_THROW_ERROR("Instance extension " + std::string(re) + " is not available");
			}
		}
	}


	//debug utils messenger
	static std::string vulkan_message_severity_to_string(const VkDebugUtilsMessageSeverityFlagBitsEXT severity) {
		switch(severity) {
			case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: return "VERBOSE";
			case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: return "INFO";
			case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: return "WARNING";
			case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: return "ERROR";
			default: return "UNKNOWN MESSAGE SEVERITY";
		}
	}

	static std::string vulkan_message_type_to_string(const VkDebugUtilsMessageTypeFlagsEXT type) {
		switch(type) {
			case VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: return "GENERAL";
			case VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: return "VALIDATION";
			case VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: return "PERFORMANCE";
			default: return "UNKNOWN MESSAGE TYPE";
		}
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL debug_utils_message_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data) {
		std::string severity = vulkan_message_severity_to_string(message_severity);
		std::string type = vulkan_message_type_to_string(message_type);
		if(message_severity <= VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
			WM_LOG_INFO_3("Vulkan info: " + severity + ", " + type + ", " + callback_data->pMessage);
		} else if(message_severity == VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
			WM_LOG_WARNING("Vulkan warning: " + severity + ", " + type + ", " + callback_data->pMessage);
		} else {
			WM_LOG_ERROR("Vulkan error: " + severity + ", " + type + ", " + callback_data->pMessage, __FUNCTION__, __LINE__);
		}
		return VK_FALSE;
	}

	VkDebugUtilsMessengerCreateInfoEXT wm_vulkan_rendering_system::create_debug_utils_messenger_create_info() const {
		VkDebugUtilsMessengerCreateInfoEXT debug_utils_messenger_create_info{};
		debug_utils_messenger_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debug_utils_messenger_create_info.messageSeverity =
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debug_utils_messenger_create_info.messageType =
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debug_utils_messenger_create_info.pfnUserCallback = debug_utils_message_callback;
		return debug_utils_messenger_create_info;
	}

	void wm_vulkan_rendering_system::create_debug_utils_messenger() {
		const auto debug_utils_messenger_create_info = create_debug_utils_messenger_create_info();
		auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		WM_ASSERT(vkCreateDebugUtilsMessengerEXT != nullptr);
		WM_ASSERT_VULKAN(vkCreateDebugUtilsMessengerEXT(instance, &debug_utils_messenger_create_info, nullptr, &debug_utils_messenger));
		WM_LOG_INFO_2("Vulkan debug messenger created");
	}

	void wm_vulkan_rendering_system::destroy_debug_utils_messenger() {
		auto vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		WM_ASSERT(vkDestroyDebugUtilsMessengerEXT != nullptr);
		vkDestroyDebugUtilsMessengerEXT(instance, debug_utils_messenger, nullptr);
		debug_utils_messenger = VK_NULL_HANDLE;
		WM_LOG_INFO_2("Vulkan debug messenger destroyed");
	}


	//device
	void wm_vulkan_rendering_system::create_device() {
		get_physical_device();

		std::vector<VkDeviceQueueCreateInfo> device_queue_create_infos{};
		std::vector<int32_t> queue_family_indices = {graphics_queue_family_index};
		if(graphics_queue_family_index != presentation_queue_family_index) {
			queue_family_indices.push_back(presentation_queue_family_index);
		}
		std::array<float, 1> queue_priorities = {1.0};
		for(int32_t queue_fmaily_index : queue_family_indices) {
			VkDeviceQueueCreateInfo device_graphics_queue_create_info{};
			device_graphics_queue_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			device_graphics_queue_create_info.queueFamilyIndex = queue_fmaily_index;
			device_graphics_queue_create_info.queueCount = 1;
			device_graphics_queue_create_info.pQueuePriorities = queue_priorities.data();
			device_queue_create_infos.push_back(device_graphics_queue_create_info);
		}

		VkPhysicalDeviceFeatures physical_device_features{};
		physical_device_features.samplerAnisotropy = anisotropy ? VK_TRUE : VK_FALSE;

		std::vector<const char*> layers = get_required_layers();
		std::vector<const char*> device_extensions = get_required_device_extensions();

		VkDeviceCreateInfo device_create_info{};
		device_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		device_create_info.queueCreateInfoCount = static_cast<uint32_t>(device_queue_create_infos.size());
		device_create_info.pQueueCreateInfos = device_queue_create_infos.data();
		device_create_info.enabledLayerCount = static_cast<uint32_t>(layers.size());
		device_create_info.ppEnabledLayerNames = layers.data();
		device_create_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
		device_create_info.ppEnabledExtensionNames = device_extensions.data();
		device_create_info.pEnabledFeatures = &physical_device_features;

		WM_ASSERT_VULKAN(vkCreateDevice(physical_device, &device_create_info, nullptr, &device));
		WM_LOG_INFO_2("Vulkan device created");

	#ifdef WM_BUILD_DEBUG
		VkDebugUtilsObjectNameInfoEXT debug_utils_object_name_info{};
		debug_utils_object_name_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
		debug_utils_object_name_info.objectHandle = reinterpret_cast<uint64_t>(device);
		debug_utils_object_name_info.objectType = VkObjectType::VK_OBJECT_TYPE_DEVICE;
		debug_utils_object_name_info.pObjectName = "Logical device";
		auto vkSetDebugUtilsObjectNameEXT = reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>(vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT"));
		vkSetDebugUtilsObjectNameEXT(device, &debug_utils_object_name_info);
	#endif

		vkGetDeviceQueue(device, graphics_queue_family_index, 0, &graphics_queue);
		vkGetDeviceQueue(device, presentation_queue_family_index, 0, &presentation_queue);
	}

	void wm_vulkan_rendering_system::get_physical_device() {
		const auto required_device_extensions = get_required_device_extensions();
		uint32_t physical_device_count;
		WM_ASSERT_VULKAN(vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr));
		WM_ASSERT(physical_device_count > 0);
		std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
		WM_ASSERT_VULKAN(vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices.data()));
		std::string physical_device_name = "";
		for(const auto& pd : physical_devices) {
			VkPhysicalDeviceProperties physical_device_properties;
			VkPhysicalDeviceFeatures physical_device_features;
			VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
			vkGetPhysicalDeviceProperties(pd, &physical_device_properties);
			vkGetPhysicalDeviceFeatures(pd, &physical_device_features);
			vkGetPhysicalDeviceMemoryProperties(pd, &physical_device_memory_properties);

			uint32_t surface_format_count;
			WM_ASSERT_VULKAN(vkGetPhysicalDeviceSurfaceFormatsKHR(pd, surface, &surface_format_count, nullptr));

			uint32_t surface_presentation_mode_count;
			WM_ASSERT_VULKAN(vkGetPhysicalDeviceSurfacePresentModesKHR(pd, surface, &surface_presentation_mode_count, nullptr));

			const auto available_device_extensions = get_available_device_extensions(pd);
			if(!are_device_extensions_supported(required_device_extensions, available_device_extensions)) {
				continue;
			}
			if(surface_format_count == 0 || surface_presentation_mode_count == 0) {
				continue;
			}

			int32_t i = 0;
			int32_t graphics_queue_family_index = -1;
			int32_t presentation_queue_family_index = -1;
			uint32_t queue_family_count;
			VkBool32 presentation_supported = false;
			vkGetPhysicalDeviceQueueFamilyProperties(pd, &queue_family_count, nullptr);
			std::vector<VkQueueFamilyProperties> queue_family_properties(queue_family_count);
			vkGetPhysicalDeviceQueueFamilyProperties(pd, &queue_family_count, queue_family_properties.data());
			for(const auto& qfp : queue_family_properties) {
				if(qfp.queueCount > 0) {
					WM_ASSERT_VULKAN(vkGetPhysicalDeviceSurfaceSupportKHR(pd, i, surface, &presentation_supported));
					if(qfp.queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT && presentation_supported) {
						graphics_queue_family_index = i;
						presentation_queue_family_index = i;
						break;
					}
					if(graphics_queue_family_index == -1 && qfp.queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT) {
						graphics_queue_family_index = i;
					}
					if(presentation_queue_family_index == -1 && presentation_supported) {
						presentation_queue_family_index = i;
					}
				}

				i++;
			}
			if(graphics_queue_family_index != -1 && presentation_queue_family_index != -1) {
				physical_device = pd;
				this->graphics_queue_family_index = graphics_queue_family_index;
				this->presentation_queue_family_index = presentation_queue_family_index;
				this->physical_device_memory_properties = physical_device_memory_properties;
				this->anisotropy = physical_device_features.samplerAnisotropy;
				this->max_anisotropy = physical_device_properties.limits.maxSamplerAnisotropy;
				this->msaa_sample_count = get_msaa_sample_count(physical_device_properties);
				physical_device_name = physical_device_properties.deviceName;
				if(physical_device_properties.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
					break;
				}
			}
		}
		WM_ASSERT(physical_device != VK_NULL_HANDLE);
		WM_LOG_INFO_2("Vulkan physical device selected: " + physical_device_name);
	}


	//device extensions
	std::vector<const char*> wm_vulkan_rendering_system::get_required_device_extensions() const {
		std::vector<const char*> required_extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
		return required_extensions;
	}

	std::vector<VkExtensionProperties> wm_vulkan_rendering_system::get_available_device_extensions(const VkPhysicalDevice physical_device) const {
		uint32_t device_extension_count;
		WM_ASSERT_VULKAN(vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &device_extension_count, nullptr));
		std::vector<VkExtensionProperties> available_device_extensions(device_extension_count);
		WM_ASSERT_VULKAN(vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &device_extension_count, available_device_extensions.data()));
		return available_device_extensions;
	}

	bool wm_vulkan_rendering_system::are_device_extensions_supported(const std::vector<const char*>& required_device_extensions, const std::vector<VkExtensionProperties>& available_device_extensions) const {
		for(const auto& rde : required_device_extensions) {
			bool device_extension_found = false;
			for(const auto& ade : available_device_extensions) {
				if(std::strcmp(rde, ade.extensionName) == 0) {
					device_extension_found = true;
					break;
				}
			}
			if(!device_extension_found) {
				return false;
			}
		}
		return true;
	}


	//swap chain
	void wm_vulkan_rendering_system::register_event_handlers() {
		engine::get_event_system()->add_event_listener<window_framebuffer_size_event>(window_framebuffer_size_event::get_key(), [this](const window_framebuffer_size_event event) {
			this->framebuffer_resized = true;
			this->minimized = event.get_new_size().x == 0 || event.get_new_size().y == 0;
		});
		engine::get_event_system()->add_event_listener<mouse_scroll_event>(mouse_scroll_event::get_key(), [this](const mouse_scroll_event event) {
			this->mouse_scroll = event.get_offset();
		});
		engine::get_event_system()->add_event_listener<keyboard_character_event>(keyboard_character_event::get_key(), [this](const keyboard_character_event event) {
			ImGuiIO& io = ImGui::GetIO();
			io.AddInputCharacter(event.get_utf_32_code_point());
		});
	}

	void wm_vulkan_rendering_system::create_swap_chain() {
		surface_format = get_surface_format();
		auto surface_presentation_mode = get_surface_presentation_mode();
		auto surface_capabilities = get_surface_capabilities();
		swap_chain_extent = get_swap_chain_extent(surface_capabilities);

		uint32_t image_count = surface_capabilities.minImageCount + 1;
		if(surface_capabilities.maxImageCount > 0 && image_count > surface_capabilities.maxImageCount) {
			image_count = surface_capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR swap_chain_create_info{};
		swap_chain_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swap_chain_create_info.surface = surface;
		swap_chain_create_info.minImageCount = image_count;
		swap_chain_create_info.imageFormat = surface_format.format;
		swap_chain_create_info.imageColorSpace = surface_format.colorSpace;
		swap_chain_create_info.imageExtent = swap_chain_extent;
		swap_chain_create_info.imageArrayLayers = 1;
		swap_chain_create_info.imageUsage = VkImageUsageFlagBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swap_chain_create_info.preTransform = surface_capabilities.currentTransform;
		swap_chain_create_info.compositeAlpha = VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swap_chain_create_info.presentMode = surface_presentation_mode;
		swap_chain_create_info.clipped = VK_TRUE;
		//swap_chain_create_info.oldSwapchain = swap_chain;
		if(graphics_queue_family_index != presentation_queue_family_index) {
			std::array<uint32_t, 2> queue_family_indices = {static_cast<uint32_t>(graphics_queue_family_index), static_cast<uint32_t>(presentation_queue_family_index)};

			swap_chain_create_info.imageSharingMode = VkSharingMode::VK_SHARING_MODE_CONCURRENT;
			swap_chain_create_info.queueFamilyIndexCount = static_cast<uint32_t>(queue_family_indices.size());
			swap_chain_create_info.pQueueFamilyIndices = queue_family_indices.data();
		} else {
			swap_chain_create_info.imageSharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
		}

		WM_ASSERT_VULKAN(vkCreateSwapchainKHR(device, &swap_chain_create_info, nullptr, &swap_chain));

		WM_ASSERT_VULKAN(vkGetSwapchainImagesKHR(device, swap_chain, &image_count, nullptr));
		swap_chain_images.resize(image_count);
		WM_ASSERT_VULKAN(vkGetSwapchainImagesKHR(device, swap_chain, &image_count, swap_chain_images.data()));

		WM_LOG_INFO_2("Vulkan swap chain created");
	}

	void wm_vulkan_rendering_system::recreate_swap_chain() {
		framebuffer_resized = false;
		WM_ASSERT_VULKAN(vkDeviceWaitIdle(device));

		cleanup_swap_chain();

		create_swap_chain();
		create_swap_chain_image_views();
		create_render_pass();
		create_color_resources();
		create_depth_resources();
		create_framebuffers();
		create_uniform_buffers();
		create_descriptor_pool();
		create_descriptor_sets();
		create_command_buffers();
	}

	VkSurfaceFormatKHR wm_vulkan_rendering_system::get_surface_format() const {
		uint32_t surface_format_count;
		WM_ASSERT_VULKAN(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &surface_format_count, nullptr));
		std::vector<VkSurfaceFormatKHR> surface_formats(surface_format_count);
		WM_ASSERT_VULKAN(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &surface_format_count, surface_formats.data()));

		if(surface_formats.size() == 1 && surface_formats.at(0).format == VkFormat::VK_FORMAT_UNDEFINED) {
			return {VkFormat::VK_FORMAT_B8G8R8A8_UNORM, VkColorSpaceKHR::VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
		}

		for(const auto& sf : surface_formats) {
			if(sf.format == VkFormat::VK_FORMAT_B8G8R8A8_UNORM && sf.colorSpace == VkColorSpaceKHR::VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return sf;
			}
		}
		return surface_formats.at(0);
	}

	VkPresentModeKHR wm_vulkan_rendering_system::get_surface_presentation_mode() const {
		uint32_t surface_presentation_mode_count;
		WM_ASSERT_VULKAN(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &surface_presentation_mode_count, nullptr));
		std::vector<VkPresentModeKHR> surface_presentation_modes(surface_presentation_mode_count);
		WM_ASSERT_VULKAN(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &surface_presentation_mode_count, surface_presentation_modes.data()));

		for(const auto& spm : surface_presentation_modes) {
			if(spm == VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR) {
				return spm;
			}
		}
		return VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR;
	}

	VkSurfaceCapabilitiesKHR wm_vulkan_rendering_system::get_surface_capabilities() const {
		VkSurfaceCapabilitiesKHR surface_capabilities;
		WM_ASSERT_VULKAN(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &surface_capabilities));
		return surface_capabilities;
	}

	VkExtent2D wm_vulkan_rendering_system::get_swap_chain_extent(const VkSurfaceCapabilitiesKHR surface_capabilities) const {
		if(surface_capabilities.currentExtent.width != UINT_MAX) {
			return surface_capabilities.currentExtent;
		} else {
			const glm::ivec2 framebuffer_size = engine::get_window_system()->get_framebuffer_size();
			VkExtent2D extent;
			extent.width = std::clamp(static_cast<uint32_t>(framebuffer_size.x), surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width);
			extent.height = std::clamp(static_cast<uint32_t>(framebuffer_size.y), surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height);
			return extent;
		}
	}

	void wm_vulkan_rendering_system::create_swap_chain_image_views() {
		swap_chain_image_views.resize(swap_chain_images.size());
		for(int32_t i = 0; i < swap_chain_images.size(); i++) {
			swap_chain_image_views.at(i) = create_image_view(swap_chain_images.at(i), surface_format.format, VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 1);
		}
	}

	void wm_vulkan_rendering_system::cleanup_swap_chain() {
		vkDestroyImageView(device, color_image_view, nullptr);
		vkDestroyImage(device, color_image, nullptr);
		vkFreeMemory(device, color_image_device_memory, nullptr);

		vkDestroyImageView(device, depth_image_view, nullptr);
		vkDestroyImage(device, depth_image, nullptr);
		vkFreeMemory(device, depth_image_device_memory, nullptr);

		for(auto framebuffer : swap_chain_framebuffers) {
			vkDestroyFramebuffer(device, framebuffer, nullptr);
		}
		WM_LOG_INFO_2("Vulkan framebuffers destroyed");
		vkFreeCommandBuffers(device, command_pool, static_cast<uint32_t>(command_buffers.size()), command_buffers.data());
		WM_LOG_INFO_2("Vulkan command buffers destroyed");
		vkDestroyRenderPass(device, render_pass, nullptr);
		WM_LOG_INFO_2("Vulkan renderpass destroyed");

		for(auto swap_chain_image_view : swap_chain_image_views) {
			vkDestroyImageView(device, swap_chain_image_view, nullptr);
		}
		vkDestroySwapchainKHR(device, swap_chain, nullptr);
		swap_chain = VK_NULL_HANDLE;

		for(size_t i = 0; i < swap_chain_images.size(); i++) {
			vkDestroyBuffer(device, uniform_buffers.at(i), nullptr);
			vkFreeMemory(device, uniform_buffers_device_memories.at(i), nullptr);
		}
		WM_LOG_INFO_2("Vulkan uniform buffers destroyed");

		vkDestroyDescriptorPool(device, descriptor_pool, nullptr);

		WM_LOG_INFO_2("Vulkan swap chain destroyed");
	}


	//pipeline
	void wm_vulkan_rendering_system::create_descriptor_set_layout() {
		VkDescriptorSetLayoutBinding uniform_buffer_descriptor_set_layout_binding{};
		uniform_buffer_descriptor_set_layout_binding.binding = 0;
		uniform_buffer_descriptor_set_layout_binding.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uniform_buffer_descriptor_set_layout_binding.descriptorCount = 1;
		uniform_buffer_descriptor_set_layout_binding.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayoutBinding sampler_descriptor_set_layout_binding{};
		sampler_descriptor_set_layout_binding.binding = 1;
		sampler_descriptor_set_layout_binding.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		sampler_descriptor_set_layout_binding.descriptorCount = 1;
		sampler_descriptor_set_layout_binding.pImmutableSamplers = nullptr;
		sampler_descriptor_set_layout_binding.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;

		std::array<VkDescriptorSetLayoutBinding, 2> descriptor_set_layout_bindings = {uniform_buffer_descriptor_set_layout_binding, sampler_descriptor_set_layout_binding};

		VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info{};
		descriptor_set_layout_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptor_set_layout_create_info.bindingCount = static_cast<uint32_t>(descriptor_set_layout_bindings.size());
		descriptor_set_layout_create_info.pBindings = descriptor_set_layout_bindings.data();

		WM_ASSERT_VULKAN(vkCreateDescriptorSetLayout(device, &descriptor_set_layout_create_info, nullptr, &descriptor_set_layout));
	}

	void wm_vulkan_rendering_system::create_pipeline() {
		auto vertex_shader_code = read_file("res/shader/shader.vert.spv");
		WM_LOG_INFO_2("Lambertian vertex shader loaded");
		auto fragment_shader_code = read_file("res/shader/shader.frag.spv");
		WM_LOG_INFO_2("Lambertian fragment shader loaded");
		auto vertex_shader_module = create_shader_module(vertex_shader_code);
		auto fragment_shader_module = create_shader_module(fragment_shader_code);

		VkPipelineShaderStageCreateInfo pipeline_vertex_shader_stage_create_info{};
		pipeline_vertex_shader_stage_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		pipeline_vertex_shader_stage_create_info.stage = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
		pipeline_vertex_shader_stage_create_info.module = vertex_shader_module;
		pipeline_vertex_shader_stage_create_info.pName = "main";

		VkPipelineShaderStageCreateInfo pipeline_fragment_shader_stage_create_info{};
		pipeline_fragment_shader_stage_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		pipeline_fragment_shader_stage_create_info.stage = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
		pipeline_fragment_shader_stage_create_info.module = fragment_shader_module;
		pipeline_fragment_shader_stage_create_info.pName = "main";

		std::array<VkPipelineShaderStageCreateInfo, 2> pipeline_shader_stage_create_infos = {pipeline_vertex_shader_stage_create_info, pipeline_fragment_shader_stage_create_info};

		VkVertexInputBindingDescription binding_description{};
		binding_description.binding = 0;
		binding_description.stride = sizeof(gpu_vertex);
		binding_description.inputRate = VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;
		std::array<VkVertexInputBindingDescription, 1> binding_descriptions = {binding_description};

		VkVertexInputAttributeDescription position_attribute_description;
		position_attribute_description.binding = 0;
		position_attribute_description.location = 0;
		position_attribute_description.format = VkFormat::VK_FORMAT_R32G32B32_SFLOAT;
		position_attribute_description.offset = offsetof(gpu_vertex, position);

		VkVertexInputAttributeDescription texture_coordinates_attribute_description;
		texture_coordinates_attribute_description.binding = 0;
		texture_coordinates_attribute_description.location = 1;
		texture_coordinates_attribute_description.format = VkFormat::VK_FORMAT_R32G32B32_SFLOAT;
		texture_coordinates_attribute_description.offset = offsetof(gpu_vertex, normal);

		VkVertexInputAttributeDescription color_attribute_description;
		color_attribute_description.binding = 0;
		color_attribute_description.location = 2;
		color_attribute_description.format = VkFormat::VK_FORMAT_R32G32_SFLOAT;
		color_attribute_description.offset = offsetof(gpu_vertex, texture_coordinates);

		std::array<VkVertexInputAttributeDescription, 3> attribute_descriptions{position_attribute_description, texture_coordinates_attribute_description, color_attribute_description};

		VkPipelineVertexInputStateCreateInfo pipeline_vertex_input_state_create_info{};
		pipeline_vertex_input_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		pipeline_vertex_input_state_create_info.vertexBindingDescriptionCount = static_cast<uint32_t>(binding_descriptions.size());
		pipeline_vertex_input_state_create_info.pVertexBindingDescriptions = binding_descriptions.data();
		pipeline_vertex_input_state_create_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size());
		pipeline_vertex_input_state_create_info.pVertexAttributeDescriptions = attribute_descriptions.data();

		VkPipelineInputAssemblyStateCreateInfo pipeline_input_assembly_state_create_info{};
		pipeline_input_assembly_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		pipeline_input_assembly_state_create_info.topology = VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		pipeline_input_assembly_state_create_info.primitiveRestartEnable = VK_FALSE;

		VkPipelineViewportStateCreateInfo pipeline_viewport_state_create_info{};
		pipeline_viewport_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		pipeline_viewport_state_create_info.viewportCount = 1;
		pipeline_viewport_state_create_info.scissorCount = 1;

		std::array<VkDynamicState, 2> dynamic_states = {VkDynamicState::VK_DYNAMIC_STATE_VIEWPORT, VkDynamicState::VK_DYNAMIC_STATE_SCISSOR};

		VkPipelineDynamicStateCreateInfo pipeline_dynamic_state_create_info{};
		pipeline_dynamic_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		pipeline_dynamic_state_create_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
		pipeline_dynamic_state_create_info.pDynamicStates = dynamic_states.data();

		VkPipelineRasterizationStateCreateInfo pipeline_rasterization_state_create_info{};
		pipeline_rasterization_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		pipeline_rasterization_state_create_info.depthClampEnable = VK_FALSE;
		pipeline_rasterization_state_create_info.rasterizerDiscardEnable = VK_FALSE;
		pipeline_rasterization_state_create_info.polygonMode = VkPolygonMode::VK_POLYGON_MODE_FILL;
		pipeline_rasterization_state_create_info.lineWidth = 1.0f;
		pipeline_rasterization_state_create_info.cullMode = VkCullModeFlagBits::VK_CULL_MODE_BACK_BIT;
		pipeline_rasterization_state_create_info.frontFace = VkFrontFace::VK_FRONT_FACE_CLOCKWISE;
		pipeline_rasterization_state_create_info.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo pipeline_multisample_state_create_info{};
		pipeline_multisample_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		pipeline_multisample_state_create_info.sampleShadingEnable = VK_FALSE;
		pipeline_multisample_state_create_info.rasterizationSamples = msaa_sample_count;

		VkPipelineColorBlendAttachmentState pipeline_color_blend_attachment_state{};
		pipeline_color_blend_attachment_state.colorWriteMask =
			VkColorComponentFlagBits::VK_COLOR_COMPONENT_R_BIT |
			VkColorComponentFlagBits::VK_COLOR_COMPONENT_G_BIT |
			VkColorComponentFlagBits::VK_COLOR_COMPONENT_B_BIT |
			VkColorComponentFlagBits::VK_COLOR_COMPONENT_A_BIT;
		pipeline_color_blend_attachment_state.blendEnable = VK_FALSE;
		std::array<VkPipelineColorBlendAttachmentState, 1> pipeline_color_blend_attachment_states = {pipeline_color_blend_attachment_state};

		VkPipelineColorBlendStateCreateInfo pipeline_color_blend_state_create_info{};
		pipeline_color_blend_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		pipeline_color_blend_state_create_info.logicOpEnable = VK_FALSE;
		pipeline_color_blend_state_create_info.attachmentCount = static_cast<uint32_t>(pipeline_color_blend_attachment_states.size());
		pipeline_color_blend_state_create_info.pAttachments = pipeline_color_blend_attachment_states.data();

		std::array<VkDescriptorSetLayout, 1> descriptor_set_layouts = {descriptor_set_layout};
		VkPipelineLayoutCreateInfo pipeline_layout_create_info{};
		pipeline_layout_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeline_layout_create_info.setLayoutCount = static_cast<uint32_t>(descriptor_set_layouts.size());
		pipeline_layout_create_info.pSetLayouts = descriptor_set_layouts.data();
		WM_ASSERT_VULKAN(vkCreatePipelineLayout(device, &pipeline_layout_create_info, nullptr, &pipeline_layout));

		VkPipelineDepthStencilStateCreateInfo pipeline_depth_stencil_state_create_info{};
		pipeline_depth_stencil_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		pipeline_depth_stencil_state_create_info.depthTestEnable = VK_TRUE;
		pipeline_depth_stencil_state_create_info.depthWriteEnable = VK_TRUE;
		pipeline_depth_stencil_state_create_info.depthCompareOp = VkCompareOp::VK_COMPARE_OP_LESS;
		pipeline_depth_stencil_state_create_info.depthBoundsTestEnable = VK_FALSE;
		pipeline_depth_stencil_state_create_info.stencilTestEnable = VK_FALSE;

		VkGraphicsPipelineCreateInfo graphics_pipeline_create_info{};
		graphics_pipeline_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		graphics_pipeline_create_info.stageCount = static_cast<uint32_t>(pipeline_shader_stage_create_infos.size());
		graphics_pipeline_create_info.pStages = pipeline_shader_stage_create_infos.data();
		graphics_pipeline_create_info.pVertexInputState = &pipeline_vertex_input_state_create_info;
		graphics_pipeline_create_info.pInputAssemblyState = &pipeline_input_assembly_state_create_info;
		graphics_pipeline_create_info.pViewportState = &pipeline_viewport_state_create_info;
		graphics_pipeline_create_info.pDynamicState = &pipeline_dynamic_state_create_info;
		graphics_pipeline_create_info.pRasterizationState = &pipeline_rasterization_state_create_info;
		graphics_pipeline_create_info.pMultisampleState = &pipeline_multisample_state_create_info;
		graphics_pipeline_create_info.pColorBlendState = &pipeline_color_blend_state_create_info;
		graphics_pipeline_create_info.layout = pipeline_layout;
		graphics_pipeline_create_info.renderPass = render_pass;
		graphics_pipeline_create_info.subpass = 0;
		graphics_pipeline_create_info.pDepthStencilState = &pipeline_depth_stencil_state_create_info;

		std::array<VkGraphicsPipelineCreateInfo, 1> graphics_pipeline_create_infos = {graphics_pipeline_create_info};

		WM_ASSERT_VULKAN(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, static_cast<uint32_t>(graphics_pipeline_create_infos.size()), graphics_pipeline_create_infos.data(), nullptr, &pipeline));

		vkDestroyShaderModule(device, vertex_shader_module, nullptr);
		vkDestroyShaderModule(device, fragment_shader_module, nullptr);

		WM_LOG_INFO_2("Vulkan graphics pipeline created");
	}

	void wm_vulkan_rendering_system::create_render_pass() {
		VkSubpassDependency subpass_dependency{};
		subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		subpass_dependency.dstSubpass = 0;
		subpass_dependency.srcStageMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VkPipelineStageFlagBits::VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		subpass_dependency.srcAccessMask = 0;
		subpass_dependency.dstStageMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VkPipelineStageFlagBits::VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		subpass_dependency.dstAccessMask = VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VkAccessFlagBits::VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		std::array<VkSubpassDependency, 1> subpass_dependencies = {subpass_dependency};

		VkAttachmentDescription color_attachment_description{};
		color_attachment_description.format = surface_format.format;
		color_attachment_description.samples = msaa_sample_count;
		color_attachment_description.loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
		color_attachment_description.storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
		color_attachment_description.stencilLoadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		color_attachment_description.stencilStoreOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
		color_attachment_description.initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
		color_attachment_description.finalLayout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription depth_attachment_description{};
		depth_attachment_description.format = get_depth_format();
		depth_attachment_description.samples = msaa_sample_count;
		depth_attachment_description.loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
		depth_attachment_description.storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depth_attachment_description.stencilLoadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depth_attachment_description.stencilStoreOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depth_attachment_description.initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
		depth_attachment_description.finalLayout = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription color_attachment_resolve_description{};
		color_attachment_resolve_description.format = surface_format.format;
		color_attachment_resolve_description.samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
		color_attachment_resolve_description.loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		color_attachment_resolve_description.storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
		color_attachment_resolve_description.stencilLoadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		color_attachment_resolve_description.stencilStoreOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
		color_attachment_resolve_description.initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
		color_attachment_resolve_description.finalLayout = VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		std::array<VkAttachmentDescription, 3> attachment_descriptions = {color_attachment_description, depth_attachment_description, color_attachment_resolve_description};

		VkAttachmentReference color_attachment_reference{};
		color_attachment_reference.attachment = 0;
		color_attachment_reference.layout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		std::array<VkAttachmentReference, 1> color_attachment_references = {color_attachment_reference};

		VkAttachmentReference depth_attachment_reference{};
		depth_attachment_reference.attachment = 1;
		depth_attachment_reference.layout = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference color_attachment_resolve_reference{};
		color_attachment_resolve_reference.attachment = 2;
		color_attachment_resolve_reference.layout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		std::array<VkAttachmentReference, 1> color_attachment_resolve_references = {color_attachment_resolve_reference};

		VkSubpassDescription subpass_description{};
		subpass_description.pipelineBindPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass_description.colorAttachmentCount = static_cast<uint32_t>(color_attachment_references.size());
		subpass_description.pColorAttachments = color_attachment_references.data();
		subpass_description.pDepthStencilAttachment = &depth_attachment_reference;
		subpass_description.pResolveAttachments = color_attachment_resolve_references.data();
		std::array<VkSubpassDescription, 1> subpass_descriptions = {subpass_description};

		VkRenderPassCreateInfo render_pass_create_info{};
		render_pass_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		render_pass_create_info.attachmentCount = static_cast<uint32_t>(attachment_descriptions.size());
		render_pass_create_info.pAttachments = attachment_descriptions.data();
		render_pass_create_info.subpassCount = static_cast<uint32_t>(subpass_descriptions.size());
		render_pass_create_info.pSubpasses = subpass_descriptions.data();
		render_pass_create_info.dependencyCount = static_cast<uint32_t>(subpass_dependencies.size());
		render_pass_create_info.pDependencies = subpass_dependencies.data();

		WM_ASSERT_VULKAN(vkCreateRenderPass(device, &render_pass_create_info, nullptr, &render_pass));

		WM_LOG_INFO_2("Vulkan renderpass created");
	}

	void wm_vulkan_rendering_system::create_framebuffers() {
		swap_chain_framebuffers.resize(swap_chain_image_views.size());

		for(int32_t i = 0; i < static_cast<int32_t>(swap_chain_image_views.size()); i++) {
			std::array<VkImageView, 3> attachments = {color_image_view, depth_image_view, swap_chain_image_views.at(i)};

			VkFramebufferCreateInfo framebuffer_create_info{};
			framebuffer_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebuffer_create_info.renderPass = render_pass;
			framebuffer_create_info.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebuffer_create_info.pAttachments = attachments.data();
			framebuffer_create_info.width = swap_chain_extent.width;
			framebuffer_create_info.height = swap_chain_extent.height;
			framebuffer_create_info.layers = 1;

			WM_ASSERT_VULKAN(vkCreateFramebuffer(device, &framebuffer_create_info, nullptr, &swap_chain_framebuffers.at(i)));
		}
		WM_LOG_INFO_2("Vulkan framebuffers created");
	}


	//shader
	std::vector<char> wm_vulkan_rendering_system::read_file(const std::string& file_name) const {
		std::ifstream file(file_name, std::ios::ate | std::ios::binary);
		WM_ASSERT(file.is_open());

		int32_t file_size = static_cast<int32_t>(file.tellg());
		std::vector<char> buffer(file_size);
		file.seekg(0);
		file.read(buffer.data(), file_size);
		file.close();

		return buffer;
	}

	VkShaderModule wm_vulkan_rendering_system::create_shader_module(const std::vector<char>& code) const {
		VkShaderModuleCreateInfo shader_module_create_info{};
		shader_module_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shader_module_create_info.codeSize = code.size();
		shader_module_create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shader_module;
		WM_ASSERT_VULKAN(vkCreateShaderModule(device, &shader_module_create_info, nullptr, &shader_module));
		return shader_module;
	}


	//texture
	void wm_vulkan_rendering_system::create_texture_image() {
		auto image = engine::get_resource_system()->get_image("res/mesh/helmet.jpg");
		texture_mipmap_level_count = static_cast<uint32_t>(std::floor(std::log2(std::max(image->get_size().x, image->get_size().y)))) + 1;
		VkDeviceSize size = image->get_size().x * image->get_size().y * 4;

		VkBuffer staging_buffer;
		VkDeviceMemory staging_buffer_device_memory;
		create_buffer(size, VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer, staging_buffer_device_memory);

		void* data;
		WM_ASSERT_VULKAN(vkMapMemory(device, staging_buffer_device_memory, 0, size, 0, &data));
		std::memcpy(data, image->get_pixels(), static_cast<size_t>(size));
		vkUnmapMemory(device, staging_buffer_device_memory);

		create_image(image->get_size(), texture_mipmap_level_count, VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, VkImageTiling::VK_IMAGE_TILING_OPTIMAL, VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, texture_image, texture_image_device_memory);

		transition_image_layout(texture_image, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, texture_mipmap_level_count);
		copy_buffer_to_image(staging_buffer, texture_image, static_cast<uint32_t>(image->get_size().x), static_cast<uint32_t>(image->get_size().y));
		generate_mipmaps(texture_image, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, image->get_size(), texture_mipmap_level_count);

		vkDestroyBuffer(device, staging_buffer, nullptr);
		vkFreeMemory(device, staging_buffer_device_memory, nullptr);
		image.destroy();

		WM_LOG_INFO_2("Vulkan texture created");
	}

	void wm_vulkan_rendering_system::create_texture_image_view() {
		texture_image_view = create_image_view(texture_image, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, texture_mipmap_level_count);
	}

	void wm_vulkan_rendering_system::create_image(const glm::ivec2& size, const uint32_t mipmap_level_count, const VkSampleCountFlagBits sample_count, const VkFormat format, const VkImageTiling image_tiling, const VkImageUsageFlags image_usage, const VkMemoryPropertyFlags properties, VkImage& texture_image, VkDeviceMemory& texture_image_device_memory) {
		VkImageCreateInfo image_create_info{};
		image_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		image_create_info.imageType = VkImageType::VK_IMAGE_TYPE_2D;
		image_create_info.extent.width = static_cast<uint32_t>(size.x);
		image_create_info.extent.height = static_cast<uint32_t>(size.y);
		image_create_info.extent.depth = 1;
		image_create_info.mipLevels = mipmap_level_count;
		image_create_info.arrayLayers = 1;
		image_create_info.format = format;
		image_create_info.tiling = image_tiling;
		image_create_info.initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
		image_create_info.usage = image_usage;
		image_create_info.sharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
		image_create_info.samples = sample_count;

		WM_ASSERT_VULKAN(vkCreateImage(device, &image_create_info, nullptr, &texture_image));

		VkMemoryRequirements memory_requirements;
		vkGetImageMemoryRequirements(device, texture_image, &memory_requirements);

		int32_t memory_type_index = -1;
		for(uint32_t i = 0; i < physical_device_memory_properties.memoryTypeCount; i++) {
			if(memory_requirements.memoryTypeBits & (1 << i) && (physical_device_memory_properties.memoryTypes[i].propertyFlags & properties) == properties) {
				memory_type_index = i;
				break;
			}
		}
		WM_ASSERT(memory_type_index != -1);

		VkMemoryAllocateInfo memory_allocation_info{};
		memory_allocation_info.sType = VkStructureType::VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memory_allocation_info.allocationSize = memory_requirements.size;
		memory_allocation_info.memoryTypeIndex = memory_type_index;

		WM_ASSERT_VULKAN(vkAllocateMemory(device, &memory_allocation_info, nullptr, &texture_image_device_memory));

		WM_ASSERT_VULKAN(vkBindImageMemory(device, texture_image, texture_image_device_memory, 0));
	}

	VkImageView wm_vulkan_rendering_system::create_image_view(const VkImage image, const VkFormat format, const VkImageAspectFlags image_aspect_flags, const uint32_t mipmap_level_count) {
		VkImageViewCreateInfo image_view_create_info{};
		image_view_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		image_view_create_info.image = image;
		image_view_create_info.viewType = VkImageViewType::VK_IMAGE_VIEW_TYPE_2D;
		image_view_create_info.format = format;
		image_view_create_info.subresourceRange.aspectMask = image_aspect_flags;
		image_view_create_info.subresourceRange.baseMipLevel = 0;
		image_view_create_info.subresourceRange.levelCount = mipmap_level_count;
		image_view_create_info.subresourceRange.baseArrayLayer = 0;
		image_view_create_info.subresourceRange.layerCount = 1;

		VkImageView image_view;
		WM_ASSERT_VULKAN(vkCreateImageView(device, &image_view_create_info, nullptr, &image_view));

		return image_view;
	}

	void wm_vulkan_rendering_system::create_texture_sampler() {
		create_sampler(texture_mipmap_level_count, max_anisotropy, texture_sampler);
	}

	void wm_vulkan_rendering_system::create_sampler(const uint32_t mipmap_level_count, const float max_anisotropy, VkSampler& sampler) {
		VkSamplerCreateInfo sampler_create_info{};
		sampler_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		sampler_create_info.magFilter = VkFilter::VK_FILTER_LINEAR;
		sampler_create_info.minFilter = VkFilter::VK_FILTER_LINEAR;
		sampler_create_info.addressModeU = VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler_create_info.addressModeV = VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler_create_info.addressModeW = VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler_create_info.anisotropyEnable = max_anisotropy != 0.0f;
		sampler_create_info.maxAnisotropy = max_anisotropy;
		sampler_create_info.borderColor = VkBorderColor::VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		sampler_create_info.unnormalizedCoordinates = VK_FALSE;
		sampler_create_info.compareEnable = VK_FALSE;//TODO: PCF
		sampler_create_info.compareOp = VkCompareOp::VK_COMPARE_OP_ALWAYS;
		sampler_create_info.mipmapMode = VkSamplerMipmapMode::VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler_create_info.minLod = 0.0f;
		sampler_create_info.maxLod = static_cast<float>(mipmap_level_count);

		WM_ASSERT_VULKAN(vkCreateSampler(device, &sampler_create_info, nullptr, &sampler));
	}

	void wm_vulkan_rendering_system::transition_image_layout(VkImage image, VkImageLayout old_layout, VkImageLayout new_layout, const uint32_t mipmap_level_count) {
		VkCommandBuffer command_buffer = begin_single_time_commands();

		VkPipelineStageFlags source_stage;
		VkPipelineStageFlags destination_stage;

		VkImageMemoryBarrier image_memory_barrier{};
		image_memory_barrier.sType = VkStructureType::VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		image_memory_barrier.oldLayout = old_layout;
		image_memory_barrier.newLayout = new_layout;
		image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		image_memory_barrier.image = image;
		image_memory_barrier.subresourceRange.aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
		image_memory_barrier.subresourceRange.baseMipLevel = 0;
		image_memory_barrier.subresourceRange.levelCount = mipmap_level_count;
		image_memory_barrier.subresourceRange.baseArrayLayer = 0;
		image_memory_barrier.subresourceRange.layerCount = 1;
		if(old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			image_memory_barrier.srcAccessMask = 0;
			image_memory_barrier.dstAccessMask = VkAccessFlagBits::VK_ACCESS_TRANSFER_WRITE_BIT;
			source_stage = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destination_stage = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT;
		} else if(old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			image_memory_barrier.srcAccessMask = VkAccessFlagBits::VK_ACCESS_TRANSFER_WRITE_BIT;
			image_memory_barrier.dstAccessMask = VkAccessFlagBits::VK_ACCESS_SHADER_READ_BIT;
			source_stage = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT;
			destination_stage = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		} else {
			WM_THROW_ERROR("unsupported layout transition");
		}

		std::array<VkImageMemoryBarrier, 1> image_memory_barriers = {image_memory_barrier};

		vkCmdPipelineBarrier(command_buffer, source_stage, destination_stage, 0, 0, nullptr, 0, nullptr, static_cast<uint32_t>(image_memory_barriers.size()), image_memory_barriers.data());

		end_single_time_commands(command_buffer);
	}

	void wm_vulkan_rendering_system::copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
		VkCommandBuffer command_buffer = begin_single_time_commands();

		VkBufferImageCopy buffer_image_copy{};
		buffer_image_copy.bufferOffset = 0;
		buffer_image_copy.bufferRowLength = 0;
		buffer_image_copy.bufferImageHeight = 0;
		buffer_image_copy.imageSubresource.aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
		buffer_image_copy.imageSubresource.mipLevel = 0;
		buffer_image_copy.imageSubresource.baseArrayLayer = 0;
		buffer_image_copy.imageSubresource.layerCount = 1;
		buffer_image_copy.imageOffset = {0, 0, 0};
		buffer_image_copy.imageExtent = {width, height, 1};

		std::array<VkBufferImageCopy, 1> buffer_image_copies = {buffer_image_copy};

		vkCmdCopyBufferToImage(command_buffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, static_cast<uint32_t>(buffer_image_copies.size()), buffer_image_copies.data());

		end_single_time_commands(command_buffer);
	}

	void wm_vulkan_rendering_system::generate_mipmaps(VkImage image, const VkFormat format, const glm::vec2& size, const uint32_t mipmap_level_count) {
		VkFormatProperties format_properties;
		vkGetPhysicalDeviceFormatProperties(physical_device, format, &format_properties);
		WM_ASSERT((format_properties.optimalTilingFeatures & VkFormatFeatureFlagBits::VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT) == VkFormatFeatureFlagBits::VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT);

		VkCommandBuffer command_buffer = begin_single_time_commands();

		VkImageMemoryBarrier image_memory_barrier{};
		image_memory_barrier.sType = VkStructureType::VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		image_memory_barrier.image = image;
		image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		image_memory_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_memory_barrier.subresourceRange.baseArrayLayer = 0;
		image_memory_barrier.subresourceRange.layerCount = 1;
		image_memory_barrier.subresourceRange.levelCount = 1;

		glm::ivec2 mipmap_size = size;

		for(uint32_t i = 1; i < mipmap_level_count; i++) {
			image_memory_barrier.subresourceRange.baseMipLevel = i - 1;
			image_memory_barrier.oldLayout = VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			image_memory_barrier.newLayout = VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			image_memory_barrier.srcAccessMask = VkAccessFlagBits::VK_ACCESS_TRANSFER_WRITE_BIT;
			image_memory_barrier.dstAccessMask = VkAccessFlagBits::VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(command_buffer, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);

			VkImageBlit image_blit{};
			image_blit.srcOffsets[0] = {0, 0, 0};
			image_blit.srcOffsets[1] = {mipmap_size.x, mipmap_size.y, 1};
			image_blit.srcSubresource.aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
			image_blit.srcSubresource.mipLevel = i - 1;
			image_blit.srcSubresource.baseArrayLayer = 0;
			image_blit.srcSubresource.layerCount = 1;
			image_blit.dstOffsets[0] = {0, 0, 0};
			image_blit.dstOffsets[1] = {mipmap_size.x > 1 ? mipmap_size.x / 2 : 1, mipmap_size.y > 1 ? mipmap_size.y / 2 : 1, 1};
			image_blit.dstSubresource.aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
			image_blit.dstSubresource.mipLevel = i;
			image_blit.dstSubresource.baseArrayLayer = 0;
			image_blit.dstSubresource.layerCount = 1;

			vkCmdBlitImage(command_buffer, image, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &image_blit, VkFilter::VK_FILTER_LINEAR);

			image_memory_barrier.oldLayout = VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			image_memory_barrier.newLayout = VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			image_memory_barrier.srcAccessMask = VkAccessFlagBits::VK_ACCESS_TRANSFER_READ_BIT;
			image_memory_barrier.dstAccessMask = VkAccessFlagBits::VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(command_buffer, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);

			mipmap_size.x = std::max(mipmap_size.x / 2.0f, 1.0f);
			mipmap_size.y = std::max(mipmap_size.y / 2.0f, 1.0f);
		}

		image_memory_barrier.subresourceRange.baseMipLevel = mipmap_level_count - 1;
		image_memory_barrier.oldLayout = VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		image_memory_barrier.newLayout = VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		image_memory_barrier.srcAccessMask = VkAccessFlagBits::VK_ACCESS_TRANSFER_WRITE_BIT;
		image_memory_barrier.dstAccessMask = VkAccessFlagBits::VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(command_buffer, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);

		end_single_time_commands(command_buffer);
	}


	//color texture
	void wm_vulkan_rendering_system::create_color_resources() {
		VkFormat format = surface_format.format;
		create_image(glm::vec2(swap_chain_extent.width, swap_chain_extent.height), 1, msaa_sample_count, format, VkImageTiling::VK_IMAGE_TILING_OPTIMAL, VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, color_image, color_image_device_memory);
		color_image_view = create_image_view(color_image, format, VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}

	VkSampleCountFlagBits wm_vulkan_rendering_system::get_msaa_sample_count(const VkPhysicalDeviceProperties physical_device_properties) const {
		VkSampleCountFlags counts = physical_device_properties.limits.framebufferColorSampleCounts & physical_device_properties.limits.framebufferDepthSampleCounts;
		if(counts & VkSampleCountFlagBits::VK_SAMPLE_COUNT_64_BIT) {
			return VkSampleCountFlagBits::VK_SAMPLE_COUNT_64_BIT;
		} else if(counts & VkSampleCountFlagBits::VK_SAMPLE_COUNT_32_BIT) {
			return VkSampleCountFlagBits::VK_SAMPLE_COUNT_32_BIT;
		} else if(counts & VkSampleCountFlagBits::VK_SAMPLE_COUNT_16_BIT) {
			return VkSampleCountFlagBits::VK_SAMPLE_COUNT_16_BIT;
		} else if(counts & VkSampleCountFlagBits::VK_SAMPLE_COUNT_8_BIT) {
			return VkSampleCountFlagBits::VK_SAMPLE_COUNT_8_BIT;
		} else if(counts & VkSampleCountFlagBits::VK_SAMPLE_COUNT_4_BIT) {
			return VkSampleCountFlagBits::VK_SAMPLE_COUNT_4_BIT;
		} else if(counts & VkSampleCountFlagBits::VK_SAMPLE_COUNT_2_BIT) {
			return VkSampleCountFlagBits::VK_SAMPLE_COUNT_2_BIT;
		}

		return VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
	}


	//depth texture
	void wm_vulkan_rendering_system::create_depth_resources() {
		VkFormat depth_format = get_depth_format();
		create_image(glm::vec2(swap_chain_extent.width, swap_chain_extent.height), 1, msaa_sample_count, depth_format, VkImageTiling::VK_IMAGE_TILING_OPTIMAL, VkImageUsageFlagBits::VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depth_image, depth_image_device_memory);
		depth_image_view = create_image_view(depth_image, depth_format, VkImageAspectFlagBits::VK_IMAGE_ASPECT_DEPTH_BIT, 1);
	}

	VkFormat wm_vulkan_rendering_system::get_depth_format() const {
		return get_depth_format({VkFormat::VK_FORMAT_D32_SFLOAT, VkFormat::VK_FORMAT_D32_SFLOAT_S8_UINT, VkFormat::VK_FORMAT_D24_UNORM_S8_UINT}, VkImageTiling::VK_IMAGE_TILING_OPTIMAL, VkFormatFeatureFlagBits::VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	}

	VkFormat wm_vulkan_rendering_system::get_depth_format(const std::vector<VkFormat>& formats, const VkImageTiling image_tiling, const VkFormatFeatureFlags format_features) const {
		for(VkFormat format : formats) {
			VkFormatProperties format_properties;
			vkGetPhysicalDeviceFormatProperties(physical_device, format, &format_properties);
			if(image_tiling == VkImageTiling::VK_IMAGE_TILING_LINEAR && (format_properties.linearTilingFeatures & format_features) == format_features) {
				return format;
			} else if(image_tiling == VkImageTiling::VK_IMAGE_TILING_OPTIMAL && (format_properties.optimalTilingFeatures & format_features) == format_features) {
				return format;
			}
		}
		WM_THROW_ERROR("Can't find appropirate depth format");
	}

	bool wm_vulkan_rendering_system::depth_has_stencil_component(const VkFormat format) const {
		return format == VkFormat::VK_FORMAT_D32_SFLOAT_S8_UINT || format == VkFormat::VK_FORMAT_D24_UNORM_S8_UINT;
	}


	//vertex buffer
	void wm_vulkan_rendering_system::load_mesh() {
		auto mesh = engine::get_resource_system()->get_mesh("res/mesh/helmet.obj");
		for(auto& v : mesh->get_vertices()) {
			vertices.push_back({v.get_position(), v.get_normal(), v.get_texture_coordinate()});
		}
		indices = mesh->get_indices();
	}

	void wm_vulkan_rendering_system::create_vertex_buffer() {
		const size_t size = sizeof(vertices.at(0)) * vertices.size();

		VkBuffer staging_buffer;
		VkDeviceMemory staging_buffer_device_memory;
		create_buffer(size, VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer, staging_buffer_device_memory);

		void* data;
		WM_ASSERT_VULKAN(vkMapMemory(device, staging_buffer_device_memory, 0, size, 0, &data));
		std::memcpy(data, vertices.data(), size);
		vkUnmapMemory(device, staging_buffer_device_memory);

		create_buffer(size, VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_DST_BIT | VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertex_buffer, vertex_buffer_device_memory);
		copy_buffer(staging_buffer, vertex_buffer, size);

		vkDestroyBuffer(device, staging_buffer, nullptr);
		vkFreeMemory(device, staging_buffer_device_memory, nullptr);

		WM_LOG_INFO_2("Vulkan vertex buffer created");
	}

	void wm_vulkan_rendering_system::create_index_buffer() {
		const size_t size = sizeof(indices.at(0)) * indices.size();

		VkBuffer staging_buffer;
		VkDeviceMemory staging_buffer_device_memory;
		create_buffer(size, VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer, staging_buffer_device_memory);

		void* data;
		WM_ASSERT_VULKAN(vkMapMemory(device, staging_buffer_device_memory, 0, size, 0, &data));
		std::memcpy(data, indices.data(), size);
		vkUnmapMemory(device, staging_buffer_device_memory);

		create_buffer(size, VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_DST_BIT | VkBufferUsageFlagBits::VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, index_buffer, index_buffer_device_memory);
		copy_buffer(staging_buffer, index_buffer, size);

		vkDestroyBuffer(device, staging_buffer, nullptr);
		vkFreeMemory(device, staging_buffer_device_memory, nullptr);

		WM_LOG_INFO_2("Vulkan index buffer created");
	}

	void wm_vulkan_rendering_system::create_buffer(const size_t size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& device_memory) const {
		VkBufferCreateInfo buffer_create_info{};
		buffer_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buffer_create_info.size = size;
		buffer_create_info.usage = usage;
		buffer_create_info.sharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;

		WM_ASSERT_VULKAN(vkCreateBuffer(device, &buffer_create_info, nullptr, &buffer));

		VkMemoryRequirements memory_requirements;
		vkGetBufferMemoryRequirements(device, buffer, &memory_requirements);

		int32_t memory_type_index = -1;
		for(uint32_t i = 0; i < physical_device_memory_properties.memoryTypeCount; i++) {
			if(memory_requirements.memoryTypeBits & (1 << i) && (physical_device_memory_properties.memoryTypes[i].propertyFlags & properties) == properties) {
				memory_type_index = i;
				break;
			}
		}
		WM_ASSERT(memory_type_index != -1);

		VkMemoryAllocateInfo memory_allocation_info{};
		memory_allocation_info.sType = VkStructureType::VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memory_allocation_info.allocationSize = memory_requirements.size;
		memory_allocation_info.memoryTypeIndex = memory_type_index;

		WM_ASSERT_VULKAN(vkAllocateMemory(device, &memory_allocation_info, nullptr, &device_memory));

		WM_ASSERT_VULKAN(vkBindBufferMemory(device, buffer, device_memory, 0));
	}

	void wm_vulkan_rendering_system::copy_buffer(const VkBuffer source_buffer, const VkBuffer destination_buffer, const size_t size) {
		VkCommandBuffer command_buffer = begin_single_time_commands();

		VkBufferCopy buffer_copy{};
		buffer_copy.size = size;
		vkCmdCopyBuffer(command_buffer, source_buffer, destination_buffer, 1, &buffer_copy);

		end_single_time_commands(command_buffer);
	}


	//uniform buffers
	void wm_vulkan_rendering_system::create_uniform_buffers() {
		size_t buffer_size = sizeof(uniform_buffer_object);

		uniform_buffers.resize(swap_chain_images.size());
		uniform_buffers_device_memories.resize(swap_chain_images.size());

		for(size_t i = 0; i < swap_chain_images.size(); i++) {
			create_buffer(
				buffer_size,
				VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				uniform_buffers.at(i),
				uniform_buffers_device_memories.at(i)
			);
		}
		WM_LOG_INFO_2("Vulkan uniform buffers created");
	}

	void wm_vulkan_rendering_system::update_uniform_buffer(const uint32_t image_index) const {
		static const float ROTATION_SPEED = 0.05f;
		static float rotation = 0.0f;
		const float delta_time = engine::get_time_system()->get_delta_time();
		rotation += delta_time * ROTATION_SPEED;

		auto parent = engine::get_scene_system()->get_node_by_tag("parent");
		if(parent.is_valid()) {
			parent->get_transform()->set_relative_rotation(glm::vec3(0.0f, 1.0f, 0.0f), rotation);
		}
		glm::mat4 model_matrix = glm::mat4(1.0f);
		glm::mat3 inverse_model_matrix = glm::mat3(1.0f);
		auto child = engine::get_scene_system()->get_node_by_tag("child");
		if(child.is_valid()) {
			child->get_transform()->set_absolute_rotation(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
			model_matrix = child->get_transform()->get_model_matrix();
			inverse_model_matrix = child->get_transform()->get_inverse_model_matrix();
		}
		uniform_buffer_object ubo{};
		ubo.model = model_matrix;
		ubo.inverse_model = inverse_model_matrix;

		auto camera = engine::get_scene_system()->get_node_by_tag("camera")->get_component(camera_component::get_key());
		ubo.view = camera->get_view_matrix();
		ubo.projection = camera->get_projection_matrix();

		void* data;
		WM_ASSERT_VULKAN(vkMapMemory(device, uniform_buffers_device_memories.at(image_index), 0, sizeof(ubo), 0, &data));
		std::memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, uniform_buffers_device_memories.at(image_index));
	}

	void wm_vulkan_rendering_system::create_descriptor_pool() {
		VkDescriptorPoolSize uniform_buffer_descriptor_pool_size{};
		uniform_buffer_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uniform_buffer_descriptor_pool_size.descriptorCount = static_cast<uint32_t>(swap_chain_images.size());

		VkDescriptorPoolSize sampler_descriptor_pool_size{};
		sampler_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		sampler_descriptor_pool_size.descriptorCount = static_cast<uint32_t>(swap_chain_images.size());

		std::array<VkDescriptorPoolSize, 2> descriptor_pool_sizes = {uniform_buffer_descriptor_pool_size, sampler_descriptor_pool_size};

		VkDescriptorPoolCreateInfo descriptor_pool_create_info{};
		descriptor_pool_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptor_pool_create_info.poolSizeCount = static_cast<uint32_t>(descriptor_pool_sizes.size());
		descriptor_pool_create_info.pPoolSizes = descriptor_pool_sizes.data();
		descriptor_pool_create_info.maxSets = static_cast<uint32_t>(swap_chain_images.size());

		WM_ASSERT_VULKAN(vkCreateDescriptorPool(device, &descriptor_pool_create_info, nullptr, &descriptor_pool));
	}

	void wm_vulkan_rendering_system::create_descriptor_sets() {
		std::vector<VkDescriptorSetLayout> descriptor_set_layouts(swap_chain_images.size(), descriptor_set_layout);
		VkDescriptorSetAllocateInfo descriptor_set_allocation_info{};
		descriptor_set_allocation_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptor_set_allocation_info.descriptorPool = descriptor_pool;
		descriptor_set_allocation_info.descriptorSetCount = static_cast<uint32_t>(descriptor_set_layouts.size());
		descriptor_set_allocation_info.pSetLayouts = descriptor_set_layouts.data();

		descriptor_sets.resize(swap_chain_images.size());
		WM_ASSERT_VULKAN(vkAllocateDescriptorSets(device, &descriptor_set_allocation_info, descriptor_sets.data()));

		for(size_t i = 0; i < swap_chain_images.size(); i++) {
			VkDescriptorBufferInfo descriptor_buffer_info{};
			descriptor_buffer_info.buffer = uniform_buffers.at(i);
			descriptor_buffer_info.offset = 0;
			descriptor_buffer_info.range = sizeof(uniform_buffer_object);
			std::array<VkDescriptorBufferInfo, 1> descriptor_buffer_infos = {descriptor_buffer_info};

			VkDescriptorImageInfo descriptor_image_info{};
			descriptor_image_info.imageLayout = VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			descriptor_image_info.imageView = texture_image_view;
			descriptor_image_info.sampler = texture_sampler;
			std::array<VkDescriptorImageInfo, 1> descriptor_image_infos = {descriptor_image_info};

			VkWriteDescriptorSet uniform_buffer_write_descriptor_set{};
			uniform_buffer_write_descriptor_set.sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			uniform_buffer_write_descriptor_set.dstSet = descriptor_sets.at(i);
			uniform_buffer_write_descriptor_set.dstBinding = 0;
			uniform_buffer_write_descriptor_set.dstArrayElement = 0;
			uniform_buffer_write_descriptor_set.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			uniform_buffer_write_descriptor_set.descriptorCount = static_cast<uint32_t>(descriptor_buffer_infos.size());
			uniform_buffer_write_descriptor_set.pBufferInfo = descriptor_buffer_infos.data();

			VkWriteDescriptorSet sampler_write_descriptor_set{};
			sampler_write_descriptor_set.sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			sampler_write_descriptor_set.dstSet = descriptor_sets.at(i);
			sampler_write_descriptor_set.dstBinding = 1;
			sampler_write_descriptor_set.dstArrayElement = 0;
			sampler_write_descriptor_set.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			sampler_write_descriptor_set.descriptorCount = static_cast<uint32_t>(descriptor_image_infos.size());
			sampler_write_descriptor_set.pImageInfo = descriptor_image_infos.data();

			std::array<VkWriteDescriptorSet, 2> write_descriptor_sets = {uniform_buffer_write_descriptor_set, sampler_write_descriptor_set};

			vkUpdateDescriptorSets(device, static_cast<uint32_t>(write_descriptor_sets.size()), write_descriptor_sets.data(), 0, nullptr);
		}
	}


	//command buffers
	void wm_vulkan_rendering_system::create_command_pool() {
		VkCommandPoolCreateInfo command_pool_create_info{};
		command_pool_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		command_pool_create_info.queueFamilyIndex = graphics_queue_family_index;
		command_pool_create_info.flags = VkCommandPoolCreateFlagBits::VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		WM_ASSERT_VULKAN(vkCreateCommandPool(device, &command_pool_create_info, nullptr, &command_pool));
	}

	void wm_vulkan_rendering_system::create_command_buffers() {
		command_buffers.resize(swap_chain_framebuffers.size());
		VkCommandBufferAllocateInfo command_buffer_allocation_info{};
		command_buffer_allocation_info.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		command_buffer_allocation_info.commandPool = command_pool;
		command_buffer_allocation_info.level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		command_buffer_allocation_info.commandBufferCount = static_cast<uint32_t>(command_buffers.size());

		WM_ASSERT_VULKAN(vkAllocateCommandBuffers(device, &command_buffer_allocation_info, command_buffers.data()));

		WM_LOG_INFO_2("Vulkan command buffers allocated");
	}

	VkCommandBuffer wm_vulkan_rendering_system::begin_single_time_commands() {
		VkCommandBufferAllocateInfo command_buffer_allocate_info{};
		command_buffer_allocate_info.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		command_buffer_allocate_info.level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		command_buffer_allocate_info.commandPool = command_pool;
		command_buffer_allocate_info.commandBufferCount = 1;

		VkCommandBuffer command_buffer;
		WM_ASSERT_VULKAN(vkAllocateCommandBuffers(device, &command_buffer_allocate_info, &command_buffer));

		VkCommandBufferBeginInfo command_buffer_begin_info{};
		command_buffer_begin_info.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		command_buffer_begin_info.flags = VkCommandBufferUsageFlagBits::VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		WM_ASSERT_VULKAN(vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info));

		return command_buffer;
	}

	void wm_vulkan_rendering_system::end_single_time_commands(const VkCommandBuffer command_buffer) {
		WM_ASSERT_VULKAN(vkEndCommandBuffer(command_buffer));

		VkSubmitInfo submit_info{};
		submit_info.sType = VkStructureType::VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &command_buffer;

		std::array<VkCommandBuffer, 1> command_buffers = {command_buffer};

		WM_ASSERT_VULKAN(vkQueueSubmit(graphics_queue, 1, &submit_info, VK_NULL_HANDLE));
		WM_ASSERT_VULKAN(vkQueueWaitIdle(graphics_queue));

		vkFreeCommandBuffers(device, command_pool, static_cast<uint32_t>(command_buffers.size()), command_buffers.data());
	}

	void wm_vulkan_rendering_system::update_command_buffer(const uint32_t image_index) {
		vkResetCommandBuffer(command_buffers.at(image_index), VkCommandBufferResetFlagBits::VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

		VkCommandBufferBeginInfo command_buffer_begin_info{};
		command_buffer_begin_info.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		before_draw_imgui(image_index);

		WM_ASSERT_VULKAN(vkBeginCommandBuffer(command_buffers.at(image_index), &command_buffer_begin_info));

		VkClearValue color_clear_value = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
		VkClearValue depth_clear_value = {{{1.0f, 0.0f}}};
		std::array<VkClearValue, 2> clear_values = {color_clear_value, depth_clear_value};

		VkRenderPassBeginInfo render_pass_begin_info{};
		render_pass_begin_info.sType = VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_begin_info.renderPass = render_pass;
		render_pass_begin_info.framebuffer = swap_chain_framebuffers.at(image_index);
		render_pass_begin_info.renderArea.offset = {0, 0};
		render_pass_begin_info.renderArea.extent = swap_chain_extent;
		render_pass_begin_info.clearValueCount = static_cast<uint32_t>(clear_values.size());
		render_pass_begin_info.pClearValues = clear_values.data();

		vkCmdBeginRenderPass(command_buffers.at(image_index), &render_pass_begin_info, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(command_buffers.at(image_index), VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = static_cast<float>(swap_chain_extent.height);
		viewport.width = static_cast<float>(swap_chain_extent.width);
		viewport.height = -static_cast<float>(swap_chain_extent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		vkCmdSetViewport(command_buffers.at(image_index), 0, 1, &viewport);

		VkRect2D scissor_rectangle{};
		scissor_rectangle.offset = {0, 0};
		scissor_rectangle.extent = swap_chain_extent;

		vkCmdSetScissor(command_buffers.at(image_index), 0, 1, &scissor_rectangle);

		std::array<VkBuffer, 1> vertex_buffers = {vertex_buffer};
		std::array<VkDeviceSize, 1> offsets = {0};
		vkCmdBindVertexBuffers(command_buffers.at(image_index), 0, 1, vertex_buffers.data(), offsets.data());
		vkCmdBindIndexBuffer(command_buffers.at(image_index), index_buffer, 0, VkIndexType::VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(command_buffers.at(image_index), VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout, 0, 1, &descriptor_sets.at(image_index), 0, nullptr);

		vkCmdDrawIndexed(command_buffers.at(image_index), static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

		draw_imgui(image_index);

		vkCmdEndRenderPass(command_buffers.at(image_index));

		WM_ASSERT_VULKAN(vkEndCommandBuffer(command_buffers.at(image_index)));
	}


	//sync
	void wm_vulkan_rendering_system::create_semaphores() {
		image_is_available.resize(MAX_FRAMES_IN_FLIGHT);
		rendering_is_finished.resize(MAX_FRAMES_IN_FLIGHT);
		in_flight.resize(MAX_FRAMES_IN_FLIGHT);
		images_in_flight.resize(swap_chain_images.size(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphore_create_info{};
		semaphore_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fence_create_info{};
		fence_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fence_create_info.flags = VkFenceCreateFlagBits::VK_FENCE_CREATE_SIGNALED_BIT;

		for(int32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			WM_ASSERT_VULKAN(vkCreateSemaphore(device, &semaphore_create_info, nullptr, &image_is_available.at(i)));
			WM_ASSERT_VULKAN(vkCreateSemaphore(device, &semaphore_create_info, nullptr, &rendering_is_finished.at(i)));
			WM_ASSERT_VULKAN(vkCreateFence(device, &fence_create_info, nullptr, &in_flight.at(i)));
		}
	}


	//imgui
	void wm_vulkan_rendering_system::initialize_imgui() {
		create_imgui();
		create_imgui_font_image();
		create_imgui_descriptor_sets();
		create_imgui_pipeline();
	}

	void wm_vulkan_rendering_system::create_imgui() {
		ImGui::CreateContext();

		//ImGui::StyleColorsLight();
		//ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_::ImGuiConfigFlags_NavEnableGamepad;

		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;

		const int32_t INITIAL_VERTEX_COUNT = 512;
		for(int32_t i = 0; i < swap_chain_images.size(); i++) {
			imgui_vertex_buffers.push_back(VK_NULL_HANDLE);
			imgui_vertex_buffer_device_memories.push_back(VK_NULL_HANDLE);
			imgui_vertex_count.push_back(INITIAL_VERTEX_COUNT);
			create_buffer(INITIAL_VERTEX_COUNT * sizeof(ImDrawVert), VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, imgui_vertex_buffers.at(i), imgui_vertex_buffer_device_memories.at(i));

			imgui_index_buffers.push_back(VK_NULL_HANDLE);
			imgui_index_buffer_device_memories.push_back(VK_NULL_HANDLE);
			imgui_index_count.push_back(0);
			create_buffer(INITIAL_VERTEX_COUNT * sizeof(ImDrawIdx), VkBufferUsageFlagBits::VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, imgui_index_buffers.at(i), imgui_index_buffer_device_memories.at(i));
		}
	}

	void wm_vulkan_rendering_system::create_imgui_font_image() {
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(swap_chain_extent.width, swap_chain_extent.height);
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

		unsigned char* font_data;
		int32_t font_texture_width;
		int32_t font_texture_height;
		io.Fonts->GetTexDataAsRGBA32(&font_data, &font_texture_width, &font_texture_height);
		VkDeviceSize size = font_texture_width * font_texture_height * 4 * sizeof(char);

		VkBuffer staging_buffer;
		VkDeviceMemory staging_buffer_device_memory;
		create_buffer(size, VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer, staging_buffer_device_memory);

		void* data;
		WM_ASSERT_VULKAN(vkMapMemory(device, staging_buffer_device_memory, 0, size, 0, &data));
		std::memcpy(data, font_data, static_cast<size_t>(size));
		vkUnmapMemory(device, staging_buffer_device_memory);

		create_image(glm::ivec2(font_texture_width, font_texture_height), 1, VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, VkImageTiling::VK_IMAGE_TILING_OPTIMAL, VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, font_image, font_image_device_memory);
		font_image_view = create_image_view(font_image, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 1);

		transition_image_layout(font_image, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1);
		copy_buffer_to_image(staging_buffer, font_image, font_texture_width, font_texture_height);
		transition_image_layout(font_image, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1);

		vkDestroyBuffer(device, staging_buffer, nullptr);
		vkFreeMemory(device, staging_buffer_device_memory, nullptr);

		create_sampler(1, 0.0f, font_sampler);
	}

	void wm_vulkan_rendering_system::create_imgui_descriptor_sets() {
		VkDescriptorPoolSize descriptor_pool_size{};
		descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptor_pool_size.descriptorCount = static_cast<uint32_t>(swap_chain_images.size());
		std::array<VkDescriptorPoolSize, 1> descriptor_pool_sizes = {descriptor_pool_size};

		VkDescriptorPoolCreateInfo descriptor_pool_create_info{};
		descriptor_pool_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptor_pool_create_info.maxSets = static_cast<uint32_t>(swap_chain_images.size());
		descriptor_pool_create_info.poolSizeCount = static_cast<uint32_t>(descriptor_pool_sizes.size());
		descriptor_pool_create_info.pPoolSizes = descriptor_pool_sizes.data();

		WM_ASSERT_VULKAN(vkCreateDescriptorPool(device, &descriptor_pool_create_info, nullptr, &imgui_descriptor_pool));

		VkDescriptorSetLayoutBinding descriptor_set_layout_binding{};
		descriptor_set_layout_binding.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptor_set_layout_binding.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
		descriptor_set_layout_binding.binding = 0;
		descriptor_set_layout_binding.descriptorCount = 1;
		std::array<VkDescriptorSetLayoutBinding, 1> descriptor_set_layout_bindings = {descriptor_set_layout_binding};

		VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info{};
		descriptor_set_layout_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptor_set_layout_create_info.bindingCount = static_cast<uint32_t>(descriptor_set_layout_bindings.size());
		descriptor_set_layout_create_info.pBindings = descriptor_set_layout_bindings.data();

		WM_ASSERT_VULKAN(vkCreateDescriptorSetLayout(device, &descriptor_set_layout_create_info, nullptr, &imgui_descriptor_set_layout));

		std::vector<VkDescriptorSetLayout> descriptor_set_layouts(swap_chain_images.size(), imgui_descriptor_set_layout);
		VkDescriptorSetAllocateInfo descriptor_set_allocate_info{};
		descriptor_set_allocate_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptor_set_allocate_info.descriptorPool = imgui_descriptor_pool;
		descriptor_set_allocate_info.descriptorSetCount = static_cast<uint32_t>(descriptor_set_layouts.size());
		descriptor_set_allocate_info.pSetLayouts = descriptor_set_layouts.data();

		imgui_descriptor_sets.resize(swap_chain_images.size());
		WM_ASSERT_VULKAN(vkAllocateDescriptorSets(device, &descriptor_set_allocate_info, imgui_descriptor_sets.data()));

		for(size_t i = 0; i < swap_chain_images.size(); i++) {
			VkDescriptorImageInfo descriptor_image_info{};
			descriptor_image_info.imageLayout = VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			descriptor_image_info.imageView = font_image_view;
			descriptor_image_info.sampler = font_sampler;
			std::array<VkDescriptorImageInfo, 1> descriptor_image_infos = {descriptor_image_info};

			VkWriteDescriptorSet write_descriptor_set{};
			write_descriptor_set.sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write_descriptor_set.dstSet = imgui_descriptor_sets.at(i);
			write_descriptor_set.dstBinding = 0;
			write_descriptor_set.dstArrayElement = 0;
			write_descriptor_set.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			write_descriptor_set.descriptorCount = static_cast<uint32_t>(descriptor_image_infos.size());
			write_descriptor_set.pImageInfo = descriptor_image_infos.data();

			std::array<VkWriteDescriptorSet, 1> write_descriptor_sets = {write_descriptor_set};

			vkUpdateDescriptorSets(device, static_cast<uint32_t>(write_descriptor_sets.size()), write_descriptor_sets.data(), 0, nullptr);
		}
	}

	void wm_vulkan_rendering_system::create_imgui_pipeline() {
		VkPushConstantRange push_constant_range{};
		push_constant_range.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
		push_constant_range.offset = 0;
		push_constant_range.size = sizeof(push_constatnt_block);
		std::array<VkPushConstantRange, 1> push_constant_ranges = {push_constant_range};

		std::array<VkDescriptorSetLayout, 1> descriptor_set_layouts = {imgui_descriptor_set_layout};

		VkPipelineLayoutCreateInfo pipeline_layout_create_info{};
		pipeline_layout_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeline_layout_create_info.setLayoutCount = static_cast<uint32_t>(descriptor_set_layouts.size());
		pipeline_layout_create_info.pSetLayouts = descriptor_set_layouts.data();
		pipeline_layout_create_info.pushConstantRangeCount = static_cast<uint32_t>(push_constant_ranges.size());
		pipeline_layout_create_info.pPushConstantRanges = push_constant_ranges.data();
		WM_ASSERT_VULKAN(vkCreatePipelineLayout(device, &pipeline_layout_create_info, nullptr, &imgui_pipeline_layout));

		VkPipelineInputAssemblyStateCreateInfo pipeline_input_assembly_state_create_info{};
		pipeline_input_assembly_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		pipeline_input_assembly_state_create_info.topology = VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		VkPipelineRasterizationStateCreateInfo pipeline_rasterization_state_create_info{};
		pipeline_rasterization_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		pipeline_rasterization_state_create_info.polygonMode = VkPolygonMode::VK_POLYGON_MODE_FILL;
		pipeline_rasterization_state_create_info.cullMode = VkCullModeFlagBits::VK_CULL_MODE_NONE;
		pipeline_rasterization_state_create_info.frontFace = VkFrontFace::VK_FRONT_FACE_COUNTER_CLOCKWISE;
		pipeline_rasterization_state_create_info.lineWidth = 1.0f;

		VkPipelineColorBlendAttachmentState pipeline_color_blend_attachment_state{};
		pipeline_color_blend_attachment_state.blendEnable = VK_TRUE;
		pipeline_color_blend_attachment_state.colorWriteMask = VkColorComponentFlagBits::VK_COLOR_COMPONENT_R_BIT | VkColorComponentFlagBits::VK_COLOR_COMPONENT_G_BIT | VkColorComponentFlagBits::VK_COLOR_COMPONENT_B_BIT | VkColorComponentFlagBits::VK_COLOR_COMPONENT_A_BIT;
		pipeline_color_blend_attachment_state.srcColorBlendFactor = VkBlendFactor::VK_BLEND_FACTOR_SRC_ALPHA;
		pipeline_color_blend_attachment_state.dstColorBlendFactor = VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		pipeline_color_blend_attachment_state.colorBlendOp = VkBlendOp::VK_BLEND_OP_ADD;
		pipeline_color_blend_attachment_state.srcAlphaBlendFactor = VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		pipeline_color_blend_attachment_state.dstAlphaBlendFactor = VkBlendFactor::VK_BLEND_FACTOR_ZERO;
		pipeline_color_blend_attachment_state.alphaBlendOp = VkBlendOp::VK_BLEND_OP_ADD;
		std::array<VkPipelineColorBlendAttachmentState, 1> pipeline_color_blend_attachment_states = {pipeline_color_blend_attachment_state};

		VkPipelineColorBlendStateCreateInfo pipeline_color_blend_state_create_info{};
		pipeline_color_blend_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		pipeline_color_blend_state_create_info.attachmentCount = static_cast<uint32_t>(pipeline_color_blend_attachment_states.size());
		pipeline_color_blend_state_create_info.pAttachments = pipeline_color_blend_attachment_states.data();

		VkPipelineDepthStencilStateCreateInfo pipeline_depth_stencil_state_create_info{};
		pipeline_depth_stencil_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		pipeline_depth_stencil_state_create_info.depthTestEnable = VK_FALSE;
		pipeline_depth_stencil_state_create_info.depthWriteEnable = VK_FALSE;
		pipeline_depth_stencil_state_create_info.depthCompareOp = VkCompareOp::VK_COMPARE_OP_LESS_OR_EQUAL;
		pipeline_depth_stencil_state_create_info.back.compareOp = VkCompareOp::VK_COMPARE_OP_ALWAYS;

		VkPipelineViewportStateCreateInfo pipeline_viewport_state_create_info{};
		pipeline_viewport_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		pipeline_viewport_state_create_info.viewportCount = 1;
		pipeline_viewport_state_create_info.scissorCount = 1;

		std::array<VkDynamicState, 2> dynamic_states = {VkDynamicState::VK_DYNAMIC_STATE_VIEWPORT, VkDynamicState::VK_DYNAMIC_STATE_SCISSOR};

		VkPipelineDynamicStateCreateInfo pipeline_dynamic_state_create_info{};
		pipeline_dynamic_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		pipeline_dynamic_state_create_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
		pipeline_dynamic_state_create_info.pDynamicStates = dynamic_states.data();

		VkPipelineMultisampleStateCreateInfo pipeline_multisample_state_create_info{};
		pipeline_multisample_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		pipeline_multisample_state_create_info.rasterizationSamples = msaa_sample_count;

		auto verteex_shader_code = read_file("res/shader/imgui.vert.spv");
		WM_LOG_INFO_2("ImGui vertex shader loaded");
		auto fragment_shader_code = read_file("res/shader/imgui.frag.spv");
		WM_LOG_INFO_2("ImGui fragment shader loaded");
		auto vertex_shader_module = create_shader_module(verteex_shader_code);
		auto fragment_shader_module = create_shader_module(fragment_shader_code);

		VkPipelineShaderStageCreateInfo pipeline_vertex_shader_stage_create_info{};
		pipeline_vertex_shader_stage_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		pipeline_vertex_shader_stage_create_info.stage = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
		pipeline_vertex_shader_stage_create_info.module = vertex_shader_module;
		pipeline_vertex_shader_stage_create_info.pName = "main";

		VkPipelineShaderStageCreateInfo pipeline_fragment_shader_stage_create_info{};
		pipeline_fragment_shader_stage_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		pipeline_fragment_shader_stage_create_info.stage = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
		pipeline_fragment_shader_stage_create_info.module = fragment_shader_module;
		pipeline_fragment_shader_stage_create_info.pName = "main";

		std::array<VkPipelineShaderStageCreateInfo, 2> pipeline_shader_stage_create_infos = {pipeline_vertex_shader_stage_create_info, pipeline_fragment_shader_stage_create_info};

		VkVertexInputBindingDescription binding_description{};
		binding_description.binding = 0;
		binding_description.stride = sizeof(ImDrawVert);
		binding_description.inputRate = VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;
		std::array<VkVertexInputBindingDescription, 1> binding_descriptions = {binding_description};

		VkVertexInputAttributeDescription position_attribute_description;
		position_attribute_description.binding = 0;
		position_attribute_description.location = 0;
		position_attribute_description.format = VkFormat::VK_FORMAT_R32G32_SFLOAT;
		position_attribute_description.offset = offsetof(ImDrawVert, pos);

		VkVertexInputAttributeDescription color_attribute_description;
		color_attribute_description.binding = 0;
		color_attribute_description.location = 1;
		color_attribute_description.format = VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
		color_attribute_description.offset = offsetof(ImDrawVert, col);

		VkVertexInputAttributeDescription texture_coordinates_attribute_description;
		texture_coordinates_attribute_description.binding = 0;
		texture_coordinates_attribute_description.location = 2;
		texture_coordinates_attribute_description.format = VkFormat::VK_FORMAT_R32G32_SFLOAT;
		texture_coordinates_attribute_description.offset = offsetof(ImDrawVert, uv);

		std::array<VkVertexInputAttributeDescription, 3> attribute_descriptions{position_attribute_description, texture_coordinates_attribute_description, color_attribute_description};

		VkPipelineVertexInputStateCreateInfo pipeline_vertex_input_state_create_info{};
		pipeline_vertex_input_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		pipeline_vertex_input_state_create_info.vertexBindingDescriptionCount = static_cast<uint32_t>(binding_descriptions.size());
		pipeline_vertex_input_state_create_info.pVertexBindingDescriptions = binding_descriptions.data();
		pipeline_vertex_input_state_create_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size());
		pipeline_vertex_input_state_create_info.pVertexAttributeDescriptions = attribute_descriptions.data();

		VkGraphicsPipelineCreateInfo graphics_pipeline_create_info{};
		graphics_pipeline_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		graphics_pipeline_create_info.stageCount = static_cast<uint32_t>(pipeline_shader_stage_create_infos.size());
		graphics_pipeline_create_info.pStages = pipeline_shader_stage_create_infos.data();
		graphics_pipeline_create_info.pVertexInputState = &pipeline_vertex_input_state_create_info;
		graphics_pipeline_create_info.pInputAssemblyState = &pipeline_input_assembly_state_create_info;
		graphics_pipeline_create_info.pViewportState = &pipeline_viewport_state_create_info;
		graphics_pipeline_create_info.pDynamicState = &pipeline_dynamic_state_create_info;
		graphics_pipeline_create_info.pRasterizationState = &pipeline_rasterization_state_create_info;
		graphics_pipeline_create_info.pMultisampleState = &pipeline_multisample_state_create_info;
		graphics_pipeline_create_info.pColorBlendState = &pipeline_color_blend_state_create_info;
		graphics_pipeline_create_info.layout = imgui_pipeline_layout;
		graphics_pipeline_create_info.renderPass = render_pass;
		graphics_pipeline_create_info.subpass = 0;
		graphics_pipeline_create_info.pDepthStencilState = &pipeline_depth_stencil_state_create_info;

		std::array<VkGraphicsPipelineCreateInfo, 1> graphics_pipeline_create_infos = {graphics_pipeline_create_info};

		WM_ASSERT_VULKAN(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, static_cast<uint32_t>(graphics_pipeline_create_infos.size()), graphics_pipeline_create_infos.data(), nullptr, &imgui_pipeline));

		vkDestroyShaderModule(device, vertex_shader_module, nullptr);
		vkDestroyShaderModule(device, fragment_shader_module, nullptr);
	}

	void wm_vulkan_rendering_system::prepare_imgui() {
		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize = ImVec2(swap_chain_extent.width, swap_chain_extent.height);
		io.DeltaTime = engine::get_time_system()->get_frame_time() / 1000.0f;

		auto input_handler = engine::get_window_system()->get_input_handler();

		glm::dvec2 mouse_position = engine::get_window_system()->get_input_handler()->get_mouse_position().get_position();
		io.MousePos = ImVec2(mouse_position.x, mouse_position.y);
		io.MouseDown[0] = input_handler->get_mouse_button_state(mouse_button::button_left).is_down();
		io.MouseDown[1] = input_handler->get_mouse_button_state(mouse_button::button_right).is_down();
		io.MouseDown[2] = input_handler->get_mouse_button_state(mouse_button::button_middle).is_down();
		io.MouseDown[3] = input_handler->get_mouse_button_state(mouse_button::button_4).is_down();
		io.MouseDown[4] = input_handler->get_mouse_button_state(mouse_button::button_5).is_down();
		io.MouseWheelH = mouse_scroll.x;
		io.MouseWheel = mouse_scroll.y;
		io.KeyCtrl = input_handler->get_keyboard_button_state(keyboard_button::button_left_control).is_down() || input_handler->get_keyboard_button_state(keyboard_button::button_right_control).is_down();
		io.KeyShift = input_handler->get_keyboard_button_state(keyboard_button::button_left_shift).is_down() || input_handler->get_keyboard_button_state(keyboard_button::button_right_shift).is_down();
		io.KeyAlt = input_handler->get_keyboard_button_state(keyboard_button::button_left_alt).is_down() || input_handler->get_keyboard_button_state(keyboard_button::button_right_alt).is_down();
		io.KeySuper = input_handler->get_keyboard_button_state(keyboard_button::button_left_super).is_down() || input_handler->get_keyboard_button_state(keyboard_button::button_right_super).is_down();
		for(auto button : utility::get_keyboard_buttons()) {
			io.KeysDown[static_cast<uint32_t>(button)] = input_handler->get_keyboard_button_state(button).is_down();
		}

		const float AXIS_TRESHOLD = 0.3f; //TODO
		for(int32_t i = 0; i < input_handler->get_max_gamepad_count(); i++) {
			if(input_handler->is_gamepad_available(i)) {
				//buttons
				io.NavInputs[ImGuiNavInput_::ImGuiNavInput_Activate] = input_handler->get_gamepad_button_state(i, gamepad_button::button_a).is_down() ? 1.0f : 0.0f;
				io.NavInputs[ImGuiNavInput_::ImGuiNavInput_Cancel] = input_handler->get_gamepad_button_state(i, gamepad_button::button_b).is_down() ? 1.0f : 0.0f;
				io.NavInputs[ImGuiNavInput_::ImGuiNavInput_Menu] = input_handler->get_gamepad_button_state(i, gamepad_button::button_x).is_down() ? 1.0f : 0.0f;
				io.NavInputs[ImGuiNavInput_::ImGuiNavInput_Input] = input_handler->get_gamepad_button_state(i, gamepad_button::button_y).is_down() ? 1.0f : 0.0f;
				//dpad
				io.NavInputs[ImGuiNavInput_::ImGuiNavInput_DpadLeft] = input_handler->get_gamepad_button_state(i, gamepad_button::button_dpad_left).is_down() ? 1.0f : 0.0f;
				io.NavInputs[ImGuiNavInput_::ImGuiNavInput_DpadRight] = input_handler->get_gamepad_button_state(i, gamepad_button::button_dpad_right).is_down() ? 1.0f : 0.0f;
				io.NavInputs[ImGuiNavInput_::ImGuiNavInput_DpadUp] = input_handler->get_gamepad_button_state(i, gamepad_button::button_dpad_up).is_down() ? 1.0f : 0.0f;
				io.NavInputs[ImGuiNavInput_::ImGuiNavInput_DpadDown] = input_handler->get_gamepad_button_state(i, gamepad_button::button_dpad_down).is_down() ? 1.0f : 0.0f;
				//bumpers
				io.NavInputs[ImGuiNavInput_::ImGuiNavInput_FocusPrev] = input_handler->get_gamepad_button_state(i, gamepad_button::button_left_bumper).is_down() ? 1.0f : 0.0f;
				io.NavInputs[ImGuiNavInput_::ImGuiNavInput_FocusNext] = input_handler->get_gamepad_button_state(i, gamepad_button::button_right_bumper).is_down() ? 1.0f : 0.0f;
				//triggers
				io.NavInputs[ImGuiNavInput_::ImGuiNavInput_TweakSlow] = input_handler->get_gamepad_axis_state(i, gamepad_axis::axis_left_trigger).get_value();
				io.NavInputs[ImGuiNavInput_::ImGuiNavInput_TweakFast] = input_handler->get_gamepad_axis_state(i, gamepad_axis::axis_right_trigger).get_value();
				//axes
				float y_axis = input_handler->get_gamepad_axis_state(i, gamepad_axis::axis_left_y).get_value();
				io.NavInputs[ImGuiNavInput_LStickUp] = std::abs(y_axis) > AXIS_TRESHOLD ? -y_axis : 0.0f;
				io.NavInputs[ImGuiNavInput_LStickDown] = std::abs(y_axis) > AXIS_TRESHOLD ? y_axis : 0.0f;
				float x_axis = input_handler->get_gamepad_axis_state(i, gamepad_axis::axis_left_x).get_value();
				io.NavInputs[ImGuiNavInput_LStickRight] = std::abs(x_axis) > AXIS_TRESHOLD ? x_axis : 0.0f;
				io.NavInputs[ImGuiNavInput_LStickLeft] = std::abs(x_axis) > AXIS_TRESHOLD ? -x_axis : 0.0f;
				break;
			}
		}

		mouse_scroll = glm::dvec2(0.0);
	}

	void wm_vulkan_rendering_system::before_draw_imgui(const uint32_t image_index) {
		ImGui::NewFrame();

		//ImGui::ShowDemoWindow();

		ImVec2 position{20.0f, 20.0f};
		ImGui::SetNextWindowPos(position);
		ImGui::Begin("Statistics", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_::ImGuiWindowFlags_NoNav);
		ImGui::Text((std::to_string(engine::get_time_system()->get_fps()) + " FPS").c_str());
		ImGui::Text((std::to_string(engine::get_time_system()->get_frame_time()) + " ms").c_str());
		ImGui::Text((std::string("Resolution: ") + std::to_string(swap_chain_extent.width) + " x " + std::to_string(swap_chain_extent.height) + " px").c_str());
		ImGui::Text("Vulkan");
		ImGui::End();
		ImGui::Render();

		auto draw_data = ImGui::GetDrawData();

		if(draw_data->TotalVtxCount > 0) {
			VkDeviceSize buffer_size = draw_data->TotalVtxCount * sizeof(ImDrawVert);
			if(imgui_vertex_count.at(image_index) < draw_data->TotalVtxCount) {
				vkDestroyBuffer(device, imgui_vertex_buffers.at(image_index), nullptr);
				vkFreeMemory(device, imgui_vertex_buffer_device_memories.at(image_index), nullptr);
				create_buffer(buffer_size, VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, imgui_vertex_buffers.at(image_index), imgui_vertex_buffer_device_memories.at(image_index));
			}
			imgui_vertex_count.at(image_index) = draw_data->TotalVtxCount;
			void* data;
			WM_ASSERT_VULKAN(vkMapMemory(device, imgui_vertex_buffer_device_memories.at(image_index), 0, buffer_size, 0, &data));
			ImDrawVert* vertex_data_address = static_cast<ImDrawVert*>(data);
			for(int32_t i = 0; i < draw_data->CmdListsCount; i++) {
				const ImDrawList* cmd_list = draw_data->CmdLists[i];
				std::memcpy(vertex_data_address, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
				vertex_data_address += cmd_list->VtxBuffer.Size;
			}
			vkUnmapMemory(device, imgui_vertex_buffer_device_memories.at(image_index));
		}

		if(draw_data->TotalIdxCount > 0) {
			VkDeviceSize buffer_size = draw_data->TotalIdxCount * sizeof(ImDrawIdx);
			if(imgui_index_count.at(image_index) < draw_data->TotalIdxCount) {
				vkDestroyBuffer(device, imgui_index_buffers.at(image_index), nullptr);
				vkFreeMemory(device, imgui_index_buffer_device_memories.at(image_index), nullptr);
				create_buffer(buffer_size, VkBufferUsageFlagBits::VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, imgui_index_buffers.at(image_index), imgui_index_buffer_device_memories.at(image_index));
			}
			imgui_index_count.at(image_index) = draw_data->TotalIdxCount;
			void* data;
			WM_ASSERT_VULKAN(vkMapMemory(device, imgui_index_buffer_device_memories.at(image_index), 0, buffer_size, 0, &data));
			ImDrawIdx* index_data_address = static_cast<ImDrawIdx*>(data);
			for(int32_t i = 0; i < draw_data->CmdListsCount; i++) {
				const ImDrawList* cmd_list = draw_data->CmdLists[i];
				std::memcpy(index_data_address, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
				index_data_address += cmd_list->IdxBuffer.Size;
			}
			vkUnmapMemory(device, imgui_index_buffer_device_memories.at(image_index));
		}
	}

	void wm_vulkan_rendering_system::draw_imgui(const uint32_t image_index) {
		ImGuiIO& io = ImGui::GetIO();

		vkCmdBindDescriptorSets(command_buffers.at(image_index), VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, imgui_pipeline_layout, 0, 1, &imgui_descriptor_sets.at(image_index), 0, nullptr);
		vkCmdBindPipeline(command_buffers.at(image_index), VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, imgui_pipeline);

		VkViewport viewport{};
		viewport.width = io.DisplaySize.x;
		viewport.height = io.DisplaySize.y;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(command_buffers.at(image_index), 0, 1, &viewport);

		imgui_push_constants.scale = glm::vec2(2.0f / io.DisplaySize.x, 2.0f / io.DisplaySize.y);
		imgui_push_constants.translate = glm::vec2(-1.0f);
		vkCmdPushConstants(command_buffers.at(image_index), imgui_pipeline_layout, VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(push_constatnt_block), &imgui_push_constants);

		auto draw_data = ImGui::GetDrawData();

		if(draw_data->CmdListsCount > 0) {
			VkDeviceSize offsets[1] = {0};
			vkCmdBindVertexBuffers(command_buffers.at(image_index), 0, 1, &imgui_vertex_buffers.at(image_index), offsets);
			vkCmdBindIndexBuffer(command_buffers.at(image_index), imgui_index_buffers.at(image_index), 0, VkIndexType::VK_INDEX_TYPE_UINT16);

			int32_t vertex_offset = 0;
			int32_t index_offset = 0;
			for(int32_t i = 0; i < draw_data->CmdListsCount; i++) {
				auto cmd_list = draw_data->CmdLists[i];
				for(int32_t j = 0; j < cmd_list->CmdBuffer.Size; j++) {
					auto pcmd = &cmd_list->CmdBuffer[j];
					VkRect2D scissor_rectangle;
					scissor_rectangle.offset.x = std::max((int32_t) (pcmd->ClipRect.x), 0);
					scissor_rectangle.offset.y = std::max((int32_t) (pcmd->ClipRect.y), 0);
					scissor_rectangle.extent.width = (uint32_t) (pcmd->ClipRect.z - pcmd->ClipRect.x);
					scissor_rectangle.extent.height = (uint32_t) (pcmd->ClipRect.w - pcmd->ClipRect.y);
					vkCmdSetScissor(command_buffers.at(image_index), 0, 1, &scissor_rectangle);
					vkCmdDrawIndexed(command_buffers.at(image_index), pcmd->ElemCount, 1, index_offset, vertex_offset, 0);
					index_offset += pcmd->ElemCount;
				}
				vertex_offset += cmd_list->VtxBuffer.Size;
			}
		}
	}

	void wm_vulkan_rendering_system::destroy_imgui() {
		ImGui::DestroyContext();
		for(int32_t i = 0; i < swap_chain_images.size(); i++) {
			vkDestroyBuffer(device, imgui_vertex_buffers.at(i), nullptr);
			vkFreeMemory(device, imgui_vertex_buffer_device_memories.at(i), nullptr);
			vkDestroyBuffer(device, imgui_index_buffers.at(i), nullptr);
			vkFreeMemory(device, imgui_index_buffer_device_memories.at(i), nullptr);
		}
		vkDestroySampler(device, font_sampler, nullptr);
		vkDestroyImageView(device, font_image_view, nullptr);
		vkDestroyImage(device, font_image, nullptr);
		vkFreeMemory(device, font_image_device_memory, nullptr);
		vkDestroyPipeline(device, imgui_pipeline, nullptr);
		vkDestroyPipelineLayout(device, imgui_pipeline_layout, nullptr);
		vkDestroyDescriptorPool(device, imgui_descriptor_pool, nullptr);
		vkDestroyDescriptorSetLayout(device, imgui_descriptor_set_layout, nullptr);

		WM_LOG_INFO_2("ImGui destroyed");
	}

	//drawing
	void wm_vulkan_rendering_system::update() {
		if(engine::get_window_system()->is_closing() || minimized) {
			return;
		}

		prepare_imgui();

		frame_index = engine::get_time_system()->get_frame_index() % MAX_FRAMES_IN_FLIGHT;
		uint32_t image_index;

		WM_ASSERT_VULKAN(vkWaitForFences(device, 1, &in_flight.at(frame_index), VK_TRUE, UINT64_MAX));

		auto result = vkAcquireNextImageKHR(device, swap_chain, UINT64_MAX, image_is_available.at(frame_index), VK_NULL_HANDLE, &image_index);
		if(result == VkResult::VK_ERROR_OUT_OF_DATE_KHR) {
			recreate_swap_chain();
		} else if(result != VkResult::VK_SUBOPTIMAL_KHR) {
			WM_ASSERT_VULKAN(result);
		}

		if(images_in_flight.at(image_index) != VK_NULL_HANDLE) {
			WM_ASSERT_VULKAN(vkWaitForFences(device, 1, &images_in_flight.at(image_index), VK_TRUE, UINT64_MAX));
		}
		images_in_flight.at(image_index) = images_in_flight.at(frame_index);

		std::array<VkSemaphore, 1> image_is_available_semaphores = {image_is_available.at(frame_index)};
		std::array<VkPipelineStageFlags, 1> pipeline_stages = {VkPipelineStageFlagBits::VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		std::array<VkSemaphore, 1> rendering_is_finished_semaphores = {rendering_is_finished.at(frame_index)};

		update_uniform_buffer(image_index);
		update_command_buffer(image_index);

		VkSubmitInfo submit_info{};
		submit_info.sType = VkStructureType::VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.waitSemaphoreCount = static_cast<uint32_t>(image_is_available_semaphores.size());
		submit_info.pWaitSemaphores = image_is_available_semaphores.data();
		submit_info.pWaitDstStageMask = pipeline_stages.data();
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &command_buffers.at(image_index);
		submit_info.signalSemaphoreCount = static_cast<uint32_t>(rendering_is_finished_semaphores.size());
		submit_info.pSignalSemaphores = rendering_is_finished_semaphores.data();

		WM_ASSERT_VULKAN(vkResetFences(device, 1, &in_flight.at(frame_index)));

		WM_ASSERT_VULKAN(vkQueueSubmit(graphics_queue, 1, &submit_info, in_flight.at(frame_index)));

		std::array<VkSwapchainKHR, 1> swap_chains = {swap_chain};

		VkPresentInfoKHR present_info{};
		present_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present_info.waitSemaphoreCount = static_cast<uint32_t>(rendering_is_finished_semaphores.size());
		present_info.pWaitSemaphores = rendering_is_finished_semaphores.data();
		present_info.swapchainCount = static_cast<uint32_t>(swap_chains.size());
		present_info.pSwapchains = swap_chains.data();
		present_info.pImageIndices = &image_index;

		result = vkQueuePresentKHR(graphics_queue, &present_info);
		if(result == VkResult::VK_ERROR_OUT_OF_DATE_KHR || result == VkResult::VK_SUBOPTIMAL_KHR) {
			recreate_swap_chain();
		} else {
			if(framebuffer_resized) {
				recreate_swap_chain();
			}
			WM_ASSERT_VULKAN(result);
		}

		WM_LOG_INFO_3("vulkan rendering system updated");
	}

	wm_vulkan_rendering_system::~wm_vulkan_rendering_system() {
		vkDeviceWaitIdle(device);

		destroy_imgui();

		for(int32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(device, image_is_available.at(i), nullptr);
			vkDestroySemaphore(device, rendering_is_finished.at(i), nullptr);
			vkDestroyFence(device, in_flight.at(i), nullptr);
		}

		vkDestroyPipeline(device, pipeline, nullptr);
		vkDestroyPipelineLayout(device, pipeline_layout, nullptr);
		WM_LOG_INFO_2("Vulkan graphics pipeline destroyed");

		cleanup_swap_chain();

		vkDestroySampler(device, texture_sampler, nullptr);
		vkDestroyImageView(device, texture_image_view, nullptr);
		vkDestroyImage(device, texture_image, nullptr);
		vkFreeMemory(device, texture_image_device_memory, nullptr);
		WM_LOG_INFO_2("Vulkan texture destroyed");

		vkDestroyDescriptorSetLayout(device, descriptor_set_layout, nullptr);

		vkDestroyBuffer(device, index_buffer, nullptr);
		vkFreeMemory(device, index_buffer_device_memory, nullptr);
		WM_LOG_INFO_2("Vulkan index buffer destroyed");

		vkDestroyBuffer(device, vertex_buffer, nullptr);
		vkFreeMemory(device, vertex_buffer_device_memory, nullptr);
		WM_LOG_INFO_2("Vulkan vertex buffer destroyed");

		vkDestroyCommandPool(device, command_pool, nullptr);

		vkDestroyDevice(device, nullptr);
		device = VK_NULL_HANDLE;
		WM_LOG_INFO_2("Vulkan device destroyed");

	#ifdef WM_BUILD_DEBUG
		destroy_debug_utils_messenger();
	#endif
		vkDestroySurfaceKHR(instance, surface, nullptr);
		surface = VK_NULL_HANDLE;
		WM_LOG_INFO_2("Vulkan surface destroyed");

		vkDestroyInstance(instance, nullptr);
		instance = VK_NULL_HANDLE;
		WM_LOG_INFO_2("Vulkan instance destroyed");

		WM_LOG_INFO_1("Vulkan rendering system destroyed");
	}

}
