#include "log_system.h"

#include "../../implementation/log/wm_log_system.h"

namespace wm {

	ptr<log_system> log_system::create() {
		return ptr<log_system>(new wm_log_system());
	}

	key<log_system> log_system::get_key() {
		static const key<log_system> key("WM_LOG_SYSTEM");
		return key;
	}

}
