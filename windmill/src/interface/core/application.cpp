#include <glm/vec2.hpp>

#include "application.h"
#include "engine.h"
#include "time_system.h"

namespace wm {

	application::application(const std::string& name): name(name) { }

	void application::run() {
		try {
			initialize();
			WM_LOG_INFO_1(name + " started");
			loop();
			WM_LOG_INFO_1(name + " stopped");
			destroy();
		} catch(const std::exception& ex) {
			WM_LOG_ERROR(ex.what());
			WM_BREAKPOINT();
		} catch(...) {
			WM_LOG_ERROR("Unknown exception");
			WM_BREAKPOINT();
		}
	}

	void application::initialize() {
		add_log_system();
		add_time_system();
		add_window_system();
	}

	void application::add_log_system() {
		const auto log_system = log_system::create();
	#ifdef WM_BUILD_DEBUG
		const auto clw = log_writer::create_console_log_writer();
		log_system->add_log_writer(clw);
		const auto flw = log_writer::create_file_log_writer(log_level::Info_3);
		log_system->add_log_writer(flw);
	#else
		const auto flw = log_writer::create_file_log_writer(log_level::Error);
		log_system->add_log_writer(flw);
	#endif
		engine::set_system(log_system::get_key(), log_system);
		WM_LOG_INFO_1("log system added");
	}

	void application::add_time_system() {
		const auto time_system = time_system::create();
		engine::set_system(time_system::get_key(), time_system);
		WM_LOG_INFO_1("time system added");
	}

	void application::add_window_system() {
		const auto window_system = window_system::get_instance();
		window_system->create_window(glm::ivec2(640, 480), name, false);
		engine::set_system(window_system::get_key(), window_system);
		WM_LOG_INFO_1("window system added");
	}

	void application::loop() {
		while(engine::get_window_system()->get_window_count() > 0) {
			engine::update_systems();
		}
	}

	void application::destroy() {
		engine::destroy();
	}

}
