#include "keyboard_button_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(keyboard_button_event, "WM_KEYBOARD_BUTTON_EVENT");

	keyboard_button_event::keyboard_button_event(const keyboard_button button, const button_state state, const int32_t scancode, const bool shift, const bool ctrl, const bool alt, const bool super, const bool caps_lock, const bool num_lock):
		event(true), button(button), state(state), scancode(scancode), shift(shift), ctrl(ctrl), alt(alt), super(super), caps_lock(caps_lock), num_lock(num_lock), button_name(get_button_name(button, scancode)) { }

	std::string keyboard_button_event::get_button_name(const keyboard_button button, const int32_t scancode) const {
		switch(button) {
			case keyboard_button::button_backspace: return "BACKSPACE";
			case keyboard_button::button_caps_lock: return "CAPS LOCK";
			case keyboard_button::button_delete: return "DELETE";
			case keyboard_button::button_down: return "DOWN";
			case keyboard_button::button_end: return "END";
			case keyboard_button::button_enter: return "ENTER";
			case keyboard_button::button_escape: return "ESC";
			case keyboard_button::button_f1: return "F1";
			case keyboard_button::button_f2: return "F2";
			case keyboard_button::button_f3: return "F3";
			case keyboard_button::button_f4: return "F4";
			case keyboard_button::button_f5: return "F5";
			case keyboard_button::button_f6: return "F6";
			case keyboard_button::button_f7: return "F7";
			case keyboard_button::button_f8: return "F8";
			case keyboard_button::button_f9: return "F9";
			case keyboard_button::button_f10: return "F10";
			case keyboard_button::button_f11: return "F11";
			case keyboard_button::button_f12: return "F12";
			case keyboard_button::button_f13: return "F13";
			case keyboard_button::button_f14: return "F14";
			case keyboard_button::button_f15: return "F15";
			case keyboard_button::button_f16: return "F16";
			case keyboard_button::button_f17: return "F17";
			case keyboard_button::button_f18: return "F18";
			case keyboard_button::button_f19: return "F19";
			case keyboard_button::button_f20: return "F20";
			case keyboard_button::button_f21: return "F21";
			case keyboard_button::button_f22: return "F22";
			case keyboard_button::button_f23: return "F23";
			case keyboard_button::button_f24: return "F24";
			case keyboard_button::button_f25: return "F25";
			case keyboard_button::button_home: return "HOME";
			case keyboard_button::button_insert: return "INS";
			case keyboard_button::button_left: return "LEFT";
			case keyboard_button::button_left_alt: return "LEFT ALT";
			case keyboard_button::button_left_control: return "LEFT CTRL";
			case keyboard_button::button_left_shift: return "LEFT SHIFT";
			case keyboard_button::button_left_super: return "LEFT SUPER";
			case keyboard_button::button_menu: return "MENU";
			case keyboard_button::button_num_lock: return "NUM LOCK";
			case keyboard_button::button_page_down: return "PAGE DOWN";
			case keyboard_button::button_page_up: return "PAGE UP";
			case keyboard_button::button_pause: return "PAUSE";
			case keyboard_button::button_print_screen: return "PRINT SCREEN";
			case keyboard_button::button_right: return "RIGHT";
			case keyboard_button::button_right_alt: return "RIGHT ALT";
			case keyboard_button::button_right_control: return "RIGHT CTRL";
			case keyboard_button::button_right_shift: return "RIGHT SHIFT";
			case keyboard_button::button_right_super: return "RIGHT SUEPR";
			case keyboard_button::button_scroll_lock: return "SCROLL LOCK";
			case keyboard_button::button_space: return "SPACE";
			case keyboard_button::button_tab: return "TAB";
			case keyboard_button::button_up: return "UP";
			case keyboard_button::button_kp_enter: return "KEYPAD ENTER";
			default:
				const auto button_code = static_cast<int>(button);
				const auto name = button_code == GLFW_KEY_UNKNOWN && scancode == 256 ? nullptr :glfwGetKeyName(button_code, scancode);
				return name == nullptr ? "UNKNOWN KEYBOARD BUTTON" : name;
		}
	}

	keyboard_button keyboard_button_event::get_button() const {
		return button;
	}

	std::string keyboard_button_event::get_button_name() const {
		return button_name;
	}

	button_state keyboard_button_event::get_state() const {
		return state;
	}

	int32_t keyboard_button_event::get_scancode() const {
		return scancode;
	}

	bool keyboard_button_event::is_shift_down() const {
		return shift;
	}

	bool keyboard_button_event::is_ctrl_down() const {
		return ctrl;
	}

	bool keyboard_button_event::is_alt_down() const {
		return alt;
	}

	bool keyboard_button_event::is_super_down() const {
		return super;
	}

	bool keyboard_button_event::is_caps_lock_active() const {
		return caps_lock;
	}

	bool keyboard_button_event::is_num_lock_active() const {
		return num_lock;
	}

}
