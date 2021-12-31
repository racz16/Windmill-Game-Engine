#pragma once

#include "button_state.h"
#include "keyboard_button.h"
#include "mouse_button.h"
#include "position_state.h"
#include "gamepad_axis.h"
#include "gamepad_button.h"
#include "axis_state.h"

namespace wm {

	class WM_PUBLIC window_input_handler {
	public:
		virtual void update() = 0;
		virtual button_state get_keyboard_button_state(const keyboard_button button) const = 0;
		virtual bool is_mouse_over_window() const = 0;
		virtual button_state get_mouse_button_state(const mouse_button button) const = 0;
		virtual position_state get_mouse_position() const = 0;
		virtual bool is_gamepad_available(const int32_t gamepad_index) const = 0;
		virtual std::string get_gamepad_name(const int32_t gamepad_index) const = 0;
		virtual axis_state get_gamepad_axis_state(const int32_t gamepad_index, gamepad_axis axis) const = 0;
		virtual button_state get_gamepad_button_state(const int32_t gamepad_index, gamepad_button button) const = 0;
		virtual std::string get_clipboard_text() const = 0;
		virtual void set_clipboard_text(const std::string& text) const = 0;
		virtual ~window_input_handler() { }
	};

}
