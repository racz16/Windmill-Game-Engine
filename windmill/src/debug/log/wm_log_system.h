#pragma once

#include "debug/log/log_system.h"
#include "debug/log/log_level.h"
#include "debug/log/log_writer.h"

namespace wm {

	class wm_log_system: public log_system {
	private:
		std::vector<std::shared_ptr<log_writer>> log_writers {};
	public:
		wm_log_system();
		void add_log_writer(std::shared_ptr<log_writer> writer) override;
		size_t get_log_writer_count() override;
		std::shared_ptr<log_writer> get_log_writer(const uint32_t index) override;
		void remove_log_writer(const uint32_t index) override;
		void clear_log_writers() override;
		void log_messaage(const log_level level, const std::string& message, const std::string& function, const uint32_t line, const std::string& log_source) override;
	};

}
