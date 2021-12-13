#pragma once

#include "../../defines/general_defines.h"
#include "button_action.h"

namespace wm {

	class WM_PUBLIC button_state {
	private:
		button_action action;
	public:
		button_state(const button_action action);
		bool is_down() const;
		bool is_up() const;
		button_action get_action() const;
		operator button_action() const;
	};

}
