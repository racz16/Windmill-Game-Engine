#include "engine.h"

namespace wm {

	std::unordered_map<int32_t, ptr<system>> engine::systems;
	std::vector<std::pair<int32_t, std::string>> engine::order;
	parameter_container engine::parameters;

	engine::engine() { }

	int32_t engine::index_of_order(const int32_t hash) {
		for(int32_t i = 0; i < order.size(); i++) {
			if(order.at(i).first == hash) {
				return i;
			}
		}
		return -1;
	}

	void engine::update_systems() {
		for(int32_t i = 0; i < order.size(); i++) {
			const auto key = order.at(i).first;
			const auto system = systems.at(key);
			WM_ASSERT(system.is_valid());
			if(system->is_active()) {
				system->update();
			}
		}
	}

	ptr<log_system> engine::get_log_system() {
		return get_system<log_system>(log_system::get_key());
	}

	ptr<window_system> engine::get_window_system() {
		return get_system<window_system>(window_system::get_key());
	}

	ptr<time_system> engine::get_time_system() {
		return get_system<time_system>(time_system::get_key());
	}

	void engine::destroy() {
		for(int32_t i = static_cast<int32_t>(order.size()) - 1; i >= 0; i--) {
			const auto key = order.at(i).first;
			auto system = systems.at(key);
			system.destroy();
		}
		systems.clear();
		order.clear();
	}

	parameter_container& engine::get_parameters() {
		return parameters;
	}

	key<std::string> engine::get_app_name_key() {
		static const key<std::string> key("WM_APP_NAME");
		return key;
	}

	std::string engine::get_app_name() {
		return engine::get_parameters().get<std::string>(engine::get_app_name_key());
	}

}
