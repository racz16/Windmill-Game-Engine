#pragma once

#include "../core/engine.h"

namespace wm {

	template<class T>
	class event_listener {
	private:
		int32_t id = -1;
		std::function<void(const T)> callback_function;
		wm::key<T> key;
		ptr<void> source{nullptr};
		bool has_source = false;

		event_listener<T>(const std::function<void(const T)> callback_function, const wm::key<T> key): callback_function(callback_function), key(key) {}

		event_listener<T>(const std::function<void(const T)> callback_function, const wm::key<T> key, const ptr<void> source) : callback_function(callback_function), key(key), source(source), has_source(true) {}

		static ptr<event_listener<T>> create(event_listener<T>* raw_pointer) {
			auto pointer = ptr<event_listener<T>>(raw_pointer);
			const int32_t id = pointer.get_id();
			raw_pointer->initialize(id);
			return pointer;
		}

		void initialize(const int32_t id) {
			this->id = id;
		}

		const ptr<event_listener<T>> get_ptr() const {
			if(id == -1) {
				return ptr<event_listener<T>>(nullptr);
			} else {
				return ptr<event_listener<T>>(id);
			}
		}

	public:

		static ptr<event_listener<T>> create(const std::function<void(const T)> callback_function, const wm::key<T> key) {
			auto raw_pointer = new event_listener<T>(callback_function, key);
			return create(raw_pointer);
		}

		template<class S>
		static ptr<event_listener<T>> create(const std::function<void(const T)> callback_function, const wm::key<T> key, const ptr<S> source) {
			auto raw_pointer = new event_listener<T>(callback_function, key, source.template convert<void>());
			return create(raw_pointer);
		}

		void callback(const T event) const {
			callback_function(event);
		}

		void unsubscribe() {
			if(has_source) {
				engine::get_event_system()->remove_event_listener(key, get_ptr(), source);
			} else {
				engine::get_event_system()->remove_event_listener(key, get_ptr());
			}
		}

		~event_listener() {
			unsubscribe();
		}

	};

}
