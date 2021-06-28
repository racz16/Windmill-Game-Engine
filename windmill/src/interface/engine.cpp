#include "engine.h"
#include "../wm_engine.h"

namespace wm {

	std::shared_ptr<engine> engine::create() {
		return std::make_shared<wm_engine>();
	}

	std::shared_ptr<log_system> engine::get_log_system() {
		static auto global_log = log_system::create();
		if(global_log->get_log_writer_count() == 0) {
		#ifdef WM_BUILD_DEBUG
			auto clw = log_writer::create_console_log_writer();
			global_log->add_log_writer(clw);
			auto flw = log_writer::create_file_log_writer(log_level::Info_3);
			global_log->add_log_writer(flw);
		#else
			auto flw = log_writer::create_file_log_writer(log_level::Error);
			global_log->add_log_writer(flw);
		#endif
		}
		return global_log;
	}

}
