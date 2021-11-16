#pragma once

#include "event/event_system.h"

#include "../core/wm_base_system.h"

namespace wm {

	class wm_event_system: public wm_base_system, public event_system {
	private:
		std::unordered_map<int32_t, std::vector<std::pair<int32_t, std::any>>> event_listeners;
	protected:
		virtual void add_event_listener(const int32_t key, const int32_t event_listener_id, const std::any event_listener) override;
		virtual bool has_event_listener(const int32_t key, const int32_t event_listener_id) override;
		virtual bool has_key(const int32_t key) override;
		virtual void remove_event_listener(const int32_t key, const int32_t event_listener_id) override;
		virtual const std::vector<std::pair<int32_t, std::any>>& get_event_listeners(const int32_t key) const override;

		void update() override { wm_base_system::update(); }
		bool is_active() const override { return wm_base_system::is_active(); }
		void set_active(const bool active) override { wm_base_system::set_active(active); }
	public:
		wm_event_system();
		~wm_event_system() override;
	};

}
