#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

#include "core/engine.h"
#include "defines/log_defines.h"
#include "window/event/window_framebuffer_size_event.h"

#include "wm_vulkan_rendering_system.h"
#include "../window/wm_glfw_window.h"

namespace wm {

	const std::vector<vertex> wm_vulkan_rendering_system::vertices = {
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};

	const std::vector<uint16_t> wm_vulkan_rendering_system::indices = {
		0, 1, 2, 2, 3, 0
	};


	wm_vulkan_rendering_system::wm_vulkan_rendering_system() {
		create_instance();
	#ifdef WM_BUILD_DEBUG
		create_debug_utils_messenger();
	#endif
		create_surface();
		create_device();
		create_swap_chain();
		create_swap_chain_image_views();
		create_render_pass();
		create_descriptor_set_layout();
		create_pipeline();
		create_framebuffers();
		create_command_pool();
		create_vertex_buffer();
		create_index_buffer();
		create_uniform_buffers();
		create_descriptor_pool();
		create_descriptor_sets();
		create_command_buffers();
		create_semaphores();

		auto el = ptr<event_listener<window_framebuffer_size_event>>(new event_listener<window_framebuffer_size_event>([this](const window_framebuffer_size_event event) {
			this->framebuffer_resized = true;
			this->minimized = event.get_new_size().x == 0 || event.get_new_size().y == 0;
		})).to_ptr_view();
		engine::get_event_system()->add_event_listener(window_framebuffer_size_event::get_key(), el);

		WM_LOG_INFO_1("vulkan rendering system constructed");
	}

	void wm_vulkan_rendering_system::create_instance() {
		const auto layers = get_layers();
		const auto extensions = get_extensions();

		VkApplicationInfo application_info {};
		application_info.sType = VkStructureType::VK_STRUCTURE_TYPE_APPLICATION_INFO;
		application_info.pEngineName = "Windmill Game Engine";									// TODO: nem ide kéne beégetni, hanem valahol, pl. engine-ben definiálni
		application_info.engineVersion = VK_MAKE_API_VERSION(0, 0, 1, 0);						// TODO: nem ide kéne beégetni, hanem valahol, pl. engine-ben definiálni
		application_info.pApplicationName = engine::get_app_name().c_str();						// TODO: mi van akkor, ha nincs ott semmi?
		application_info.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);					// TODO: engine parameter container-ből kéne jönnie
		application_info.apiVersion = VK_API_VERSION_1_2;

		VkInstanceCreateInfo instance_create_info {};
		instance_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instance_create_info.pApplicationInfo = &application_info;
		instance_create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		instance_create_info.ppEnabledExtensionNames = extensions.data();
		instance_create_info.enabledLayerCount = static_cast<uint32_t>(layers.size());
		instance_create_info.ppEnabledLayerNames = layers.data();
	#ifdef WM_BUILD_DEBUG
		const auto debug_utils_messenger_create_info = create_debug_utils_messenger_create_info();
		instance_create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debug_utils_messenger_create_info;
		// TODO: VK_EXT_validation_features: ki és be lehet kapcsolni validációs funkciókat
		// ha minden igaz, amiket bekapcsolni lehet, azokat a validation layer is bekapcsolja
		// de van pár, ami alapból be van kapcsolva és release módban ki lehetne szedni
		// https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/chap4.html#VkValidationFeaturesEXT
	#endif

		auto result = vkCreateInstance(&instance_create_info, nullptr, &instance);
		WM_ASSERT(result == VkResult::VK_SUCCESS);
		WM_LOG_INFO_2("Vulkan instance created");
	}


	//layers
	std::vector<const char*> wm_vulkan_rendering_system::get_layers() const {
		const auto required_layers = get_required_layers();
		const auto available_layers = get_available_layers();
	#ifdef WM_BUILD_DEBUG
		//log_available_layers(available_layers);
	#endif
		validate_layers(required_layers, available_layers);
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
		auto result = vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
		WM_ASSERT(result == VkResult::VK_SUCCESS);
		std::vector<VkLayerProperties> available_layers(layer_count);
		result = vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
		WM_ASSERT(result == VkResult::VK_SUCCESS);
		return available_layers;
	}

	void wm_vulkan_rendering_system::log_available_layers(const std::vector<VkLayerProperties>& available_layers) const {
		std::string result_string = "Available Vulkan layers:\r\n";
		for(uint32_t i = 0; i < available_layers.size(); i++) {
			result_string += std::string("\t") + available_layers[i].layerName;
			if(i != available_layers.size() - 1) {
				result_string += "\r\n";
			}
		}
		WM_LOG_INFO_2(result_string);
	}

	void wm_vulkan_rendering_system::validate_layers(const std::vector<const char*>& required_layers, const std::vector<VkLayerProperties>& available_layers) const {
		for(const auto& rl : required_layers) {
			bool layer_found = false;
			for(const auto& al : available_layers) {
				if(strcmp(rl, al.layerName) == 0) {
					layer_found = true;
					break;
				}
			}
			if(!layer_found) {
				throw new std::runtime_error("Layer " + std::string(rl) + " is not available");
			}
		}
	}


	//instance extensions
	std::vector<const char*> wm_vulkan_rendering_system::get_extensions() const {
		const auto required_extensions = get_required_extensions();
		const auto available_extensions = get_available_extensions();
	#ifdef WM_BUILD_DEBUG
		//log_available_extensions(available_extensions);
	#endif
		validate_extensions(required_extensions, available_extensions);
		return required_extensions;
	}

	std::vector<const char*> wm_vulkan_rendering_system::get_required_extensions() const {
		uint32_t window_extension_count;
		const auto window_extensions = glfwGetRequiredInstanceExtensions(&window_extension_count);			// TODO: GLFW-tól függetleníteni kéne
		auto required_extensions = std::vector<const char*>(window_extensions, window_extensions + window_extension_count);
	#ifdef WM_BUILD_DEBUG
		required_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	#endif
		return required_extensions;
	}

	std::vector<VkExtensionProperties> wm_vulkan_rendering_system::get_available_extensions() const {
		uint32_t extension_count;
		auto result = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
		WM_ASSERT(result == VkResult::VK_SUCCESS);
		std::vector<VkExtensionProperties> available_extensions(extension_count);
		result = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, available_extensions.data());
		WM_ASSERT(result == VkResult::VK_SUCCESS);
		return available_extensions;
	}

	void wm_vulkan_rendering_system::log_available_extensions(const std::vector<VkExtensionProperties>& available_extensions) const {
		std::string result_string = "Available Vulkan instance extensions:\r\n";
		for(uint32_t i = 0; i < available_extensions.size(); i++) {
			result_string += std::string("\t") + available_extensions[i].extensionName;
			if(i != available_extensions.size() - 1) {
				result_string += "\r\n";
			}
		}
		WM_LOG_INFO_2(result_string);
	}

	void wm_vulkan_rendering_system::validate_extensions(const std::vector<const char*>& required_extensions, const std::vector<VkExtensionProperties>& available_extensions) const {
		for(const auto& re : required_extensions) {
			bool extension_found = false;
			for(const auto& ae : available_extensions) {
				if(strcmp(re, ae.extensionName) == 0) {
					extension_found = true;
					break;
				}
			}
			if(!extension_found) {
				throw new std::runtime_error("Instance extension " + std::string(re) + " is not available");
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
			WM_THROW_ERROR("Vulkan error: " + severity + ", " + type + ", " + callback_data->pMessage);
		}
		return VK_FALSE;
		// TODO: el lehet nevezni az objektumokat és itt az lehet, hogy hasznos lenne, meg amúgy is érdemes volna megnézni, hogy milyen paramétereket kapok még itt
		// https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/chap45.html#debugging-debug-utils
	}

	VkDebugUtilsMessengerCreateInfoEXT wm_vulkan_rendering_system::create_debug_utils_messenger_create_info() const {
		VkDebugUtilsMessengerCreateInfoEXT debug_utils_messenger_create_info {};
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
		const auto result = vkCreateDebugUtilsMessengerEXT(instance, &debug_utils_messenger_create_info, nullptr, &debug_utils_messenger);
		WM_ASSERT(result == VkResult::VK_SUCCESS);
		WM_LOG_INFO_2("Vulkan debug messenger created");
	}

	void wm_vulkan_rendering_system::destroy_debug_utils_messenger() {
		auto vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		WM_ASSERT(vkDestroyDebugUtilsMessengerEXT != nullptr);
		vkDestroyDebugUtilsMessengerEXT(instance, debug_utils_messenger, nullptr);
		debug_utils_messenger = VK_NULL_HANDLE;
		WM_LOG_INFO_2("Vulkan debug messenger destroyed");
	}


	//surface
	void wm_vulkan_rendering_system::create_surface() {
		auto result = glfwCreateWindowSurface(instance, engine::get_window_system()->get_window(0).convert<wm_glfw_window>()->get_handler(), nullptr, &surface); // TODO
		WM_ASSERT(result == VkResult::VK_SUCCESS);
		WM_LOG_INFO_2("Vulkan surface created");
	}


	//device
	void wm_vulkan_rendering_system::create_device() {
		get_physical_device();

		std::vector<VkDeviceQueueCreateInfo> device_queue_create_infos {};
		std::vector<int32_t> queue_family_indices = {graphics_queue_family_index};
		if(graphics_queue_family_index != presentation_queue_family_index) {
			queue_family_indices.push_back(presentation_queue_family_index);
		}
		std::vector<float> queue_priorities = {1.0};
		for(int32_t queue_fmaily_index : queue_family_indices) {
			VkDeviceQueueCreateInfo device_graphics_queue_create_info {};
			device_graphics_queue_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			device_graphics_queue_create_info.queueFamilyIndex = queue_fmaily_index;
			device_graphics_queue_create_info.queueCount = 1;
			device_graphics_queue_create_info.pQueuePriorities = queue_priorities.data();
			device_queue_create_infos.push_back(device_graphics_queue_create_info);
		}

		//VkPhysicalDeviceFeatures physical_device_features {};

		std::vector<const char*> layers = get_required_layers();
		std::vector<const char*> device_extensions = get_required_device_extensions();

		VkDeviceCreateInfo device_create_info {};
		device_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		device_create_info.queueCreateInfoCount = static_cast<uint32_t>(device_queue_create_infos.size());
		device_create_info.pQueueCreateInfos = device_queue_create_infos.data();
		device_create_info.enabledLayerCount = static_cast<uint32_t>(layers.size());
		device_create_info.ppEnabledLayerNames = layers.data();
		device_create_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
		device_create_info.ppEnabledExtensionNames = device_extensions.data();
		//device_create_info.pEnabledFeatures = &physical_device_features;

		auto result = vkCreateDevice(physical_device, &device_create_info, nullptr, &device);
		WM_ASSERT(result == VkResult::VK_SUCCESS);
		WM_LOG_INFO_2("Vulkan device created");

		vkGetDeviceQueue(device, graphics_queue_family_index, 0, &graphics_queue);
		vkGetDeviceQueue(device, presentation_queue_family_index, 0, &presentation_queue);
	}

	void wm_vulkan_rendering_system::get_physical_device() {
		const auto required_device_extensions = get_required_device_extensions();
		uint32_t physical_device_count;
		auto result = vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr);
		WM_ASSERT(result == VkResult::VK_SUCCESS);
		WM_ASSERT(physical_device_count > 0);
		std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
		result = vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices.data());
		WM_ASSERT(result == VkResult::VK_SUCCESS);
		std::string physical_device_name = "";
		//TODO: egyedi kiválasztási lehetőség
		//vagy preferencia, pl. dedikált vs. integrált
		//vagy id megadása, pl. felületen kiválasztotta a felsoroltak közül
		for(const auto& pd : physical_devices) {
			VkPhysicalDeviceProperties physical_device_properties;
			//VkPhysicalDeviceFeatures physical_device_features;
			VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
			vkGetPhysicalDeviceProperties(pd, &physical_device_properties);
			//vkGetPhysicalDeviceFeatures(pd, &physical_device_features);
			vkGetPhysicalDeviceMemoryProperties(pd, &physical_device_memory_properties);

			uint32_t surface_format_count;
			result = vkGetPhysicalDeviceSurfaceFormatsKHR(pd, surface, &surface_format_count, nullptr);
			WM_ASSERT(result == VkResult::VK_SUCCESS);

			uint32_t surface_presentation_mode_count;
			result = vkGetPhysicalDeviceSurfacePresentModesKHR(pd, surface, &surface_presentation_mode_count, nullptr);
			WM_ASSERT(result == VkResult::VK_SUCCESS);

			const auto available_device_extensions = get_available_device_extensions(pd);
			if(!are_device_extensions_supported(required_device_extensions, available_device_extensions)) {
				continue;
			}
			if(surface_format_count == 0 || surface_presentation_mode_count == 0) {
				continue;
			}

			//TODO: itt lehetne akár csekkolni vulkan api verzió kompatibilitást, textúra méret limiteket stb.
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
					vkGetPhysicalDeviceSurfaceSupportKHR(pd, i, surface, &presentation_supported);
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
		auto result = vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &device_extension_count, nullptr);
		WM_ASSERT(result == VkResult::VK_SUCCESS);
		std::vector<VkExtensionProperties> available_device_extensions(device_extension_count);
		result = vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &device_extension_count, available_device_extensions.data());
		WM_ASSERT(result == VkResult::VK_SUCCESS);
		return available_device_extensions;
	}

	bool wm_vulkan_rendering_system::are_device_extensions_supported(const std::vector<const char*>& required_device_extensions, const std::vector<VkExtensionProperties>& available_device_extensions) const {
		for(const auto& rde : required_device_extensions) {
			bool device_extension_found = false;
			for(const auto& ade : available_device_extensions) {
				if(strcmp(rde, ade.extensionName) == 0) {
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
	void wm_vulkan_rendering_system::create_swap_chain() {
		surface_format = get_surface_format();
		auto surface_presentation_mode = get_surface_presentation_mode();
		auto surface_capabilities = get_surface_capabilities();
		swap_chain_extent = get_swap_chain_extent(surface_capabilities);

		uint32_t image_count = surface_capabilities.minImageCount + 1;
		if(surface_capabilities.maxImageCount > 0 && image_count > surface_capabilities.maxImageCount) {
			image_count = surface_capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR swap_chain_create_info {};
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
		swap_chain_create_info.oldSwapchain = VK_NULL_HANDLE;
		if(graphics_queue_family_index != presentation_queue_family_index) {
			std::vector<uint32_t> queue_family_indices = {static_cast<uint32_t>(graphics_queue_family_index), static_cast<uint32_t>(presentation_queue_family_index)};

			swap_chain_create_info.imageSharingMode = VkSharingMode::VK_SHARING_MODE_CONCURRENT;
			swap_chain_create_info.queueFamilyIndexCount = static_cast<uint32_t>(queue_family_indices.size());
			swap_chain_create_info.pQueueFamilyIndices = queue_family_indices.data();
		} else {
			swap_chain_create_info.imageSharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
		}

		auto result = vkCreateSwapchainKHR(device, &swap_chain_create_info, nullptr, &swap_chain);
		WM_ASSERT(result == VkResult::VK_SUCCESS);

		result = vkGetSwapchainImagesKHR(device, swap_chain, &image_count, nullptr);
		WM_ASSERT(result == VkResult::VK_SUCCESS);
		swap_chain_images.resize(image_count);
		result = vkGetSwapchainImagesKHR(device, swap_chain, &image_count, swap_chain_images.data());
		WM_ASSERT(result == VkResult::VK_SUCCESS);

		WM_LOG_INFO_2("Vulkan swap chain created");
	}

	void wm_vulkan_rendering_system::recreate_swap_chain() {
		framebuffer_resized = false;
		auto result = vkDeviceWaitIdle(device);
		WM_ASSERT(result == VkResult::VK_SUCCESS);

		cleanup_swap_chain();

		create_swap_chain();
		create_swap_chain_image_views();
		create_render_pass();
		create_pipeline();
		create_framebuffers();
		create_uniform_buffers();
		create_descriptor_pool();
		create_descriptor_sets();
		create_command_buffers();
	}

	VkSurfaceFormatKHR wm_vulkan_rendering_system::get_surface_format() const {
		uint32_t surface_format_count;
		auto result = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &surface_format_count, nullptr);
		WM_ASSERT(result == VkResult::VK_SUCCESS);
		std::vector<VkSurfaceFormatKHR> surface_formats(surface_format_count);
		result = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &surface_format_count, surface_formats.data());
		WM_ASSERT(result == VkResult::VK_SUCCESS);

		if(surface_formats.size() == 1 && surface_formats.at(0).format == VkFormat::VK_FORMAT_UNDEFINED) {
			return {VkFormat::VK_FORMAT_B8G8R8A8_SRGB, VkColorSpaceKHR::VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
		}

		for(const auto& sf : surface_formats) {
			if(sf.format == VkFormat::VK_FORMAT_B8G8R8A8_SRGB && sf.colorSpace == VkColorSpaceKHR::VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return sf;
			}
		}
		return surface_formats.at(0);
	}

	VkPresentModeKHR wm_vulkan_rendering_system::get_surface_presentation_mode() const {
		uint32_t surface_presentation_mode_count;
		auto result = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &surface_presentation_mode_count, nullptr);
		WM_ASSERT(result == VkResult::VK_SUCCESS);
		std::vector<VkPresentModeKHR> surface_presentation_modes(surface_presentation_mode_count);
		result = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &surface_presentation_mode_count, surface_presentation_modes.data());
		WM_ASSERT(result == VkResult::VK_SUCCESS);

		for(const auto& spm : surface_presentation_modes) {
			if(spm == VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR) {
				return spm;
			}
		}
		return VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR;
	}

	VkSurfaceCapabilitiesKHR wm_vulkan_rendering_system::get_surface_capabilities() const {
		VkSurfaceCapabilitiesKHR surface_capabilities;
		auto result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &surface_capabilities);
		WM_ASSERT(result == VkResult::VK_SUCCESS);
		return surface_capabilities;
	}

	VkExtent2D wm_vulkan_rendering_system::get_swap_chain_extent(const VkSurfaceCapabilitiesKHR surface_capabilities) const {
		if(surface_capabilities.currentExtent.width != UINT_MAX) {
			return surface_capabilities.currentExtent;
		} else {
			const glm::ivec2 framebuffer_size = engine::get_window_system()->get_window(0)->get_framebuffer_size();
			VkExtent2D extent;
			extent.width = std::clamp(static_cast<uint32_t>(framebuffer_size.x), surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width);
			extent.height = std::clamp(static_cast<uint32_t>(framebuffer_size.y), surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height);
			return extent;
		}
	}

	void wm_vulkan_rendering_system::create_swap_chain_image_views() {
		swap_chain_image_views.resize(swap_chain_images.size());
		for(int32_t i = 0; i < swap_chain_images.size(); i++) {
			VkImageViewCreateInfo image_view_create_info {};
			image_view_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			image_view_create_info.image = swap_chain_images.at(i);
			image_view_create_info.viewType = VkImageViewType::VK_IMAGE_VIEW_TYPE_2D;
			image_view_create_info.format = surface_format.format;
			image_view_create_info.components.r = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_create_info.components.g = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_create_info.components.b = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_create_info.components.a = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_create_info.subresourceRange.aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
			image_view_create_info.subresourceRange.baseMipLevel = 0;
			image_view_create_info.subresourceRange.levelCount = 1;
			image_view_create_info.subresourceRange.baseArrayLayer = 0;
			image_view_create_info.subresourceRange.layerCount = 1;

			auto result = vkCreateImageView(device, &image_view_create_info, nullptr, &swap_chain_image_views.at(i));
			WM_ASSERT(result == VkResult::VK_SUCCESS);
		}
	}

	void wm_vulkan_rendering_system::cleanup_swap_chain() {
		for(auto framebuffer : swap_chain_framebuffers) {
			vkDestroyFramebuffer(device, framebuffer, nullptr);
		}
		vkFreeCommandBuffers(device, command_pool, static_cast<uint32_t>(command_buffers.size()), command_buffers.data());
		vkDestroyPipeline(device, pipeline, nullptr);
		vkDestroyPipelineLayout(device, pipeline_layout, nullptr);
		vkDestroyRenderPass(device, render_pass, nullptr);
		WM_LOG_INFO_2("Vulkan graphics pipeline destroyed");

		for(auto swap_chain_image_view : swap_chain_image_views) {
			vkDestroyImageView(device, swap_chain_image_view, nullptr);
		}
		vkDestroySwapchainKHR(device, swap_chain, nullptr);
		swap_chain = VK_NULL_HANDLE;

		for(size_t i = 0; i < swap_chain_images.size(); i++) {
			vkDestroyBuffer(device, uniform_buffers.at(i), nullptr);
			vkFreeMemory(device, uniform_buffers_device_memory.at(i), nullptr);
		}

		vkDestroyDescriptorPool(device, descriptor_pool, nullptr);

		WM_LOG_INFO_2("Vulkan swap chain destroyed");
	}


	//pipeline
	void wm_vulkan_rendering_system::create_descriptor_set_layout() {
		std::array<VkDescriptorSetLayoutBinding, 1> descriptor_set_layout_bindings;
		VkDescriptorSetLayoutBinding descriptor_set_layout_binding {};
		descriptor_set_layout_binding.binding = 0;
		descriptor_set_layout_binding.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptor_set_layout_binding.descriptorCount = 1;
		descriptor_set_layout_binding.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
		descriptor_set_layout_bindings.at(0) = descriptor_set_layout_binding;

		VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info {};
		descriptor_set_layout_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptor_set_layout_create_info.bindingCount = static_cast<uint32_t>(descriptor_set_layout_bindings.size());
		descriptor_set_layout_create_info.pBindings = descriptor_set_layout_bindings.data();

		auto result = vkCreateDescriptorSetLayout(device, &descriptor_set_layout_create_info, nullptr, &descriptor_set_layout);
		WM_ASSERT(result == VkResult::VK_SUCCESS);
	}

	void wm_vulkan_rendering_system::create_pipeline() {
		auto verteex_shader_code = read_file("res/shader/vert.spv");
		auto fragment_shader_code = read_file("res/shader/frag.spv");
		WM_LOG_INFO_2(std::string("Vertex shader size: ") + std::to_string(verteex_shader_code.size()));
		WM_LOG_INFO_2(std::string("Fragment shader size: ") + std::to_string(fragment_shader_code.size()));
		auto vertex_shader_module = create_shader_module(verteex_shader_code);
		auto fragment_shader_module = create_shader_module(fragment_shader_code);

		VkPipelineShaderStageCreateInfo pipeline_vertex_shader_stage_create_info {};
		pipeline_vertex_shader_stage_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		pipeline_vertex_shader_stage_create_info.stage = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
		pipeline_vertex_shader_stage_create_info.module = vertex_shader_module;
		pipeline_vertex_shader_stage_create_info.pName = "main";

		VkPipelineShaderStageCreateInfo pipeline_fragment_shader_stage_create_info {};
		pipeline_fragment_shader_stage_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		pipeline_fragment_shader_stage_create_info.stage = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
		pipeline_fragment_shader_stage_create_info.module = fragment_shader_module;
		pipeline_fragment_shader_stage_create_info.pName = "main";

		std::vector<VkPipelineShaderStageCreateInfo> pipeline_shader_stage_create_infos = {pipeline_vertex_shader_stage_create_info, pipeline_fragment_shader_stage_create_info};

		auto binding_descriptions = vertex::get_binding_descriptions();
		auto attribute_descriptions = vertex::get_attribute_descriptions();

		VkPipelineVertexInputStateCreateInfo pipeline_vertex_input_state_create_info {};
		pipeline_vertex_input_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		pipeline_vertex_input_state_create_info.vertexBindingDescriptionCount = static_cast<uint32_t>(binding_descriptions.size());
		pipeline_vertex_input_state_create_info.pVertexBindingDescriptions = binding_descriptions.data();
		pipeline_vertex_input_state_create_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size());
		pipeline_vertex_input_state_create_info.pVertexAttributeDescriptions = attribute_descriptions.data();

		VkPipelineInputAssemblyStateCreateInfo pipeline_input_assembly_state_create_info {};
		pipeline_input_assembly_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		pipeline_input_assembly_state_create_info.topology = VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		pipeline_input_assembly_state_create_info.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(swap_chain_extent.width);
		viewport.height = static_cast<float>(swap_chain_extent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor {};
		scissor.offset = {0, 0};
		scissor.extent = swap_chain_extent;

		VkPipelineViewportStateCreateInfo pipeline_viewport_state_create_info {};
		pipeline_viewport_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		pipeline_viewport_state_create_info.viewportCount = 1;
		pipeline_viewport_state_create_info.pViewports = &viewport;
		pipeline_viewport_state_create_info.scissorCount = 1;
		pipeline_viewport_state_create_info.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo pipeline_rasterization_state_create_info {};
		pipeline_rasterization_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		pipeline_rasterization_state_create_info.depthClampEnable = VK_FALSE;
		pipeline_rasterization_state_create_info.rasterizerDiscardEnable = VK_FALSE;
		pipeline_rasterization_state_create_info.polygonMode = VkPolygonMode::VK_POLYGON_MODE_FILL;
		pipeline_rasterization_state_create_info.lineWidth = 1.0f;
		pipeline_rasterization_state_create_info.cullMode = VkCullModeFlagBits::VK_CULL_MODE_BACK_BIT;
		pipeline_rasterization_state_create_info.frontFace = VkFrontFace::VK_FRONT_FACE_CLOCKWISE;
		pipeline_rasterization_state_create_info.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo pipeline_multisample_state_create_info {};
		pipeline_multisample_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		pipeline_multisample_state_create_info.sampleShadingEnable = VK_FALSE;
		pipeline_multisample_state_create_info.rasterizationSamples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;

		std::vector<VkPipelineColorBlendAttachmentState> pipeline_color_blend_attachment_states;
		VkPipelineColorBlendAttachmentState pipeline_color_blend_attachment_state {};
		pipeline_color_blend_attachment_state.colorWriteMask =
			VkColorComponentFlagBits::VK_COLOR_COMPONENT_R_BIT |
			VkColorComponentFlagBits::VK_COLOR_COMPONENT_G_BIT |
			VkColorComponentFlagBits::VK_COLOR_COMPONENT_B_BIT |
			VkColorComponentFlagBits::VK_COLOR_COMPONENT_A_BIT;
		pipeline_color_blend_attachment_state.blendEnable = VK_FALSE;
		pipeline_color_blend_attachment_states.push_back(pipeline_color_blend_attachment_state);

		VkPipelineColorBlendStateCreateInfo pipeline_color_blend_state_create_info {};
		pipeline_color_blend_state_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		pipeline_color_blend_state_create_info.logicOpEnable = VK_FALSE;
		pipeline_color_blend_state_create_info.attachmentCount = static_cast<uint32_t>(pipeline_color_blend_attachment_states.size());
		pipeline_color_blend_state_create_info.pAttachments = pipeline_color_blend_attachment_states.data();

		std::array<VkDescriptorSetLayout, 1> descriptor_set_layouts = {descriptor_set_layout};
		VkPipelineLayoutCreateInfo pipeline_layout_create_info {};
		pipeline_layout_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeline_layout_create_info.setLayoutCount = static_cast<uint32_t>(descriptor_set_layouts.size());
		pipeline_layout_create_info.pSetLayouts = descriptor_set_layouts.data();
		auto result = vkCreatePipelineLayout(device, &pipeline_layout_create_info, nullptr, &pipeline_layout);
		WM_ASSERT(result == VkResult::VK_SUCCESS);

		std::vector<VkGraphicsPipelineCreateInfo> graphics_pipeline_create_infos;
		VkGraphicsPipelineCreateInfo graphics_pipeline_create_info {};
		graphics_pipeline_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		graphics_pipeline_create_info.stageCount = static_cast<uint32_t>(pipeline_shader_stage_create_infos.size());
		graphics_pipeline_create_info.pStages = pipeline_shader_stage_create_infos.data();
		graphics_pipeline_create_info.pVertexInputState = &pipeline_vertex_input_state_create_info;
		graphics_pipeline_create_info.pInputAssemblyState = &pipeline_input_assembly_state_create_info;
		graphics_pipeline_create_info.pViewportState = &pipeline_viewport_state_create_info;
		graphics_pipeline_create_info.pRasterizationState = &pipeline_rasterization_state_create_info;
		graphics_pipeline_create_info.pMultisampleState = &pipeline_multisample_state_create_info;
		graphics_pipeline_create_info.pColorBlendState = &pipeline_color_blend_state_create_info;
		graphics_pipeline_create_info.layout = pipeline_layout;
		graphics_pipeline_create_info.renderPass = render_pass;
		graphics_pipeline_create_info.subpass = 0;
		graphics_pipeline_create_infos.push_back(graphics_pipeline_create_info);

		result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, static_cast<uint32_t>(graphics_pipeline_create_infos.size()), graphics_pipeline_create_infos.data(), nullptr, &pipeline);
		WM_ASSERT(result == VkResult::VK_SUCCESS);

		vkDestroyShaderModule(device, vertex_shader_module, nullptr);
		vkDestroyShaderModule(device, fragment_shader_module, nullptr);

		WM_LOG_INFO_2("Vulkan graphics pipeline created");
	}

	void wm_vulkan_rendering_system::create_render_pass() {
		std::vector<VkSubpassDependency> subpass_dependencies;
		VkSubpassDependency subpass_dependency {};
		subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		subpass_dependency.dstSubpass = 0;
		subpass_dependency.srcStageMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpass_dependency.srcAccessMask = 0;
		subpass_dependency.dstStageMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpass_dependency.dstAccessMask = VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		subpass_dependencies.push_back(subpass_dependency);

		std::vector<VkAttachmentDescription> attachment_descriptions;
		VkAttachmentDescription color_attachment_description {};
		color_attachment_description.format = surface_format.format;
		color_attachment_description.samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
		color_attachment_description.loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
		color_attachment_description.storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
		color_attachment_description.stencilLoadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		color_attachment_description.stencilStoreOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
		color_attachment_description.initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
		color_attachment_description.finalLayout = VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		attachment_descriptions.push_back(color_attachment_description);

		std::vector<VkAttachmentReference> attachment_references;
		VkAttachmentReference color_attachment_reference {};
		color_attachment_reference.attachment = 0;
		color_attachment_reference.layout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachment_references.push_back(color_attachment_reference);

		std::vector<VkSubpassDescription> subpass_descriptions;
		VkSubpassDescription subpass_description {};
		subpass_description.pipelineBindPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass_description.colorAttachmentCount = static_cast<uint32_t>(attachment_references.size());
		subpass_description.pColorAttachments = attachment_references.data();
		subpass_descriptions.push_back(subpass_description);

		VkRenderPassCreateInfo render_pass_create_info {};
		render_pass_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		render_pass_create_info.attachmentCount = static_cast<uint32_t>(attachment_descriptions.size());
		render_pass_create_info.pAttachments = attachment_descriptions.data();
		render_pass_create_info.subpassCount = static_cast<uint32_t>(subpass_descriptions.size());
		render_pass_create_info.pSubpasses = subpass_descriptions.data();
		render_pass_create_info.dependencyCount = static_cast<uint32_t>(subpass_dependencies.size());
		render_pass_create_info.pDependencies = subpass_dependencies.data();

		auto result = vkCreateRenderPass(device, &render_pass_create_info, nullptr, &render_pass);
		WM_ASSERT(result == VkResult::VK_SUCCESS);
	}

	void wm_vulkan_rendering_system::create_framebuffers() {
		swap_chain_framebuffers.resize(swap_chain_image_views.size());

		for(int32_t i = 0; i < static_cast<int32_t>(swap_chain_image_views.size()); i++) {
			std::vector<VkImageView> attachments = {swap_chain_image_views.at(i)};

			VkFramebufferCreateInfo framebuffer_create_info {};
			framebuffer_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebuffer_create_info.renderPass = render_pass;
			framebuffer_create_info.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebuffer_create_info.pAttachments = attachments.data();
			framebuffer_create_info.width = swap_chain_extent.width;
			framebuffer_create_info.height = swap_chain_extent.height;
			framebuffer_create_info.layers = 1;

			auto result = vkCreateFramebuffer(device, &framebuffer_create_info, nullptr, &swap_chain_framebuffers.at(i));
			WM_ASSERT(result == VkResult::VK_SUCCESS);
		}
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
		VkShaderModuleCreateInfo shader_module_create_info {};
		shader_module_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shader_module_create_info.codeSize = code.size();
		shader_module_create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shader_module;
		auto result = vkCreateShaderModule(device, &shader_module_create_info, nullptr, &shader_module);
		WM_ASSERT(result == VkResult::VK_SUCCESS);
		return shader_module;
	}


	//vertex buffer
	void wm_vulkan_rendering_system::create_vertex_buffer() {
		const size_t size = sizeof(vertices.at(0)) * vertices.size();

		VkBuffer staging_buffer;
		VkDeviceMemory staging_buffer_device_memory;
		create_buffer(size, VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer, staging_buffer_device_memory);

		void* data;
		auto result = vkMapMemory(device, staging_buffer_device_memory, 0, size, 0, &data);
		WM_ASSERT(result == VkResult::VK_SUCCESS);
		memcpy(data, vertices.data(), size);

		create_buffer(size, VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_DST_BIT | VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertex_buffer, vertex_buffer_device_memory);
		copy_buffer(staging_buffer, vertex_buffer, size);

		vkDestroyBuffer(device, staging_buffer, nullptr);
		vkFreeMemory(device, staging_buffer_device_memory, nullptr);
	}

	void wm_vulkan_rendering_system::create_index_buffer() {
		const size_t size = sizeof(indices.at(0)) * indices.size();

		VkBuffer staging_buffer;
		VkDeviceMemory staging_buffer_device_memory;
		create_buffer(size, VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer, staging_buffer_device_memory);

		void* data;
		auto result = vkMapMemory(device, staging_buffer_device_memory, 0, size, 0, &data);
		WM_ASSERT(result == VkResult::VK_SUCCESS);
		memcpy(data, indices.data(), size);

		create_buffer(size, VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_DST_BIT | VkBufferUsageFlagBits::VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, index_buffer, index_buffer_device_memory);
		copy_buffer(staging_buffer, index_buffer, size);

		vkDestroyBuffer(device, staging_buffer, nullptr);
		vkFreeMemory(device, staging_buffer_device_memory, nullptr);
	}

	void wm_vulkan_rendering_system::create_buffer(const size_t size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& device_memory) {
		VkBufferCreateInfo buffer_create_info {};
		buffer_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buffer_create_info.size = size;
		buffer_create_info.usage = usage;
		buffer_create_info.sharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;

		auto result = vkCreateBuffer(device, &buffer_create_info, nullptr, &buffer);
		WM_ASSERT(result == VkResult::VK_SUCCESS);

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

		VkMemoryAllocateInfo memory_allocation_info {};
		memory_allocation_info.sType = VkStructureType::VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memory_allocation_info.allocationSize = memory_requirements.size;
		memory_allocation_info.memoryTypeIndex = memory_type_index;

		result = vkAllocateMemory(device, &memory_allocation_info, nullptr, &device_memory);
		WM_ASSERT(result == VkResult::VK_SUCCESS);

		result = vkBindBufferMemory(device, buffer, device_memory, 0);
	}

	void wm_vulkan_rendering_system::copy_buffer(const VkBuffer source_buffer, const VkBuffer destination_buffer, const size_t size) {
		VkCommandBufferAllocateInfo command_buffer_allocation_info {};
		command_buffer_allocation_info.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		command_buffer_allocation_info.level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		command_buffer_allocation_info.commandPool = command_pool;
		command_buffer_allocation_info.commandBufferCount = 1;

		VkCommandBuffer command_buffer;
		auto result = vkAllocateCommandBuffers(device, &command_buffer_allocation_info, &command_buffer);
		WM_ASSERT(result == VkResult::VK_SUCCESS);

		VkCommandBufferBeginInfo command_buffer_begin_info {};
		command_buffer_begin_info.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		command_buffer_begin_info.flags = VkCommandBufferUsageFlagBits::VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		result = vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);
		WM_ASSERT(result == VkResult::VK_SUCCESS);

		VkBufferCopy buffer_copy {};
		buffer_copy.size = size;
		vkCmdCopyBuffer(command_buffer, source_buffer, destination_buffer, 1, &buffer_copy);

		result = vkEndCommandBuffer(command_buffer);
		WM_ASSERT(result == VkResult::VK_SUCCESS);

		VkSubmitInfo submit_info {};
		submit_info.sType = VkStructureType::VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &command_buffer;

		result = vkQueueSubmit(graphics_queue, 1, &submit_info, VK_NULL_HANDLE);
		WM_ASSERT(result == VkResult::VK_SUCCESS);
		result = vkQueueWaitIdle(graphics_queue);
		WM_ASSERT(result == VkResult::VK_SUCCESS);

		vkFreeCommandBuffers(device, command_pool, 1, &command_buffer);
	}


	//uniform buffers
	void wm_vulkan_rendering_system::create_uniform_buffers() {
		size_t buffer_size = sizeof(uniform_buffer_object);

		uniform_buffers.resize(swap_chain_images.size());
		uniform_buffers_device_memory.resize(swap_chain_images.size());

		for(size_t i = 0; i < swap_chain_images.size(); i++) {
			create_buffer(
				buffer_size,
				VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				uniform_buffers.at(i),
				uniform_buffers_device_memory.at(i)
			);
		}
	}

	void wm_vulkan_rendering_system::update_uniform_buffer(const uint32_t image_index) const {
		static const float ROTATION_SPEED = 0.001f;
		static float rotation = 0.0f;
		const float delta_time = engine::get_time_system()->get_delta_time();
		rotation += delta_time * ROTATION_SPEED;

		uniform_buffer_object ubo {};
		ubo.model = glm::rotate(glm::mat4(1.0f), rotation * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		ubo.projection = glm::perspectiveRH(glm::radians(45.0f), static_cast<float>(swap_chain_extent.width) / static_cast<float>(swap_chain_extent.height), 0.1f, 10.0f);

		void* data;
		vkMapMemory(device, uniform_buffers_device_memory.at(image_index), 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, uniform_buffers_device_memory.at(image_index));
	}

	void wm_vulkan_rendering_system::create_descriptor_pool() {
		VkDescriptorPoolSize descriptor_pool_size {};
		descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptor_pool_size.descriptorCount = static_cast<uint32_t>(swap_chain_images.size());
		std::array<VkDescriptorPoolSize, 1> descriptor_pool_sizes = {descriptor_pool_size};

		VkDescriptorPoolCreateInfo descriptor_pool_create_info {};
		descriptor_pool_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptor_pool_create_info.poolSizeCount = static_cast<uint32_t>(descriptor_pool_sizes.size());
		descriptor_pool_create_info.pPoolSizes = descriptor_pool_sizes.data();
		descriptor_pool_create_info.maxSets = static_cast<uint32_t>(swap_chain_images.size());

		auto result = vkCreateDescriptorPool(device, &descriptor_pool_create_info, nullptr, &descriptor_pool);
		WM_ASSERT(result == VkResult::VK_SUCCESS);
	}

	void wm_vulkan_rendering_system::create_descriptor_sets() {
		std::vector<VkDescriptorSetLayout> layouts(swap_chain_images.size(), descriptor_set_layout);
		VkDescriptorSetAllocateInfo descriptor_set_allocation_info {};
		descriptor_set_allocation_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptor_set_allocation_info.descriptorPool = descriptor_pool;
		descriptor_set_allocation_info.descriptorSetCount = static_cast<uint32_t>(swap_chain_images.size());
		descriptor_set_allocation_info.pSetLayouts = layouts.data();

		descriptor_sets.resize(swap_chain_images.size());
		auto result = vkAllocateDescriptorSets(device, &descriptor_set_allocation_info, descriptor_sets.data());
		WM_ASSERT(result == VkResult::VK_SUCCESS);

		for(size_t i = 0; i < swap_chain_images.size(); i++) {
			VkDescriptorBufferInfo descriptor_buffer_info {};
			descriptor_buffer_info.buffer = uniform_buffers.at(i);
			descriptor_buffer_info.offset = 0;
			descriptor_buffer_info.range = sizeof(uniform_buffer_object);

			VkWriteDescriptorSet write_descriptor_set {};
			write_descriptor_set.sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write_descriptor_set.dstSet = descriptor_sets.at(i);
			write_descriptor_set.dstBinding = 0;
			write_descriptor_set.dstArrayElement = 0;
			write_descriptor_set.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			write_descriptor_set.descriptorCount = 1;
			write_descriptor_set.pBufferInfo = &descriptor_buffer_info;
			std::array<VkWriteDescriptorSet, 1> write_descriptor_sets = {write_descriptor_set};

			vkUpdateDescriptorSets(device, write_descriptor_sets.size(), write_descriptor_sets.data(), 0, nullptr);
		}
	}


	//command buffers
	void wm_vulkan_rendering_system::create_command_pool() {
		VkCommandPoolCreateInfo command_pool_create_info {};
		command_pool_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		command_pool_create_info.queueFamilyIndex = graphics_queue_family_index;

		auto result = vkCreateCommandPool(device, &command_pool_create_info, nullptr, &command_pool);
		WM_ASSERT(result == VkResult::VK_SUCCESS);
	}

	void wm_vulkan_rendering_system::create_command_buffers() {
		command_buffers.resize(swap_chain_framebuffers.size());
		VkCommandBufferAllocateInfo command_buffer_allocation_info {};
		command_buffer_allocation_info.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		command_buffer_allocation_info.commandPool = command_pool;
		command_buffer_allocation_info.level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		command_buffer_allocation_info.commandBufferCount = static_cast<uint32_t>(command_buffers.size());

		auto result = vkAllocateCommandBuffers(device, &command_buffer_allocation_info, command_buffers.data());
		WM_ASSERT(result == VkResult::VK_SUCCESS);

		WM_LOG_INFO_2("Vulkan command buffer allocated");

		for(int32_t i = 0; i < static_cast<int32_t>(command_buffers.size()); i++) {
			VkCommandBufferBeginInfo command_buffer_begin_info {};
			command_buffer_begin_info.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			result = vkBeginCommandBuffer(command_buffers.at(i), &command_buffer_begin_info);
			WM_ASSERT(result == VkResult::VK_SUCCESS);

			VkClearValue clear_value = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

			VkRenderPassBeginInfo render_pass_begin_info {};
			render_pass_begin_info.sType = VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			render_pass_begin_info.renderPass = render_pass;
			render_pass_begin_info.framebuffer = swap_chain_framebuffers.at(i);
			render_pass_begin_info.renderArea.offset = {0, 0};
			render_pass_begin_info.renderArea.extent = swap_chain_extent;
			render_pass_begin_info.clearValueCount = 1;
			render_pass_begin_info.pClearValues = &clear_value;

			vkCmdBeginRenderPass(command_buffers.at(i), &render_pass_begin_info, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(command_buffers.at(i), VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

			std::array<VkBuffer, 1> vertex_buffers = {vertex_buffer};
			std::array<VkDeviceSize, 1> offsets = {0};
			vkCmdBindVertexBuffers(command_buffers.at(i), 0, 1, vertex_buffers.data(), offsets.data());
			vkCmdBindIndexBuffer(command_buffers.at(i), index_buffer, 0, VkIndexType::VK_INDEX_TYPE_UINT16);

			vkCmdBindDescriptorSets(command_buffers.at(i), VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout, 0, 1, &descriptor_sets.at(i), 0, nullptr);

			vkCmdDrawIndexed(command_buffers.at(i), static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

			vkCmdEndRenderPass(command_buffers.at(i));

			result = vkEndCommandBuffer(command_buffers.at(i));
			WM_ASSERT(result == VkResult::VK_SUCCESS);
		}

		WM_LOG_INFO_2("Vulkan drawing commands recorded");
	}


	//sync
	void wm_vulkan_rendering_system::create_semaphores() {
		image_is_available.resize(MAX_FRAMES_IN_FLIGHT);
		rendering_is_finished.resize(MAX_FRAMES_IN_FLIGHT);
		in_flight.resize(MAX_FRAMES_IN_FLIGHT);
		images_in_flight.resize(swap_chain_images.size(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphore_create_info {};
		semaphore_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fence_create_info {};
		fence_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fence_create_info.flags = VkFenceCreateFlagBits::VK_FENCE_CREATE_SIGNALED_BIT;

		for(int32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			auto result = vkCreateSemaphore(device, &semaphore_create_info, nullptr, &image_is_available.at(i));
			WM_ASSERT(result == VkResult::VK_SUCCESS);
			result = vkCreateSemaphore(device, &semaphore_create_info, nullptr, &rendering_is_finished.at(i));
			WM_ASSERT(result == VkResult::VK_SUCCESS);
			result = vkCreateFence(device, &fence_create_info, nullptr, &in_flight.at(i));
			WM_ASSERT(result == VkResult::VK_SUCCESS);
		}
	}


	//drawing
	void wm_vulkan_rendering_system::update() {
		if(engine::get_window_system()->get_window_count() == 0 || minimized) {
			return;
		}

		frame_index = engine::get_time_system()->get_frame_count() % MAX_FRAMES_IN_FLIGHT;
		uint32_t image_index;

		vkWaitForFences(device, 1, &in_flight.at(frame_index), VK_TRUE, UINT64_MAX);

		auto result = vkAcquireNextImageKHR(device, swap_chain, UINT64_MAX, image_is_available.at(frame_index), VK_NULL_HANDLE, &image_index);
		if(result == VkResult::VK_ERROR_OUT_OF_DATE_KHR) {
			recreate_swap_chain();
		} else if(result != VkResult::VK_SUBOPTIMAL_KHR) {
			WM_ASSERT(result == VkResult::VK_SUCCESS);
		}

		if(images_in_flight.at(image_index) != VK_NULL_HANDLE) {
			vkWaitForFences(device, 1, &images_in_flight.at(image_index), VK_TRUE, UINT64_MAX);
		}
		images_in_flight.at(image_index) = images_in_flight.at(frame_index);

		std::vector<VkSemaphore> image_is_available_semaphores = {image_is_available.at(frame_index)};
		std::vector<VkPipelineStageFlags> pipeline_stages = {VkPipelineStageFlagBits::VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		std::vector<VkSemaphore> rendering_is_finished_semaphores = {rendering_is_finished.at(frame_index)};

		update_uniform_buffer(image_index);

		VkSubmitInfo submit_info {};
		submit_info.sType = VkStructureType::VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.waitSemaphoreCount = static_cast<uint32_t>(image_is_available_semaphores.size());
		submit_info.pWaitSemaphores = image_is_available_semaphores.data();
		submit_info.pWaitDstStageMask = pipeline_stages.data();
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &command_buffers.at(image_index);
		submit_info.signalSemaphoreCount = static_cast<uint32_t>(rendering_is_finished_semaphores.size());
		submit_info.pSignalSemaphores = rendering_is_finished_semaphores.data();

		vkResetFences(device, 1, &in_flight.at(frame_index));

		result = vkQueueSubmit(graphics_queue, 1, &submit_info, in_flight.at(frame_index));
		WM_ASSERT(result == VkResult::VK_SUCCESS);

		std::vector<VkSwapchainKHR> swap_chains = {swap_chain};

		VkPresentInfoKHR present_info {};
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
			WM_ASSERT(result == VkResult::VK_SUCCESS);
		}

		WM_LOG_INFO_3("vulkan rendering system updated");
	}

	wm_vulkan_rendering_system::~wm_vulkan_rendering_system() {
		auto result = vkDeviceWaitIdle(device);
		WM_ASSERT(result == VkResult::VK_SUCCESS);

		for(int32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(device, image_is_available.at(i), nullptr);
			vkDestroySemaphore(device, rendering_is_finished.at(i), nullptr);
			vkDestroyFence(device, in_flight.at(i), nullptr);
		}

		cleanup_swap_chain();

		vkDestroyDescriptorSetLayout(device, descriptor_set_layout, nullptr);

		vkDestroyBuffer(device, index_buffer, nullptr);
		vkFreeMemory(device, index_buffer_device_memory, nullptr);
		WM_LOG_INFO_2("Vulkan index buffer destroyed");

		vkDestroyBuffer(device, vertex_buffer, nullptr);
		vkFreeMemory(device, vertex_buffer_device_memory, nullptr);
		WM_LOG_INFO_2("Vulkan vertex buffer destroyed");

		vkDestroyCommandPool(device, command_pool, nullptr);
		WM_LOG_INFO_2("Vulkan command pool destroyed");

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

		WM_LOG_INFO_1("vulkan rendering system destructed");
	}

}
