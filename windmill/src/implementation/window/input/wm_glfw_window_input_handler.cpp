#include "core/utility.h"

#include "wm_glfw_window_input_handler.h"

namespace wm {

	wm_glfw_window_input_handler::wm_glfw_window_input_handler(GLFWwindow* window_handler): window_handler(window_handler), mouse_position(glm::dvec2(), glm::dvec2()) {
		initialize_keyboard();
		initialize_mouse();
		initialize_gamepad();
	}

	void wm_glfw_window_input_handler::initialize_keyboard() {
		for(const auto button : utility::get_keyboard_buttons()) {
			keyboard_button_down.insert_or_assign(button, false);
			keyboard_button_state.insert_or_assign(button, button_state(button_action::keep_up));
		}
	}

	void wm_glfw_window_input_handler::initialize_mouse() {
		for(const auto button : utility::get_mouse_buttons()) {
			mouse_button_down.insert_or_assign(button, false);
			mouse_button_state.insert_or_assign(button, button_state(button_action::keep_up));
		}

		double x_pos;
		double y_pos;
		glfwGetCursorPos(window_handler, &x_pos, &y_pos);
		mouse_position = position_state(glm::dvec2(x_pos, y_pos), glm::dvec2(x_pos, y_pos));
	}

	void wm_glfw_window_input_handler::initialize_gamepad() {
		for(int32_t i = 0; i < GAMEPAD_COUNT; i++) {
			for(const auto axis : utility::get_gamepad_axes()) {
				gamepad_axis_value.at(i).insert_or_assign(axis, 0.0f);
				gamepad_axis_state.at(i).insert_or_assign(axis, axis_state(0.0f, 0.0f));
			}
			for(const auto button : utility::get_gamepad_buttons()) {
				gamepad_button_down.at(i).insert_or_assign(button, false);
				gamepad_button_state.at(i).insert_or_assign(button, button_state(button_action::keep_up));
			}
		}
	}

	void wm_glfw_window_input_handler::update() {
		update_keyboard();
		update_mouse();
		update_gamepads();
	}

	void wm_glfw_window_input_handler::update_keyboard() {
		for(const auto button : utility::get_keyboard_buttons()) {
			const bool previous_down = keyboard_button_down.at(button);
			const bool down = glfwGetKey(window_handler, static_cast<int>(button));
			const button_action action = create_button_action(previous_down, down);
			keyboard_button_down.insert_or_assign(button, down);
			keyboard_button_state.insert_or_assign(button, button_state(action));
		}
	}

	void wm_glfw_window_input_handler::update_mouse() {
		for(const auto button : utility::get_mouse_buttons()) {
			const bool previous_down = mouse_button_down.at(button);
			const bool down = glfwGetMouseButton(window_handler, static_cast<int>(button));
			const button_action action = create_button_action(previous_down, down);
			mouse_button_down.insert_or_assign(button, down);
			mouse_button_state.insert_or_assign(button, button_state(action));
		}

		double x_pos;
		double y_pos;
		glfwGetCursorPos(window_handler, &x_pos, &y_pos);
		mouse_position = position_state(mouse_position.get_position(), glm::dvec2(x_pos, y_pos));
	}

	void wm_glfw_window_input_handler::update_gamepads() {
		GLFWgamepadstate state;
		for(int32_t i = 0; i < GAMEPAD_COUNT; i++) {
			if(glfwGetGamepadState(i, &state)) {
				for(const auto axis : utility::get_gamepad_axes()) {
					const float previous_value = gamepad_axis_value.at(i).at(axis);
					const float value = state.axes[static_cast<int>(axis)];
					gamepad_axis_value.at(i).insert_or_assign(axis, value);
					gamepad_axis_state.at(i).insert_or_assign(axis, axis_state(previous_value, value));
				}
				for(const auto button : utility::get_gamepad_buttons()) {
					const bool previous_down = gamepad_button_down.at(i).at(button);
					const bool down = state.buttons[static_cast<int>(button)];
					const button_action action = create_button_action(previous_down, down);
					gamepad_button_down.at(i).insert_or_assign(button, down);
					gamepad_button_state.at(i).insert_or_assign(button, button_state(action));
				}
			}
		}
	}

	button_state wm_glfw_window_input_handler::get_keyboard_button_state(const keyboard_button button) const {
		return keyboard_button_state.at(button);
	}

	bool wm_glfw_window_input_handler::is_mouse_over_window() const {
		return glfwGetWindowAttrib(window_handler, GLFW_HOVERED);
	}

	button_state wm_glfw_window_input_handler::get_mouse_button_state(const mouse_button button) const {
		return mouse_button_state.at(button);
	}

	position_state wm_glfw_window_input_handler::get_mouse_position() const {
		return mouse_position;
	}

	bool wm_glfw_window_input_handler::is_gamepad_available(const int32_t gamepad_index) const {
		return glfwJoystickIsGamepad(gamepad_index) == GLFW_TRUE;
	}

	std::string wm_glfw_window_input_handler::get_gamepad_name(const int32_t gamepad_index) const {
		if(is_gamepad_available(gamepad_index)) {
			return glfwGetJoystickName(gamepad_index);
		} else {
			return "";
		}
	}

	axis_state wm_glfw_window_input_handler::get_gamepad_axis_state(const int32_t gamepad_index, gamepad_axis axis) const {
		if(is_gamepad_available(gamepad_index)) {
			return gamepad_axis_state.at(gamepad_index).at(axis);
		} else {
			return axis_state(0.0f, 0.0f);
		}
	}

	button_state wm_glfw_window_input_handler::get_gamepad_button_state(const int32_t gamepad_index, gamepad_button button) const {
		if(is_gamepad_available(gamepad_index)) {
			return gamepad_button_state.at(gamepad_index).at(button);
		} else {
			return button_state(button_action::keep_up);
		}
	}

	std::string wm_glfw_window_input_handler::get_clipboard_text() const {
		auto text = glfwGetClipboardString(nullptr);
		if(text) {
			return text;
		} else {
			return "";
		}
	}

	void wm_glfw_window_input_handler::set_clipboard_text(const std::string& text) const {
		glfwSetClipboardString(nullptr, text.c_str());
	}

	button_action wm_glfw_window_input_handler::create_button_action(const bool previous_down, const bool down) const {
		if(previous_down && !down) {
			return button_action::release;
		} else if(!previous_down && down) {
			return button_action::press;
		} else if(previous_down && down) {
			return button_action::keep_down;
		} else {
			return button_action::keep_up;
		}
	}

}
