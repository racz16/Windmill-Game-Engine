#pragma once

#include "../defines/general_defines.h"

namespace wm {

	class WM_PUBLIC system {
	private:
		bool active = true;
	public:
		virtual void update();
		virtual bool is_active() const;
		virtual void set_active(const bool active);
		virtual ~system();
	};

}
