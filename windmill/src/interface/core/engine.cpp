#include "engine.h"

namespace wm {

	std::unordered_map<int32_t, ptr<system>> engine::systems;
	std::vector<std::pair<int32_t, std::string>> engine::system_orders;
	parameter_container engine::parameters;
	const std::string engine::engine_name = "Windmill Game Engine";
	const glm::ivec3 engine::engine_version = glm::ivec3(0, 0, 1);

	engine::engine() { }

	int32_t engine::index_of_order(const int32_t hash) {
		for(int32_t i = 0; i < system_orders.size(); i++) {
			if(system_orders.at(i).first == hash) {
				return i;
			}
		}
		return -1;
	}

	void engine::update_systems() {
		for(int32_t i = 0; i < system_orders.size(); i++) {
			const auto key = system_orders.at(i).first;
			const auto system = systems.at(key);
			if(system->is_active()) {
				system->update();
			}
		}
	}

	ptr<log_system> engine::get_log_system() {
		return get_system<log_system>(log_system::get_key());
	}

	ptr<array_allocator_system> engine::get_array_allocator_system() {
		return get_system<array_allocator_system>(array_allocator_system::get_key());
	}

	ptr<resource_system> engine::get_resource_system() {
		return get_system<resource_system>(resource_system::get_key());
	}

	ptr<event_system> engine::get_event_system() {
		return get_system<event_system>(event_system::get_key());
	}

	ptr<window_system> engine::get_window_system() {
		return get_system<window_system>(window_system::get_key());
	}

	ptr<time_system> engine::get_time_system() {
		return get_system<time_system>(time_system::get_key());
	}

	ptr<scene_system> engine::get_scene_system() {
		return get_system<scene_system>(scene_system::get_key());
	}

	ptr<rendering_system> engine::get_rendering_system() {
		return get_system<rendering_system>(rendering_system::get_key());
	}

	void engine::destroy() {
		for(int32_t i = static_cast<int32_t>(system_orders.size()) - 1; i >= 0; i--) {
			const auto key = system_orders.at(i).first;
			auto system = systems.at(key);
			system.destroy();
		}
		systems.clear();
		system_orders.clear();
	}

	parameter_container& engine::get_parameters() {
		return parameters;
	}

	key<std::string> engine::get_app_name_key() {
		static const key<std::string> key("WM_APP_NAME");
		return key;
	}

	key<glm::ivec3> engine::get_app_version_key() {
		static const key<glm::ivec3> key("WM_APP_VERSION");
		return key;
	}

	std::string engine::get_engine_name() {
		return engine_name;
	}

	glm::ivec3 engine::get_engine_version() {
		return engine_version;
	}

	std::string engine::get_app_name() {
		return engine::get_parameters().get_or_default<std::string>(engine::get_app_name_key(), "Windmill Test Application");
	}

	glm::ivec3 engine::get_app_version() {
		return engine::get_parameters().get_or_default<glm::ivec3>(engine::get_app_version_key(), glm::ivec3(1, 0, 0));
	}

}
