#pragma once

#include "debug/log/log_level.h"

#include "wm_base_log_writer.h"

namespace wm {

	class wm_console_log_writer: public wm_base_log_writer {
	private:
		std::string entry;
	public:
		wm_console_log_writer(const log_level max_log_level = log_level::debug);
		void log_message(const log_level level, const std::string& message, const std::string& function, const int32_t line, const std::string& log_source) override;
	};

}
