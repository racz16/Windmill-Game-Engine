#pragma once

#include "../../defines.h"
#include "log_level.h"
#include "log_writer.h"

namespace wm {

	class WM_PUBLIC log_system {
	public:
		static std::shared_ptr<log_system> get_instance();

		virtual void add_log_writer(std::shared_ptr<log_writer> writer) = 0;
		virtual size_t get_log_writer_count() const = 0;
		virtual std::shared_ptr<log_writer> get_log_writer(const uint32_t index) const = 0;
		virtual void remove_log_writer(const uint32_t index) = 0;
		virtual void clear_log_writers() = 0;
		virtual	void log_messaage(const log_level level, const std::string& message, const std::string& function, const uint32_t line, const std::string& log_source) = 0;
		virtual ~log_system() { }
	};

}
