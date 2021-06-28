#include "log_writer.h"
#include "../../../debug/log/wm_console_log_writer.h"
#include "../../../debug/log/wm_file_log_writer.h"

namespace wm {

	std::shared_ptr<log_writer> log_writer::create_console_log_writer(const log_level max_log_level) {
		return std::make_shared<wm_console_log_writer>(max_log_level);
	}

	std::shared_ptr<log_writer> log_writer::create_file_log_writer(const log_level max_log_level, const std::string& path) {
		return std::make_shared<wm_file_log_writer>(max_log_level, path);
	}

}
