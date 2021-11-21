#pragma once

#include "../../event/event.h"
#include "../../core/key.h"

namespace wm {

	class WM_PUBLIC gamepad_connection_event: public event {
	private:
		bool connected;
		int32_t gamepad_index;
	public:
		static key<gamepad_connection_event> get_key();

		gamepad_connection_event(const bool connected, const int32_t gamepad_index);
		bool is_connected() const;
		bool is_disconnected() const;
		int32_t get_gamepad_index() const;
	};

}
