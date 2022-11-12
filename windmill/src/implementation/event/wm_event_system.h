#pragma once

#include "event/event_system.h"

namespace wm {

	class wm_event_system: public event_system {
	private:
		std::unordered_map<int32_t, std::vector<ptr<event_listener<event>>>> global_event_listeners;
		std::unordered_map<int32_t, std::unordered_map<ptr_view<void>, std::vector<ptr<event_listener<event>>> >> local_event_listeners;
	protected:
		void add_event_listener_impl(const int32_t key, const ptr<event_listener<event>> event_listener) override;
		void add_event_listener_impl(const int32_t key, const ptr<event_listener<event>> event_listener, const ptr_view<void> source) override;
		bool contains_event_listener_impl(const int32_t key, const ptr<event_listener<event>> event_listener) const override;
		bool contains_event_listener_impl(const int32_t key, const ptr<event_listener<event>> event_listener, const ptr_view<void> source) const override;
		bool contains_key(const int32_t key) const override;
		bool contains_key(const int32_t key, const ptr_view<void> source) const;
		bool contains_source(const int32_t key, const ptr_view<void> source) const override;
		void remove_event_listener_impl(const int32_t key, const ptr<event_listener<event>> event_listener) override;
		void remove_event_listener_impl(const int32_t key, const ptr<event_listener<event>> event_listener, const ptr_view<void> source) override;
		const std::vector<ptr<event_listener<event>>>& get_event_listeners(const int32_t key) const override;
		const std::vector<ptr<event_listener<event>>>& get_event_listeners(const int32_t key, const ptr_view<void> source) const override;
		std::vector<ptr<event_listener<event>>> get_event_listeners() const;
	public:
		wm_event_system();
		~wm_event_system() override;
	};

}
