#pragma once

#include "wm_base_log_writer.h"

namespace wm {

	class wm_file_log_writer: public wm_base_log_writer {
	private:
		std::string file_name;
		std::ofstream file_stream;
		std::string entry;

		std::string compute_file_name() const;
	public:
		wm_file_log_writer(const log_level max_log_level = log_level::debug, const std::string& path = "log");
		void log_message(const log_level level, const std::string& message, const std::string& function, const int32_t line, const std::string& log_source) override;
		~wm_file_log_writer() override;
	};

}
