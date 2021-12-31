#pragma once

#include "../../core/key.h"
#include "../../event/event.h"

namespace wm {

	class WM_PUBLIC keyboard_character_event: public event {
	private:
		uint32_t utf_32_code_point;
	public:
		static key<keyboard_character_event> get_key();

		keyboard_character_event(uint32_t utf_32_code_point);
		uint32_t get_utf_32_code_point() const;
	};

}
