#pragma once

#include "defines/general_defines.h"

#include "wm_vulkan_rendering_context.h"

namespace wm {

#define VK_ID_OF(object, type) std::any_cast<type>(object->get_native_handle())

#ifdef WM_BUILD_DEBUG
	#define VK_CALL(expression) WM_ASSERT(expression == VkResult::VK_SUCCESS)
	#define VK_LABEL(type, object, name) set_object_label(type, reinterpret_cast<uint64_t>(object), name)
	#define VK_LABEL_2(object, label) object->set_debug_label(label)
	#define VK_GROUP_START(name, command_buffer) wm_vulkan_rendering_context::start_group(name, command_buffer)
	#define VK_GROUP_STOP(command_buffer) wm_vulkan_rendering_context::stop_group(command_buffer)
	#define VK_MARKER(name, command_buffer) wm_vulkan_rendering_context::set_marker(name, command_buffer)
#else
	#define VK_CALL(expression) expression
	#define VK_LABEL(type, object, name)
	#define VK_LABEL_2(object, label)
	#define VK_GROUP_START(name, command_buffer)
	#define VK_GROUP_STOP(command_buffer)
	#define VK_MARKER(name, command_buffer)
#endif

}
