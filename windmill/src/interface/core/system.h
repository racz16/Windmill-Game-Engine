#pragma once

#include "defines.h"

namespace wm {

	class WM_PUBLIC system {
	public:
		virtual void update() = 0;
		virtual bool is_active() const = 0;
		virtual void set_active(const bool active) = 0;
		virtual ~system() { }
	};

}
