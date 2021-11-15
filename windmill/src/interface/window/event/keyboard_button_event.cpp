#include "keyboard_button_event.h"

#include "../../core/defines.h"
#include "../../core/engine.h"

namespace wm {

	key<keyboard_button_event> keyboard_button_event::get_key() {
		static const key<keyboard_button_event> key("WM_KEYBOARD_BUTTON_EVENT");
		return key;
	}

	keyboard_button_event::keyboard_button_event(const ptr<wm::window> window, const keyboard_button button, const keyboard_button_action action, const int32_t scancode, const bool shift, const bool ctrl, const bool alt, const bool super, const bool caps_lock, const bool num_lock):
		window_event(window), button(button), action(action), scancode(scancode), shift(shift), ctrl(ctrl), alt(alt), super(super), caps_lock(caps_lock), num_lock(num_lock), button_name(get_button_name(button, scancode)) { }

	std::string keyboard_button_event::get_button_name(const keyboard_button button, const int32_t scancode) const {
		switch(button) {
			case keyboard_button::BUTTON_BACKSPACE: return "BACKSPACE";
			case keyboard_button::BUTTON_CAPS_LOCK: return "CAPS LOCK";
			case keyboard_button::BUTTON_DELETE: return "DELETE";
			case keyboard_button::BUTTON_DOWN: return "DOWN";
			case keyboard_button::BUTTON_END: return "END";
			case keyboard_button::BUTTON_ENTER: return "ENTER";
			case keyboard_button::BUTTON_ESCAPE: return "ESC";
			case keyboard_button::BUTTON_F1: return "F1";
			case keyboard_button::BUTTON_F2: return "F2";
			case keyboard_button::BUTTON_F3: return "F3";
			case keyboard_button::BUTTON_F4: return "F4";
			case keyboard_button::BUTTON_F5: return "F5";
			case keyboard_button::BUTTON_F6: return "F6";
			case keyboard_button::BUTTON_F7: return "F7";
			case keyboard_button::BUTTON_F8: return "F8";
			case keyboard_button::BUTTON_F9: return "F9";
			case keyboard_button::BUTTON_F10: return "F10";
			case keyboard_button::BUTTON_F11: return "F11";
			case keyboard_button::BUTTON_F12: return "F12";
			case keyboard_button::BUTTON_F13: return "F13";
			case keyboard_button::BUTTON_F14: return "F14";
			case keyboard_button::BUTTON_F15: return "F15";
			case keyboard_button::BUTTON_F16: return "F16";
			case keyboard_button::BUTTON_F17: return "F17";
			case keyboard_button::BUTTON_F18: return "F18";
			case keyboard_button::BUTTON_F19: return "F19";
			case keyboard_button::BUTTON_F20: return "F20";
			case keyboard_button::BUTTON_F21: return "F21";
			case keyboard_button::BUTTON_F22: return "F22";
			case keyboard_button::BUTTON_F23: return "F23";
			case keyboard_button::BUTTON_F24: return "F24";
			case keyboard_button::BUTTON_F25: return  "F25";
			case keyboard_button::BUTTON_HOME: return "HOME";
			case keyboard_button::BUTTON_INSERT: return "INS";
			case keyboard_button::BUTTON_LEFT: return "LEFT";
			case keyboard_button::BUTTON_LEFT_ALT: return "LEFT ALT";
			case keyboard_button::BUTTON_LEFT_CONTROL: return "LEFT CTRL";
			case keyboard_button::BUTTON_LEFT_SHIFT: return "LEFT SHIFT";
			case keyboard_button::BUTTON_LEFT_SUPER: return "LEFT SUPER";
			case keyboard_button::BUTTON_MENU: return "MENU";
			case keyboard_button::BUTTON_NUM_LOCK: return "NUM LOCK";
			case keyboard_button::BUTTON_PAGE_DOWN: return "PAGE DOWN";
			case keyboard_button::BUTTON_PAGE_UP: return "PAGE UP";
			case keyboard_button::BUTTON_PAUSE: return "PAUSE";
			case keyboard_button::BUTTON_PRINT_SCREEN: return "PRINT SCREEN";
			case keyboard_button::BUTTON_RIGHT: return "RIGHT";
			case keyboard_button::BUTTON_RIGHT_ALT: return "RIGHT ALT";
			case keyboard_button::BUTTON_RIGHT_CONTROL: return "RIGHT CTRL";
			case keyboard_button::BUTTON_RIGHT_SHIFT: return "RIGHT SHIFT";
			case keyboard_button::BUTTON_RIGHT_SUPER: return "RIGHT SUEPR";
			case keyboard_button::BUTTON_SCROLL_LOCK: return "SCROLL LOCK";
			case keyboard_button::BUTTON_SPACE: return "SPACE";
			case keyboard_button::BUTTON_TAB: return "TAB";
			case keyboard_button::BUTTON_UP: return "UP";
			case keyboard_button::BUTTON_KP_ENTER: return "KEYPAD ENTER";
			default:
				const auto name = glfwGetKeyName(static_cast<int>(button), scancode);
				return name == nullptr ? "UNKNOWN KEYBOARD BUTTON" : name;
		}
	}

	keyboard_button keyboard_button_event::get_button() const {
		return button;
	}

	std::string keyboard_button_event::get_button_name() const {
		return button_name;
	}

	keyboard_button_action keyboard_button_event::get_action() const {
		return action;
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

	bool keyboard_button_event::is_caps_lock_down() const {
		return caps_lock;
	}

	bool keyboard_button_event::is_num_lock_down() const {
		return num_lock;
	}


}
