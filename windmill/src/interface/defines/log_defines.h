#pragma once

#include "../core/engine.h"

//	WM_LOG_SOURCE			defines the string "WINDMILL" if WM_EXPORT is defined, "APPLICATION" otherwise
//	WM_LOG_INFO_3			for logging less important informations (only in debug mode)
//	WM_LOG_INFO_2			for logging informations (only in debug mode)
//	WM_LOG_INFO_1			for logging more important informations (only in debug mode)
//	WM_LOG_DEBUG			for logging debug messages (only in debug mode)
//	WM_LOG_WARNING			for logging warning messages (only in debug mode)
//  WM_LOG_WARNING_IF		for logging warning messages if the given expression evaluates to true (only in debug mode)
//	WM_LOG_ERROR			for logging error messages

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
#define WM_LOG_ERROR(message, function, line) wm::engine::get_log_system()->log_messaage(wm::log_level::error, message, function, line, WM_LOG_SOURCE)
