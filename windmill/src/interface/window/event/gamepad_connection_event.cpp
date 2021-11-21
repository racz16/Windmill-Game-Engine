#include "gamepad_connection_event.h"

namespace wm {

	key<gamepad_connection_event> gamepad_connection_event::get_key() {
		static const key<gamepad_connection_event> key("WM_GAMEPAD_CONNECTION_EVENT");
		return key;
	}

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
