#include "defines/log_defines.h"

#include "wm_event_system.h"

namespace wm {

	wm_event_system::wm_event_system() {
		WM_LOG_INFO_1("event system created");
	}

	void wm_event_system::add_event_listener_impl(const int32_t key, const ptr<event_listener<event>> event_listener) {
		if(!contains_event_listener_impl(key, event_listener)) {
			if(!contains_key(key)) {
				std::vector<ptr<wm::event_listener<event>>> event_listeners_vector;
				global_event_listeners.insert_or_assign(key, event_listeners_vector);
			}
			auto& event_listeners_vector = global_event_listeners.at(key);
			event_listeners_vector.push_back(event_listener);
			WM_LOG_INFO_3("event listener added");
		}
	}

	void wm_event_system::add_event_listener_impl(const int32_t key, const ptr<event_listener<event>> event_listener, const ptr_view<void> source) {
		if(!contains_event_listener_impl(key, event_listener, source)) {
			if(!contains_key(key, source)) {
				std::unordered_map<ptr_view<void>, std::vector<ptr<wm::event_listener<event>>>> source_level_map;
				local_event_listeners.insert_or_assign(key, source_level_map);
			}
			auto& source_level_map = local_event_listeners.at(key);
			if(!contains_source(key, source)) {
				std::vector<ptr<wm::event_listener<event>>> event_listeners_vector;
				source_level_map.insert_or_assign(source, event_listeners_vector);
			}
			auto& event_listeners_vector = source_level_map.at(source);
			event_listeners_vector.push_back(event_listener);
			WM_LOG_INFO_3("event listener added");
		}
	}

	bool wm_event_system::contains_event_listener_impl(const int32_t key, const ptr<event_listener<event>> event_listener) const {
		if(contains_key(key)) {
			for(auto& el : get_event_listeners(key)) {
				if(el == event_listener) {
					return true;
				}
			}
		}
		return false;
	}

	bool wm_event_system::contains_event_listener_impl(const int32_t key, const ptr<event_listener<event>> event_listener, const ptr_view<void> source) const {
		if(contains_source(key, source)) {
			for(auto& el : get_event_listeners(key, source)) {
				if(el == event_listener) {
					return true;
				}
			}
		}
		return false;
	}

	bool wm_event_system::contains_key(const int32_t key) const {
		return global_event_listeners.find(key) != global_event_listeners.end();
	}

	bool wm_event_system::contains_key(const int32_t key, const ptr_view<void> source) const {
		return local_event_listeners.find(key) != local_event_listeners.end();
	}

	bool wm_event_system::contains_source(const int32_t key, const ptr_view<void> source) const {
		return contains_key(key, source) ? local_event_listeners.at(key).find(source) != local_event_listeners.at(key).end() : false;
	}

	void wm_event_system::remove_event_listener_impl(const int32_t key, const ptr<event_listener<event>> event_listener) {
		if(contains_event_listener_impl(key, event_listener)) {
			auto& event_listeners_vector = global_event_listeners.at(key);
			for(int32_t i = 0; i < event_listeners_vector.size(); i++) {
				if(event_listeners_vector.at(i) == event_listener) {
					event_listeners_vector.erase(event_listeners_vector.begin() + i);
					if(event_listeners_vector.empty()) {
						global_event_listeners.erase(key);
					}
					WM_LOG_INFO_3("event listener removed");
					return;
				}
			}
		}
	}

	void wm_event_system::remove_event_listener_impl(const int32_t key, const ptr<event_listener<event>> event_listener, const ptr_view<void> source) {
		if(contains_event_listener_impl(key, event_listener, source)) {
			auto& event_listeners_vector = local_event_listeners.at(key).at(source);
			for(int32_t i = 0; i < event_listeners_vector.size(); i++) {
				if(event_listeners_vector.at(i) == event_listener) {
					event_listeners_vector.erase(event_listeners_vector.begin() + i);
					if(event_listeners_vector.empty()) {
						local_event_listeners.at(key).erase(source);
						if(local_event_listeners.at(key).empty()) {
							local_event_listeners.erase(key);
						}
					}
					WM_LOG_INFO_3("event listener removed");
					return;
				}
			}
		}
	}

	const std::vector<ptr<event_listener<event>>>& wm_event_system::get_event_listeners(const int32_t key) const {
		return global_event_listeners.at(key);
	}

	const std::vector<ptr<event_listener<event>>>& wm_event_system::get_event_listeners(const int32_t key, const ptr_view<void> source) const {
		return local_event_listeners.at(key).at(source);
	}

	std::vector<ptr<event_listener<event>>> wm_event_system::get_event_listeners() const {
		std::vector<ptr<event_listener<event>>> result{};
		for(auto& event_listeners : global_event_listeners) {
			for(auto event_listener : event_listeners.second) {
				result.push_back(event_listener);
			}
		}
		for(auto& source_event_listeners : local_event_listeners) {
			for(auto& event_listeners : source_event_listeners.second) {
				for(auto event_listener : event_listeners.second) {
					result.push_back(event_listener);
				}
			}
		}
		return result;
	}

	wm_event_system::~wm_event_system() {
		for(auto event_listener : get_event_listeners()) {
			event_listener.destroy();
		}
		WM_LOG_INFO_1("event system destroyed");
	}

}
