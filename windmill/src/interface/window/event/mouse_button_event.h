#pragma once

#include "window_event.h"
#include "../../core/key.h"
#include "../input/mouse_button.h"
#include "../input/button_action.h"

namespace wm {

	class WM_PUBLIC mouse_button_event: public window_event {
	private:
		mouse_button button;
		button_action action;
		bool shift;
		bool ctrl;
		bool alt;
		bool super;
		bool caps_lock;
		bool num_lock;
		std::string button_name;

		std::string get_button_name(const mouse_button button) const;
	public:
		static key<mouse_button_event> get_key();

		mouse_button_event(const ptr<wm::window> window, const mouse_button button, const button_action action,const bool shift, const bool ctrl, const bool alt, const bool super, const bool caps_lock, const bool num_lock);
		mouse_button get_button() const;
		std::string get_button_name() const;
		button_action get_action() const;
		bool is_shift_down() const;
		bool is_ctrl_down() const;
		bool is_alt_down() const;
		bool is_super_down() const;
		bool is_caps_lock_down() const;
		bool is_num_lock_down() const;
	};

}