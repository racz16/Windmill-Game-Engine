#pragma once

#include "debug/log/log_level.h"
#include "debug/log/log_writer.h"

namespace wm {

	class wm_base_log_writer: public log_writer {
	private:
		log_level max_log_level;
		bool force_flush = false;
	public:
		wm_base_log_writer(const log_level max_log_level = log_level::Debug);
		log_level get_max_log_level() override;
		void set_max_log_level(const log_level level) override;
		bool is_force_flush() override;
		void set_force_flush(const bool force_flush) override;
	};

}
