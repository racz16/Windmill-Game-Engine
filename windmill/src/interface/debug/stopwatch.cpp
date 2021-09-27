#include "stopwatch.h"
#include "../../debug/wm_stopwatch.h"

namespace wm {

	ptr<stopwatch> stopwatch::create() {
		return ptr<stopwatch>(new wm_stopwatch());
	}

}
