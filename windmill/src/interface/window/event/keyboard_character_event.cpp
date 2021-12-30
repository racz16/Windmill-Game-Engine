#include "keyboard_character_event.h"

namespace wm {

	key<keyboard_character_event> keyboard_character_event::get_key() {
		static const key<keyboard_character_event> key("WM_KEYBOARD_CHARACTER_EVENT");
		return key;
	}

	keyboard_character_event::keyboard_character_event(uint32_t utf_32_code_point): event(true), utf_32_code_point(utf_32_code_point) { }

	uint32_t keyboard_character_event::get_utf_32_code_point() const {
		return utf_32_code_point;
	}

}
