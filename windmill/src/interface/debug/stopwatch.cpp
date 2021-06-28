#include "stopwatch.h"
#include "../../debug/wm_stopwatch.h"

namespace wm {

	std::shared_ptr<stopwatch> stopwatch::create() {
		return std::make_shared<wm_stopwatch>();
	}

}
