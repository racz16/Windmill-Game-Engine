#pragma once

#include "window/input/window_input_handler.h"

namespace wm {

	class wm_glfw_window_input_handler: public window_input_handler {
	private:
		static const int32_t GAMEPAD_COUNT = GLFW_JOYSTICK_LAST + 1;

		GLFWwindow* window_handler;
		//keyboard
		std::unordered_map<keyboard_button, bool> keyboard_button_down;
		std::unordered_map<keyboard_button, button_state> keyboard_button_state;
		//mouse
		std::unordered_map<mouse_button, bool> mouse_button_down;
		std::unordered_map<mouse_button, button_state> mouse_button_state;
		position_state mouse_position;
		//gamepad
		std::array<std::unordered_map<gamepad_button, bool>, GAMEPAD_COUNT> gamepad_button_down;
		std::array<std::unordered_map<gamepad_button, button_state>, GAMEPAD_COUNT> gamepad_button_state;
		std::array<std::unordered_map<gamepad_axis, float>, GAMEPAD_COUNT> gamepad_axis_value;
		std::array<std::unordered_map<gamepad_axis, axis_state>, GAMEPAD_COUNT> gamepad_axis_state;

		void initialize_keyboard();
		void initialize_mouse();
		void initialize_gamepad();
		void update_keyboard();
		void update_mouse();
		void update_gamepads();
		button_action create_button_action(const bool previous_down, const bool down) const;
	public:
		wm_glfw_window_input_handler(GLFWwindow* window_handler);
		void update() override;
		button_state get_keyboard_button_state(const keyboard_button button) const override;
		bool is_mouse_over_window() const override;
		button_state get_mouse_button_state(const mouse_button button) const override;
		position_state get_mouse_position() const override;
		int32_t get_max_gamepad_count() const override;
		bool is_gamepad_available(const int32_t gamepad_index) const override;
		std::string get_gamepad_name(const int32_t gamepad_index) const override;
		axis_state get_gamepad_axis_state(const int32_t gamepad_index, gamepad_axis axis) const override;
		button_state get_gamepad_button_state(const int32_t gamepad_index, gamepad_button button) const override;
		std::string get_clipboard_text() const override;
		void set_clipboard_text(const std::string& text) const override;
	};

}
