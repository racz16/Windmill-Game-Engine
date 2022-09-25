#include "utility.h"

namespace wm {

	const int32_t utility::COLUMN_DISTANCE = 2;

	std::array<keyboard_button, KEYBOARD_BUTTON_COUNT> utility::keyboard_buttons = {
		keyboard_button::button_space,
		keyboard_button::button_apostrophe,
		keyboard_button::button_comma,
		keyboard_button::button_minus,
		keyboard_button::button_period,
		keyboard_button::button_slash,
		keyboard_button::button_0,
		keyboard_button::button_1,
		keyboard_button::button_2,
		keyboard_button::button_3,
		keyboard_button::button_4,
		keyboard_button::button_5,
		keyboard_button::button_6,
		keyboard_button::button_7,
		keyboard_button::button_8,
		keyboard_button::button_9,
		keyboard_button::button_semicolon,
		keyboard_button::button_equal,
		keyboard_button::button_a,
		keyboard_button::button_b,
		keyboard_button::button_c,
		keyboard_button::button_d,
		keyboard_button::button_e,
		keyboard_button::button_f,
		keyboard_button::button_g,
		keyboard_button::button_h,
		keyboard_button::button_i,
		keyboard_button::button_j,
		keyboard_button::button_k,
		keyboard_button::button_l,
		keyboard_button::button_m,
		keyboard_button::button_n,
		keyboard_button::button_o,
		keyboard_button::button_p,
		keyboard_button::button_q,
		keyboard_button::button_r,
		keyboard_button::button_s,
		keyboard_button::button_t,
		keyboard_button::button_u,
		keyboard_button::button_v,
		keyboard_button::button_w,
		keyboard_button::button_x,
		keyboard_button::button_y,
		keyboard_button::button_z,
		keyboard_button::button_left_bracket,
		keyboard_button::button_backslash,
		keyboard_button::button_right_bracket,
		keyboard_button::button_backtick,
		keyboard_button::button_world_1,
		keyboard_button::button_world_2,
		keyboard_button::button_escape,
		keyboard_button::button_enter,
		keyboard_button::button_tab,
		keyboard_button::button_backspace,
		keyboard_button::button_insert,
		keyboard_button::button_delete,
		keyboard_button::button_right,
		keyboard_button::button_left,
		keyboard_button::button_down,
		keyboard_button::button_up,
		keyboard_button::button_page_up,
		keyboard_button::button_page_down,
		keyboard_button::button_home,
		keyboard_button::button_end,
		keyboard_button::button_caps_lock,
		keyboard_button::button_scroll_lock,
		keyboard_button::button_num_lock,
		keyboard_button::button_print_screen,
		keyboard_button::button_pause,
		keyboard_button::button_f1,
		keyboard_button::button_f2,
		keyboard_button::button_f3,
		keyboard_button::button_f4,
		keyboard_button::button_f5,
		keyboard_button::button_f6,
		keyboard_button::button_f7,
		keyboard_button::button_f8,
		keyboard_button::button_f9,
		keyboard_button::button_f10,
		keyboard_button::button_f11,
		keyboard_button::button_f12,
		keyboard_button::button_f13,
		keyboard_button::button_f14,
		keyboard_button::button_f15,
		keyboard_button::button_f16,
		keyboard_button::button_f17,
		keyboard_button::button_f18,
		keyboard_button::button_f19,
		keyboard_button::button_f20,
		keyboard_button::button_f21,
		keyboard_button::button_f22,
		keyboard_button::button_f23,
		keyboard_button::button_f24,
		keyboard_button::button_f25,
		keyboard_button::button_kp_0,
		keyboard_button::button_kp_1,
		keyboard_button::button_kp_2,
		keyboard_button::button_kp_3,
		keyboard_button::button_kp_4,
		keyboard_button::button_kp_5,
		keyboard_button::button_kp_6,
		keyboard_button::button_kp_7,
		keyboard_button::button_kp_8,
		keyboard_button::button_kp_9,
		keyboard_button::button_kp_decimal,
		keyboard_button::button_kp_divide,
		keyboard_button::button_kp_multiply,
		keyboard_button::button_kp_subtract,
		keyboard_button::button_kp_add,
		keyboard_button::button_kp_enter,
		keyboard_button::button_kp_equal,
		keyboard_button::button_left_shift,
		keyboard_button::button_left_control,
		keyboard_button::button_left_alt,
		keyboard_button::button_left_super,
		keyboard_button::button_right_shift,
		keyboard_button::button_right_control,
		keyboard_button::button_right_alt,
		keyboard_button::button_right_super,
		keyboard_button::button_menu,
	};
	std::array<mouse_button, MOUSE_BUTTON_COUNT> utility::mouse_buttons = {
		mouse_button::button_left,
		mouse_button::button_right,
		mouse_button::button_middle,
		mouse_button::button_4,
		mouse_button::button_5,
		mouse_button::button_6,
		mouse_button::button_7,
		mouse_button::button_8,
	};
	std::array<gamepad_axis, GAMEPAD_AXIS_COUNT> utility::gamepad_axes = {
		gamepad_axis::axis_left_x,
		gamepad_axis::axis_left_y,
		gamepad_axis::axis_right_x,
		gamepad_axis::axis_right_y,
		gamepad_axis::axis_left_trigger,
		gamepad_axis::axis_right_trigger,
	};
	std::array<gamepad_button, GAMEPAD_BUTTON_COUNT> utility::gamepad_buttons = {
		gamepad_button::button_a,
		gamepad_button::button_b,
		gamepad_button::button_x,
		gamepad_button::button_y,
		gamepad_button::button_left_bumper,
		gamepad_button::button_right_bumper,
		gamepad_button::button_back,
		gamepad_button::button_start,
		gamepad_button::button_guide,
		gamepad_button::button_left_trigger,
		gamepad_button::button_right_trigger,
		gamepad_button::button_dpad_up,
		gamepad_button::button_dpad_right,
		gamepad_button::button_dpad_down,
		gamepad_button::button_dpad_left,
	};

	utility::utility() { }

	std::string utility::to_string(const button_action action) {
		switch(action) {
			case button_action::release: return "release";
			case button_action::press: return "press";
			case button_action::keep_down: return "keep down";
			case button_action::keep_up: return "keep up";
			default: return "UNKNOWN BUTTON ACTION";
		}
	}

	std::string utility::repeat_character(const int32_t size, const std::string& character) {
		std::string result = "";
		for(int32_t i = 0; i < size; i++) {
			result += character;
		}
		return result;
	}

	void utility::add_matrix_row(const std::vector<::std::vector<std::string>>& numbers, const std::vector<int32_t>& max_columns_widths, const int32_t y, std::string& result) {
		result += "│ ";
		for(int32_t x = 0; x < max_columns_widths.size(); x++) {
			const auto number = numbers.at(x).at(y);
			result += repeat_character(max_columns_widths.at(x) - static_cast<int32_t>(number.length())) + number;
			if(x != max_columns_widths.size() - 1) {
				result += repeat_character(COLUMN_DISTANCE);
			}
		}
		result += " │\n";
	}

	const std::array<keyboard_button, KEYBOARD_BUTTON_COUNT>& utility::get_keyboard_buttons() {
		return keyboard_buttons;
	}

	const std::array<mouse_button, MOUSE_BUTTON_COUNT>& utility::get_mouse_buttons() {
		return mouse_buttons;
	}

	const std::array<gamepad_axis, GAMEPAD_AXIS_COUNT>& utility::get_gamepad_axes() {
		return gamepad_axes;
	}

	const std::array<gamepad_button, GAMEPAD_BUTTON_COUNT>& utility::get_gamepad_buttons() {
		return gamepad_buttons;
	}

	uint32_t utility::to_vulkan_version(const glm::ivec3 version) {
		return (static_cast<uint32_t>(version[0]) << 22) | (static_cast<uint32_t>(version[1]) << 12) | (static_cast<uint32_t>(version[2]));
	}

}
