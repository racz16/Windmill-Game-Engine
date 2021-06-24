#include "engine.h"
#include "../wm_engine.h"

namespace wm {

	std::shared_ptr<engine> engine::create() {
		return std::make_shared<wm_engine>();
	}

	std::shared_ptr<log_system> engine::get_log_system() {
		static auto global_log = log_system::create();
		if(global_log->get_log_writer_count() == 0) {
			auto clw = log_writer::create_console_log_writer();
			global_log->add_log_writer(clw);
		}
		return global_log;
	}

}
