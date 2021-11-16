#include "mouse_button_event.h"

namespace wm {

	key<mouse_button_event> mouse_button_event::get_key() {
		static const key<mouse_button_event> key("WM_MOUSE_BUTTON_EVENT");
		return key;
	}

	mouse_button_event::mouse_button_event(const ptr<wm::window> window, const mouse_button button, const bool press, const bool shift, const bool ctrl, const bool alt, const bool super, const bool caps_lock, const bool num_lock):
		window_event(window), button(button), press(press), shift(shift), ctrl(ctrl), alt(alt), super(super), caps_lock(caps_lock), num_lock(num_lock), button_name(get_button_name(button)) { }

	std::string mouse_button_event::get_button_name(const mouse_button button) const {
		switch(button) {
			case mouse_button::button_left: return "LEFT MOUSE BUTTON";
			case mouse_button::button_right: return "RIGHT MOUSE BUTTON";
			case mouse_button::button_middle: return "MIDDLE MOUSE BUTTON";
			case mouse_button::button_4: return "MOUSE BUTTON 4";
			case mouse_button::button_5: return "MOUSE BUTTON 4";
			case mouse_button::button_6: return "MOUSE BUTTON 4";
			case mouse_button::button_7: return "MOUSE BUTTON 4";
			case mouse_button::button_8: return "MOUSE BUTTON 4";
			default: return "UNKNOWN MOUSE BUTTON";
		}
	}

	mouse_button mouse_button_event::get_button() const {
		return button;
	}

	std::string mouse_button_event::get_button_name() const {
		return button_name;
	}

	bool mouse_button_event::is_press() const {
		return press;
	}

	bool mouse_button_event::is_release() const {
		return !press;
	}

	bool mouse_button_event::is_shift_down() const {
		return shift;
	}

	bool mouse_button_event::is_ctrl_down() const {
		return ctrl;
	}

	bool mouse_button_event::is_alt_down() const {
		return alt;
	}

	bool mouse_button_event::is_super_down() const {
		return super;
	}

	bool mouse_button_event::is_caps_lock_down() const {
		return caps_lock;
	}

	bool mouse_button_event::is_num_lock_down() const {
		return num_lock;
	}

}
