#pragma once

#include "defines.h"
#ifdef WM_PLATFORM_WINDOWS
	#include "windows.h"
#endif
#include "debug/log/log_level.h"
#include "wm_base_log_writer.h"

namespace wm {

	class wm_console_log_writer : public wm_base_log_writer {
	private:
		std::string entry;
	public:
		wm_console_log_writer(const log_level max_log_level = log_level::Debug);
		void log_message(const log_level level, const std::string& message, const std::string& function, const uint32_t line, const std::string& log_source) override;
	};

}