#pragma once

#include "../core/key.h"
#include "../ptr/ptr.h"
#include "../core/system.h"
#include "event.h"

namespace wm {

	template<class>
	class event_listener;
	class engine;
	class scene_system;

	class event_system: public system {
	protected:
		virtual void add_event_listener_impl(const int32_t key, const ptr<event_listener<event>> event_listener) = 0;
		virtual void add_event_listener_impl(const int32_t key, const ptr<event_listener<event>> event_listener, const ptr_view<void> source) = 0;
		virtual bool contains_event_listener_impl(const int32_t key, const ptr<event_listener<event>> event_listener) const = 0;
		virtual bool contains_event_listener_impl(const int32_t key, const ptr<event_listener<event>> event_listener, const ptr_view<void> source) const = 0;
		virtual bool contains_key(const int32_t key) const = 0;
		virtual bool contains_source(const int32_t key, const ptr_view<void> source) const = 0;
		virtual void remove_event_listener_impl(const int32_t key, const ptr<event_listener<event>>) = 0;
		virtual void remove_event_listener_impl(const int32_t key, const ptr<event_listener<event>>, const ptr_view<void> source) = 0;
		virtual const std::vector<ptr<event_listener<event>>>& get_event_listeners(const int32_t key) const = 0;
		virtual const std::vector<ptr<event_listener<event>>>& get_event_listeners(const int32_t key, const ptr_view<void> source) const = 0;
	public:
		static ptr<event_system> create();
		static key<event_system> get_key();

		template<class T>
		ptr<event_listener<T>> add_event_listener(const key<T> key, const std::function<void(const T)> callback) {
			auto el = ptr<event_listener<T>>(event_listener<T>::create(callback, key));
			add_event_listener_impl(key.get_hash(), el.template convert<event_listener<event>>());
			return el;
		}

		template<class T, class S>
		ptr<event_listener<T>> add_event_listener(const key<T> key, const std::function<void(const T)> callback, const ptr_view<S> source) {
			auto el = ptr<event_listener<T>>(event_listener<T>::create(callback, key, source));
			add_event_listener_impl(key.get_hash(), el.template convert<event_listener<event>>(), source.template convert<void>());
			return el;
		}

		template<class T>
		bool contains_event_listener(const key<T> key, const ptr<event_listener<T>> event_listener) {
			return contains_event_listener_impl(key.get_hash(), event_listener.template convert<wm::event_listener<event>>());
		}

		template<class T, class S>
		bool contains_event_listener(const key<T> key, const ptr<event_listener<T>> event_listener, const ptr_view<S> source) {
			return contains_event_listener_impl(key.get_hash(), event_listener.template convert<wm::event_listener<event>>(), source.template convert<void>());
		}

		template<class T>
		void remove_event_listener(const key<T> key, const ptr<event_listener<T>> event_listener) {
			remove_event_listener_impl(key.get_hash(), event_listener.template convert<wm::event_listener<event>>());
		}

		template<class T, class S>
		void remove_event_listener(const key<T> key, const ptr<event_listener<T>> event_listener, const ptr_view<S> source) {
			remove_event_listener_impl(key.get_hash(), event_listener.template convert<wm::event_listener<event>>(), source.template convert<void>());
		}

		template<class T>
		void emit_event(const key<T> key, const T event) {
			if(contains_key(key.get_hash())) {
				for(auto& event_listener : get_event_listeners(key.get_hash())) {
					event_listener.template convert<wm::event_listener<T>>()->callback(event);
				}
			}
		}

		template<class T, class S>
		void emit_event(const key<T> key, const T event, const ptr_view<S> source) {
			if(contains_source(key.get_hash(), source.template convert<void>())) {
				for(auto& event_listener : get_event_listeners(key.get_hash(), source.template convert<void>())) {
					event_listener.template convert<wm::event_listener<T>>()->callback(event);
				}
			}
			emit_event<T>(key, event);
		}

	};

}
