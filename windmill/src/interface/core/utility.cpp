#include "utility.h"

namespace wm {

	const int32_t utility::COLUMN_DISTANCE = 2;

	utility::utility() { }

	std::string utility::to_string(const keyboard_button_action action) {
		switch(action) {
			case keyboard_button_action::press: return "press";
			case keyboard_button_action::hold: return "hold";
			case keyboard_button_action::release: return "release";
			default: return "UNKNOWN KEYBOARD BUTTON ACTION";
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
			const auto number = numbers[x][y];
			result += repeat_character(max_columns_widths[x] - number.length()) + number;
			if(x != max_columns_widths.size() - 1) {
				result += repeat_character(COLUMN_DISTANCE);
			}
		}
		result += " │\n";
	}

}
