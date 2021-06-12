#include <memory>

#include "engine.h"
#include "../wm_engine.h"

namespace wm {

	std::shared_ptr<engine> engine::create() {
		return std::make_shared<wm_engine>();
	}

}
