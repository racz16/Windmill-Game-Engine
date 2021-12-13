#pragma once

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



//https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4251
//C4251 can be ignored if your class is derived from a type in the C++ Standard Library, you're compiling a debug release (/MTd), and where the compiler error message refers to _Container_base.
#pragma warning(disable: 4251)
