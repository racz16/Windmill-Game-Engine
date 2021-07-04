#include "engine.h"
#include "../wm_engine.h"

namespace wm {

	std::shared_ptr<engine> engine::get_instance() {
		static auto instance = std::make_shared<wm_engine>();
		return instance;
	}

	std::shared_ptr<log_system> engine::get_log_system() {
		static auto log_system = log_system::get_instance();
		if(log_system->get_log_writer_count() == 0) {
		#ifdef WM_BUILD_DEBUG
			auto clw = log_writer::create_console_log_writer();
			log_system->add_log_writer(clw);
			auto flw = log_writer::create_file_log_writer(log_level::Info_3);
			log_system->add_log_writer(flw);
		#else
			auto flw = log_writer::create_file_log_writer(log_level::Error);
			log_system->add_log_writer(flw);
		#endif
		}
		return log_system;
	}

	std::shared_ptr<window_system> engine::get_window_system() {
		static auto window_system = window_system::get_instance();
		return window_system;
	}

}
