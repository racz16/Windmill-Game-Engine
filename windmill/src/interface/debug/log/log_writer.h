#pragma once

#include "../../core/defines.h"
#include "../../core/ptr.h"
#include "log_level.h"

namespace wm {

	class WM_PUBLIC log_writer {
	public:
		static ptr<log_writer> create_console_log_writer(const log_level max_log_level = log_level::debug);
		static ptr<log_writer> create_file_log_writer(const log_level max_log_level = log_level::debug, const std::string& path = "log");

		virtual log_level get_max_log_level() const = 0;
		virtual void set_max_log_level(const log_level level) = 0;
		virtual bool is_force_flush() const = 0;
		virtual void set_force_flush(const bool force_flush) = 0;
		virtual void log_message(const log_level level, const std::string& message, const std::string& function, const int32_t line, const std::string& log_source) = 0;
		virtual ~log_writer() { }
	};

}
