#include "gamepad_connection_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(gamepad_connection_event, "WM_GAMEPAD_CONNECTION_EVENT");

	gamepad_connection_event::gamepad_connection_event(const bool connected, const int32_t gamepad_index): event(true), connected(connected), gamepad_index(gamepad_index) {}

	bool gamepad_connection_event::is_connected() const {
		return connected;
	}

	bool gamepad_connection_event::is_disconnected() const {
		return !connected;
	}

	int32_t gamepad_connection_event::get_gamepad_index() const {
		return gamepad_index;
	}

}
