#pragma once

#include "log/log_system.h"

namespace wm {

	class wm_log_system: public log_system {
	private:
		bool active = true;
		std::vector<ptr<log_writer>> log_writers {};
	public:
		void add_log_writer(const ptr<log_writer> writer) override;
		const std::vector<ptr<log_writer>> get_log_writers() const override;
		void remove_log_writer(const ptr<log_writer> log_writer) override;
		void clear_log_writers() override;
		void log_messaage(const log_level level, const std::string& message, const std::string& function, const int32_t line, const std::string& log_source) override;
	};

}
