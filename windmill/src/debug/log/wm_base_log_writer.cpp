#include "wm_base_log_writer.h"

namespace wm {

	wm_base_log_writer::wm_base_log_writer(const log_level max_log_level):max_log_level(max_log_level) { }

	log_level wm_base_log_writer::get_max_log_level() const {
		return this->max_log_level;
	}

	void wm_base_log_writer::set_max_log_level(const log_level level) {
		max_log_level = level;
	}

	bool wm_base_log_writer::is_force_flush() const {
		return this->force_flush;
	}

	void wm_base_log_writer::set_force_flush(const bool force_flush) {
		this->force_flush = force_flush;
	}

}
