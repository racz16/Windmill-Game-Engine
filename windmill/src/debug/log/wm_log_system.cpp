#include "wm_log_system.h"

namespace wm {

	void wm_log_system::add_log_writer(const ptr<log_writer> writer) {
		log_writers.push_back(writer);
	}

	size_t wm_log_system::get_log_writer_count() const {
		return log_writers.size();
	}

	ptr<log_writer> wm_log_system::get_log_writer(const int32_t index) const {
		return log_writers.at(index);
	}

	void wm_log_system::remove_log_writer(const ptr<log_writer> log_writer) {
		for(int32_t i = 0; i < log_writers.size(); i++) {
			if(log_writers.at(i).get_id() == log_writer.get_id()) {
				log_writers.erase(log_writers.begin() + i);
				return;
			}
		}
	}

	void wm_log_system::clear_log_writers() {
		log_writers.clear();
	}

	void wm_log_system::log_messaage(const log_level level, const std::string& message, const std::string& function, const int32_t line, const std::string& log_source) {
		for(auto writer : log_writers) {
			writer->log_message(level, message, function, line, log_source);
		}
	}

}
