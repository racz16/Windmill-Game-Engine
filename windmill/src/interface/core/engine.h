#pragma once

#include "parameter_container.h"
#include "../log/log_system.h"
#include "../ptr/array_allocator_system.h"
#include "../resource/resource_system.h"
#include "../window/window_system.h"
#include "../time/time_system.h"
#include "../event/event_system.h"
#include "../scene/scene_system.h"
#include "../rendering/rendering_system.h"

namespace wm {

	class WM_PUBLIC engine {
	private:
		static std::unordered_map<int32_t, ptr<system>> systems;
		static std::vector<std::pair<int32_t, std::string>> system_orders;
		static parameter_container parameters;
		const static std::string engine_name;
		const static glm::ivec3 engine_version;

		static int32_t index_of_order(const int32_t hash);

		engine();
	public:

		template<class T>
		static bool contains_system(const key<T> key) {
			return systems.find(key.get_hash()) != systems.end();
		}

		template<class T>
		static ptr<T> get_system(const key<T> key) {
			return systems.at(key.get_hash()).template convert<T>();
		}

		template<class T>
		static void set_system(const key<T> key, ptr<T> system) {
			WM_ASSERT(system.is_valid());
			if(index_of_order(key.get_hash()) == -1) {
				system_orders.push_back(std::pair<int32_t, std::string>(key.get_hash(), key.get_name()));
			}
			systems.insert_or_assign(key.get_hash(), system.template convert<wm::system>());
		}

		template<class T>
		static void remove_system(const key<T> key) {
			if(contains_system(key)) {
				systems.erase(key.get_hash());
				const auto index = index_of_order(key.get_hash());
				system_orders.erase(system_orders.begin() + index);
			}
		}

		static void update_systems();
		static void destroy();

		static ptr<log_system> get_log_system();
		static ptr<array_allocator_system> get_array_allocator_system();
		static ptr<resource_system> get_resource_system();
		static ptr<event_system> get_event_system();
		static ptr<window_system> get_window_system();
		static ptr<time_system> get_time_system();
		static ptr<scene_system> get_scene_system();
		static ptr<rendering_system> get_rendering_system();

		static parameter_container& get_parameters();

		static key<std::string> get_app_name_key();
		static key<glm::ivec3> get_app_version_key();

		static std::string get_engine_name();
		static glm::ivec3 get_engine_version();
		static std::string get_app_name();
		static glm::ivec3 get_app_version();
	};

}
