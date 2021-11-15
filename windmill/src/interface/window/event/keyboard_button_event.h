#pragma once

#include "window_event.h"
#include "../../core/key.h"
#include "../input/keyboard_button.h"
#include "../input/keyboard_button_action.h"

namespace wm {

	class WM_PUBLIC keyboard_button_event: public window_event {
	private:
		keyboard_button button;
		keyboard_button_action action;
		int32_t scancode;
		bool shift;
		bool ctrl;
		bool alt;
		bool super;
		bool caps_lock;
		bool num_lock;
		std::string button_name;

		std::string get_button_name(const keyboard_button button, const int32_t scancode) const;
	public:
		static key<keyboard_button_event> get_key();

		keyboard_button_event(const ptr<wm::window> window, const keyboard_button button, const keyboard_button_action action, const int32_t scancode, const bool shift, const bool ctrl, const bool alt, const bool super, const bool caps_lock, const bool num_lock);
		keyboard_button get_button() const;
		std::string get_button_name() const;
		keyboard_button_action get_action() const;
		int32_t get_scancode() const;
		bool is_shift_down() const;
		bool is_ctrl_down() const;
		bool is_alt_down() const;
		bool is_super_down() const;
		bool is_caps_lock_down() const;
		bool is_num_lock_down() const;
	};

}
