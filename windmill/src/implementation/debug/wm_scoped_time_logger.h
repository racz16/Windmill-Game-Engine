#pragma once

#include "debug/scoped_time_logger.h"

#include "wm_stopwatch.h"

namespace wm {

	class wm_scoped_time_logger: public scoped_time_logger {
	private:
		std::string name;
		wm_stopwatch stopwatch;
	public:
		wm_scoped_time_logger(const std::string& name = "elapsed time");
		~wm_scoped_time_logger() override;
	};

}
