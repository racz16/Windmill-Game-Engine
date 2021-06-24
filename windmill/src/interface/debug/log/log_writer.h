#pragma once

#include "../../defines.h"
#include "log_level.h"

namespace wm {

	class WM_PUBLIC log_writer {
	public:
		static std::shared_ptr<log_writer> create_console_log_writer();
		//TODO: file log writer, létrehozáskor megadni a fájl útvonalát
		//TODO: imgui console log writer
		//TODO: force flush
		virtual log_level get_max_log_level() = 0;
		virtual void set_max_log_level(const log_level level) = 0;
		virtual void log_message(const log_level level, const std::string& message, const std::string& function, const uint32_t line, const std::string& log_source) = 0;
		virtual ~log_writer() { }
	};

}
