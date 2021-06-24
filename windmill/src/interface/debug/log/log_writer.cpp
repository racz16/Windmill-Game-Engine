#include "log_writer.h"
#include "../../../debug/log/wm_console_log_writer.h"

namespace wm {

	std::shared_ptr<log_writer> log_writer::create_console_log_writer() {
		return std::make_shared<wm_console_log_writer>(log_level::Info_1);
	}

}
