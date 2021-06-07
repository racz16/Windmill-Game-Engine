#pragma once

#if defined(_MSC_VER)
	#define EXPORT_KEYWORD __declspec(dllexport)
	#define IMPORT_KEYWORD __declspec(dllimport)
#elif defined(__GNUC__)
	#define EXPORT_KEYWORD __attribute__((visibility("default")))
	#define IMPORT_KEYWORD
#else
	#define EXPORT_KEYWORD
	#define IMPORT_KEYWORD
	#pragma warning Unknown dynamic link import/export semantics.
#endif

#if EXPORT_WINDMILL
#   define WM_PUBLIC EXPORT_KEYWORD
#else
#   define WM_PUBLIC IMPORT_KEYWORD
#endif