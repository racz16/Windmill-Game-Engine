#pragma once

#include "general_defines.h"
#include "../core/windmill_error.h"

//	WM_BREAKPOINT			compiler specific debugger breakpoint
//	WM_THROW_ERROR			throws an exception
//	WM_ASSERT				throws an exception if the given expression evaluates to false
//	WM_ASSERT				throws an exception if the given expression doesn't evaluate to VK_SUCCESS

#ifdef WM_BUILD_DEBUG
#	ifdef WM_PLATFORM_WINDOWS
#		define WM_BREAKPOINT() __debugbreak()
#	else
#		define WM_BREAKPOINT() __builtin_trap()
#	endif
#else
#	define WM_BREAKPOINT()
#endif



#define WM_THROW_ERROR(message) throw wm::windmill_error(message, __FUNCTION__, __LINE__)



#ifdef WM_BUILD_DEBUG

#	define WM_ASSERT(expression) \
		if(!(expression)) {\
			WM_BREAKPOINT();\
			WM_THROW_ERROR(std::string(std::string("ASSERT FAILED!\r\ncondition: ") + #expression));\
		}

#	define WM_ASSERT_VULKAN(expression) WM_ASSERT(expression == VkResult::VK_SUCCESS);

#else
#	define WM_ASSERT(expression)
#	define WM_ASSERT_VULKAN(expression) expression
#endif
