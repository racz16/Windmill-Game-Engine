#include "log_system.h"
#include "../../../debug/log/wm_log_system.h"

namespace wm {

	std::shared_ptr<log_system> log_system::create() {
		return std::make_shared<wm_log_system>();
	}

}