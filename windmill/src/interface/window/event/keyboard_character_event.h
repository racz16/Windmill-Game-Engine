#pragma once

#include "../../core/key.h"
#include "window_event.h"

namespace wm {

	class WM_PUBLIC keyboard_character_event: public window_event {
	private:
		uint32_t utf_32_code_point;
	public:
		static key<keyboard_character_event> get_key();

		keyboard_character_event(const ptr<wm::window> window, uint32_t utf_32_code_point);
		uint32_t get_utf_32_code_point() const;
	};

}
