#include "button_state.h"

namespace wm {

	button_state::button_state(const button_action action): action(action) { }

	bool button_state::is_down() const {
		return action == button_action::press || action == button_action::keep_down;
	}

	bool button_state::is_up() const{
		return action == button_action::keep_up || action == button_action::release;
	}

	button_action button_state::get_action() const {
		return action;
	}

	button_state::operator button_action() const {
		return action;
	}

}
