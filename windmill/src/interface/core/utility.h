#pragma once

#include "../defines/general_defines.h"
#include "../window/input/keyboard_button.h"
#include "../window/input/button_action.h"
#include "../window/input/mouse_button.h"
#include "../window/input/gamepad_axis.h"
#include "../window/input/gamepad_button.h"

namespace wm {

	class WM_PUBLIC utility {
	private:

		static const int32_t COLUMN_DISTANCE;
		static std::array<keyboard_button, KEYBOARD_BUTTON_COUNT> keyboard_buttons;
		static std::array<mouse_button, MOUSE_BUTTON_COUNT> mouse_buttons;
		static std::array<gamepad_axis, GAMEPAD_AXIS_COUNT> gamepad_axes;
		static std::array<gamepad_button, GAMEPAD_BUTTON_COUNT> gamepad_buttons;

		utility();
		static std::string repeat_character(const int32_t size, const std::string& character = " ");
		static void add_matrix_row(const std::vector<::std::vector<std::string>>& numbers, const std::vector<int32_t>& max_columns_widths, const int32_t y, std::string& result);
	public:

		static std::string to_string(const button_action action);

		template<int32_t S, class T>
		static std::string to_string(const glm::vec<S, T, glm::defaultp>& vector, const std::string& message = "") {
			std::vector<std::string> numbers;
			int32_t character_width = (S - 1) * COLUMN_DISTANCE + 2;

			for(int32_t i = 0; i < S; i++) {
				const std::string number = std::to_string(vector[i]);
				numbers.push_back(number);
				character_width += static_cast<int32_t>(number.length());
			}

			auto result = message != "" ? message + "\n" : "";
			result += "┌" + repeat_character(character_width) + "┐\n│ ";
			for(int32_t i = 0; i < S; i++) {
				result += numbers.at(i);
				if(i != S - 1) {
					result += repeat_character(COLUMN_DISTANCE);
				}
			}
			result += " │\n└" + repeat_character(character_width) + "┘";
			return result;
		}

		template<int32_t H, int32_t W, class T>
		static std::string to_string(const glm::mat<H, W, T, glm::defaultp>& matrix, const std::string& message = "") {
			std::vector<std::vector<std::string>> numbers;
			std::vector<int32_t> max_column_widths(H, 0);
			int32_t max_character_width = (W - 1) * COLUMN_DISTANCE + 2;

			for(int32_t x = 0; x < W; x++) {
				numbers.push_back(std::vector<std::string>());
				for(int32_t y = 0; y < H; y++) {
					const std::string number = std::to_string(matrix[x][y]);
					numbers.at(x).push_back(number);
					max_column_widths.at(x) = std::max(max_column_widths.at(x), (int32_t) number.length());
				}
				max_character_width += max_column_widths.at(x);
			}

			auto result = message != "" ? message + "\n" : "";
			result += "┌" + repeat_character(max_character_width) + "┐\n";
			for(int32_t y = 0; y < H; y++) {
				add_matrix_row(numbers, max_column_widths, y, result);
			}
			result += "└" + repeat_character(max_character_width) + "┘";
			return result;
		}

		template<class T>
		static std::string to_string(const glm::qua<T, glm::defaultp>& quaternion, const std::string& message = "") {
			const int32_t S = 4;
			std::vector<std::string> numbers;
			int32_t character_width = (S - 1) * COLUMN_DISTANCE + 2;

			for(int32_t i = 0; i < S; i++) {
				const std::string number = std::to_string(quaternion[i]);
				numbers.push_back(number);
				character_width += static_cast<int32_t>(number.length());
			}

			auto result = message != "" ? message + "\n" : "";
			result += "┌" + repeat_character(character_width) + "┐\n│ ";
			result += numbers.at(S - 1) + repeat_character(COLUMN_DISTANCE);
			for(int32_t i = 0; i < S - 1; i++) {
				result += numbers.at(i);
				if(i != S - 2) {
					result += repeat_character(COLUMN_DISTANCE);
				}
			}
			result += " │\n└" + repeat_character(character_width) + "┘";
			return result;
		}

		template<class T>
		static int32_t hash(T object) {
			return static_cast<int32_t>(std::hash<T> {}(object));
		}

		template<class T>
		static std::string to_string_axis_angle(const glm::qua<T, glm::defaultp>& quaternion, const std::string& message = "") {
			auto axis = glm::axis(quaternion);
			auto angle = glm::degrees(glm::angle(quaternion));
			auto result = message != "" ? message + "\n" : "";
			result += "angle:\n" + std::to_string(angle) + "°\n";
			result += to_string(axis, "axis:");
			return result;
		}

		static const std::array<keyboard_button, KEYBOARD_BUTTON_COUNT>& get_keyboard_buttons();
		static const std::array<mouse_button, MOUSE_BUTTON_COUNT>& get_mouse_buttons();
		static const std::array<gamepad_axis, GAMEPAD_AXIS_COUNT>& get_gamepad_axes();
		static const std::array<gamepad_button, GAMEPAD_BUTTON_COUNT>& get_gamepad_buttons();

		static uint32_t to_vulkan_version(const glm::ivec3 version);
	};

}
