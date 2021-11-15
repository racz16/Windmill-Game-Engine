#include "time_system.h"

#include "../../core/wm_chrono_time_system.h"

namespace wm {

	ptr<time_system> time_system::create() {
		return ptr<time_system>(new wm_chrono_time_system());
	}

	key<time_system> time_system::get_key() {
		static const key<time_system> key("WM_TIME_SYSTEM");
		return key;
	}

}
