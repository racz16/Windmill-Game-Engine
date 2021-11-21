#include "scoped_time_logger.h"

#include "../../implementation/debug/wm_scoped_time_logger.h"

namespace wm {

	std::unique_ptr<scoped_time_logger> scoped_time_logger::create() {
		return std::make_unique<wm_scoped_time_logger>();
	}

}
