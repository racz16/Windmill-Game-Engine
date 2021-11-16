#include "log_writer.h"

#include "../../implementation/log/wm_console_log_writer.h"
#include "../../implementation/log/wm_file_log_writer.h"

namespace wm {

	ptr<log_writer> log_writer::create_console_log_writer(const log_level max_log_level) {
		return ptr<log_writer>(new wm_console_log_writer(max_log_level));
	}

	ptr<log_writer> log_writer::create_file_log_writer(const log_level max_log_level, const std::string& path) {
		return ptr<log_writer>(new wm_file_log_writer(max_log_level, path));
	}

}
