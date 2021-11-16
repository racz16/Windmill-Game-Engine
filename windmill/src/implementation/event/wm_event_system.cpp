#include "core/engine.h"

#include "wm_event_system.h"

namespace wm {

	wm_event_system::wm_event_system() {
		WM_LOG_INFO_1("wm event system constructed");
	}

	void wm_event_system::add_event_listener(const int32_t key, const int32_t event_listener_id, const std::any event_listener) {
		if(!has_event_listener(key, event_listener_id)) {
			if(!has_key(key)) {
				std::vector<std::pair<int32_t, std::any>> event_listeners_vector;
				event_listeners.insert_or_assign(key, event_listeners_vector);
			}
			auto& event_listeners_vector = event_listeners.at(key);
			auto a = std::pair<int32_t, std::any>(event_listener_id, event_listener);
			event_listeners_vector.push_back(a);
		}
	}

	bool wm_event_system::has_event_listener(const int32_t key, const int32_t event_listener_id) {
		if(has_key(key)) {
			for(auto el : get_event_listeners(key)) {
				if(el.first == event_listener_id) {
					return true;
				}
			}
		}
		return false;
	}

	bool wm_event_system::has_key(const int32_t key) {
		return event_listeners.find(key) != event_listeners.end();
	}

	void wm_event_system::remove_event_listener(const int32_t key, const int32_t event_listener_id) {
		if(has_event_listener(key, event_listener_id)) {
			auto& event_listeners_vector = event_listeners.at(key);
			for(int32_t i = 0; i < event_listeners_vector.size(); i++) {
				if(event_listeners_vector.at(i).first == event_listener_id) {
					event_listeners_vector.erase(event_listeners_vector.begin() + i);
					if(event_listeners_vector.size() == 0) {
						event_listeners.erase(key);
					}
					return;
				}
			}
		}
	}

	const std::vector<std::pair<int32_t, std::any>>& wm_event_system::get_event_listeners(const int32_t key) const {
		return event_listeners.at(key);
	}

	wm_event_system::~wm_event_system() {
		WM_LOG_INFO_1("wm event system destructed");
	}

}
