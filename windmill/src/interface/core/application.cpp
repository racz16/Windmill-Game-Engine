#include "application.h"
#include "engine.h"
#include "../defines/log_defines.h"

namespace wm {

	application::application(const std::string& name) {
		engine::get_parameters().set(engine::get_app_name_key(), name);
	}

	void application::run() {
		try {
			initialize();
			const auto name = engine::get_app_name();
			WM_LOG_INFO_1(name + " started");
			loop();
			WM_LOG_INFO_1(name + " stopped");
			destroy();
		} catch(const windmill_error& ex) {
			WM_LOG_ERROR(ex.what(), ex.get_function(), ex.get_line());
			WM_BREAKPOINT();
		} catch(const std::exception& ex) {
			WM_LOG_ERROR(ex.what(), __FUNCTION__, __LINE__);
			WM_BREAKPOINT();
		} catch(...) {
			WM_LOG_ERROR("Unknown exception", __FUNCTION__, __LINE__);
			WM_BREAKPOINT();
		}
	}

	void application::initialize() {
		add_log_system();
		add_resource_system();
		add_event_system();
		add_time_system();
		add_window_system();
		add_rendering_system();
	}

	void application::add_log_system() {
		const auto log_system = log_system::create();
	#ifdef WM_BUILD_DEBUG
		const auto clw = log_writer::create_console_log_writer();
		log_system->add_log_writer(clw);
		const auto flw = log_writer::create_file_log_writer(log_level::info_3);
		log_system->add_log_writer(flw);
	#else
		const auto flw = log_writer::create_file_log_writer(log_level::error);
		log_system->add_log_writer(flw);
	#endif
		engine::set_system(log_system::get_key(), log_system);
		WM_LOG_INFO_1("log system added");
	}

	void application::add_event_system() {
		const auto event_system = event_system::create();
		engine::set_system(event_system::get_key(), event_system);
		WM_LOG_INFO_1("event system added");
	}

	void application::add_time_system() {
		const auto time_system = time_system::create();
		engine::set_system(time_system::get_key(), time_system);
		WM_LOG_INFO_1("time system added");
	}

	void application::add_window_system() {
		const auto name = engine::get_app_name();
		const auto window_system = window_system::get_instance();
		window_system->create_window(glm::ivec2(640, 480), name, false);
		engine::set_system(window_system::get_key(), window_system);
		WM_LOG_INFO_1("window system added");
	}

	void application::add_rendering_system() {
		const auto rendering_system = rendering_system::create();
		engine::set_system(rendering_system::get_key(), rendering_system);
		WM_LOG_INFO_1("rendering system added");
	}

	void application::add_resource_system() {
		const auto resource_system = resource_system::create();
		engine::set_system(resource_system::get_key(), resource_system);
		WM_LOG_INFO_1("resource system added");
	}

	bool application::loop_condition() {
		return engine::get_window_system()->get_window_count() > 0;
	}

	void application::loop() {
		while(loop_condition()) {
			engine::update_systems();
		}
	}

	void application::destroy() {
		engine::destroy();
	}

}
