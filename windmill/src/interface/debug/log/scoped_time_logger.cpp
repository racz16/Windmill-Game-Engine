#include "scoped_time_logger.h"
#include "../../../debug/log/wm_scoped_time_logger.h"

namespace wm {

	std::shared_ptr<scoped_time_logger> scoped_time_logger::create() {
		return std::make_shared<wm_scoped_time_logger>();
	}

}
