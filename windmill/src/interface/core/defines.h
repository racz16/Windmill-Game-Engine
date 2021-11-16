#pragma once

#include <stdexcept>

//POSSIBLE DEFINES
//
//	WM_BUILD_RELEASE		defined if you build in release mode
//	WM_BUILD_DEBUG			defined if you build in debug mode
// 
//	WM_PLATFORM_WINDOWS		defined if the operating system is Windows
//	WM_PLATFORM_LINUX		defined if the operating system is Linux
//  WM_PLATFORM_NAME		the name of the operating system
// 
//	WM_COMPILER_MSVC		defined if the c++ compiler is MSVC
//	WM_COMPILER_GCC			defined if the c++ compiler is GCC
//	WM_COMPILER_UNKNOWN		defined if the c++ compiler is unknown
// 
//  WM_EXPORT				defined if you build the Windmill Engine and not an application
//	WM_EXPORT_KEYWORD		compiler specific attribute for exporting classes, functions etc.
//	WM_IMPORT_KEYWORD		compiler specific attribute for importing classes, functions etc.
//	WM_PUBLIC				for exposing the public API, one of WM_EXPORT_KEYWORD or WM_IMPORT_KEYWORD, depending on WM_EXPORT
//
//	WM_LOG_SOURCE			defines the string "WINDMILL" if WM_EXPORT is defined, "APPLICATION" otherwise
//	WM_LOG_INFO_3			for logging less important informations (only in debug mode)
//	WM_LOG_INFO_2			for logging informations (only in debug mode)
//	WM_LOG_INFO_1			for logging more important informations (only in debug mode)
//	WM_LOG_DEBUG			for logging debug messages (only in debug mode)
//	WM_LOG_WARNING			for logging warning messages (only in debug mode)
//  WM_LOG_WARNING_IF		for logging warning messages if the given expression evaluates to true (only in debug mode)
//	WM_LOG_ERROR			for logging error messages
//
//	WM_BREAKPOINT			compiler specific debugger breakpoint
//
//	WM_ASSERT				throws an exception if the given expression evaluates to false

//PLATFORM
#ifdef _WIN32
#	ifdef _WIN64
#		define WM_PLATFORM_WINDOWS
#		define WM_PLATFORM_NAME "WINDOWS"
#	else
#		error "Windows x86 is not supported"
#	endif
#elif __linux__
#	define WM_PLATFORM_LINUX
#	define WM_PLATFORM_NAME "LINUX"
#else
#	error "Not supported platform"
#endif

//COMPILER
#ifdef _MSC_VER
#	define WM_COMPILER_MSVC
#	define WM_EXPORT_KEYWORD __declspec(dllexport)
#	define WM_IMPORT_KEYWORD __declspec(dllimport)
#elif __GNUC__
#	define WM_COMPILER_GCC
#	define WM_EXPORT_KEYWORD __attribute__((visibility("default")))
#	define WM_IMPORT_KEYWORD
#else
#	define WM_COMPILER_UNKNOWN
#	define WM_EXPORT_KEYWORD
#	define WM_IMPORT_KEYWORD
#pragma warning "Unknown dynamic link import/export semantics"
#endif

//IMPORT-EXPORT
#ifdef WM_EXPORT
#   define WM_PUBLIC WM_EXPORT_KEYWORD
#else
#   define WM_PUBLIC WM_IMPORT_KEYWORD
#endif

//LOGGING
#ifdef WM_EXPORT
#	define WM_LOG_SOURCE "WINDMILL"
#else
#	define WM_LOG_SOURCE "APPLICATION"
#endif
#ifdef WM_BUILD_DEBUG
#	define WM_LOG_INFO_3(message) wm::engine::get_log_system()->log_messaage(wm::log_level::info_3, message, __FUNCTION__, __LINE__, WM_LOG_SOURCE)
#	define WM_LOG_INFO_2(message) wm::engine::get_log_system()->log_messaage(wm::log_level::info_2, message,  __FUNCTION__, __LINE__, WM_LOG_SOURCE)
#	define WM_LOG_INFO_1(message) wm::engine::get_log_system()->log_messaage(wm::log_level::info_1, message,  __FUNCTION__, __LINE__, WM_LOG_SOURCE)
#	define WM_LOG_DEBUG(message) wm::engine::get_log_system()->log_messaage(wm::log_level::debug, message,  __FUNCTION__, __LINE__, WM_LOG_SOURCE)
#	define WM_LOG_WARNING(message) wm::engine::get_log_system()->log_messaage(wm::log_level::warning, message,  __FUNCTION__, __LINE__, WM_LOG_SOURCE)
#	define WM_LOG_WARNING_IF(condition, message) \
		if(condition) { \
			WM_LOG_WARNING(message); \
		}
#else
#	define WM_LOG_INFO_3(message)
#	define WM_LOG_INFO_2(message)
#	define WM_LOG_INFO_1(message)
#	define WM_LOG_DEBUG(message)
#	define WM_LOG_WARNING(message)
#	define WM_LOG_WARNING_IF(condition, message)
#endif
#define WM_LOG_ERROR(message) wm::engine::get_log_system()->log_messaage(wm::log_level::error, message,  __FUNCTION__, __LINE__, WM_LOG_SOURCE)

//DEBUG
#ifdef WM_BUILD_DEBUG
#	ifdef WM_PLATFORM_WINDOWS
#		define WM_BREAKPOINT() __debugbreak()
#	else
#		define WM_BREAKPOINT() __builtin_trap()
#	endif
#else
#	define WM_BREAKPOINT()
#endif 

//ASSERT
#ifdef WM_BUILD_DEBUG
#	define WM_ASSERT(expression) \
		if(!(expression)) {\
			throw std::runtime_error(std::string("assert failed: ") + std::string(#expression));\
		}
#else
#	define WM_ASSERT(expression)
#endif

//https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4251
//C4251 can be ignored if your class is derived from a type in the C++ Standard Library, you're compiling a
//debug release (/MTd), and where the compiler error message refers to _Container_base.
#pragma warning(disable: 4251)
