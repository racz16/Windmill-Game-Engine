#include "wm_log_system.h"

namespace wm {

	wm_log_system::wm_log_system() {
		std::ios::sync_with_stdio(false);
	}

	void wm_log_system::add_log_writer(std::shared_ptr<log_writer> writer) {
		log_writers.push_back(writer);
	}

	size_t wm_log_system::get_log_writer_count() {
		return log_writers.size();
	}

	std::shared_ptr<log_writer> wm_log_system::get_log_writer(const uint32_t index) {
		return log_writers[index];
	}

	void wm_log_system::remove_log_writer(const uint32_t index) {
		log_writers.erase(log_writers.begin() + index);
	}

	void wm_log_system::clear_log_writers() {
		log_writers.clear();
	}

	void wm_log_system::log_messaage(const log_level level, const std::string& message, const std::string& function, const uint32_t line, const std::string& log_source) {
		for(auto writer : log_writers) {
			writer->log_message(level, message, function, line, log_source);
		}
	}

}
