#include "keyboard_character_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(keyboard_character_event, "WM_KEYBOARD_CHARACTER_EVENT");

	keyboard_character_event::keyboard_character_event(uint32_t utf_32_code_point): event(true), utf_32_code_point(utf_32_code_point) { }

	uint32_t keyboard_character_event::get_utf_32_code_point() const {
		return utf_32_code_point;
	}

}
