#pragma once

#include "../core/key.h"
#include "../core/ptr_view.h"
#include "../core/ptr.h"
#include "../core/system.h"
#include "event_listener.h"
#include "event.h"

namespace wm {

	class event_system: public system {
	protected:
		virtual void add_event_listener(const int32_t key, const int32_t event_listener_id, const std::any event_listener) = 0;
		virtual bool has_event_listener(const int32_t key, const int32_t event_listener_id) = 0;
		virtual bool has_key(const int32_t key) = 0;
		virtual void remove_event_listener(const int32_t key, const int32_t event_listener_id) = 0;
		virtual const std::vector<std::pair<int32_t, std::any>>& get_event_listeners(const int32_t key) const = 0;
	public:
		static ptr<event_system> create();
		static key<event_system> get_key();

		template<class T>
		void add_event_listener(const key<T> key, const ptr_view<event_listener<T>> event_listener) {
			WM_ASSERT(event_listener.is_valid());
			add_event_listener(key.get_hash(), event_listener.get_id(), event_listener);
		}

		template<class T>
		bool has_event_listener(const key<T> key, const ptr_view<event_listener<T>> event_listener) {
			return has_event_listener(key.get_hash(), event_listener.get_id());
		}

		template<class T>
		void remove_event_listener(const key<T> key, const ptr_view<event_listener<T>> event_listener) {
			remove_event_listener(key.get_hash(), event_listener.get_id());
		}

		template<class T>
		void emit_event(const key<T> key, const T event) {
			if(has_key(key.get_hash())) {
				for(auto el : get_event_listeners(key.get_hash())) {
					auto event_listener = std::any_cast<ptr_view<wm::event_listener<T>>>(el.second);
					WM_ASSERT(event_listener.is_valid());
					event_listener->callback(event);
				}
			}
		}

	};

}
