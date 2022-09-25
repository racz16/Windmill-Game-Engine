#pragma once

#include "log_writer.h"
#include "../core/system.h"
#include "../core/key.h"

namespace wm {

	class WM_PUBLIC log_system: public system {
	public:
		static ptr<log_system> create();
		static key<log_system> get_key();

		virtual void add_log_writer(const ptr<log_writer> writer) = 0;
		virtual const std::vector<ptr<log_writer>> get_log_writers() const = 0;
		virtual void remove_log_writer(const ptr<log_writer> log_writer) = 0;
		virtual void clear_log_writers() = 0;
		virtual	void log_messaage(const log_level level, const std::string& message, const std::string& function, const int32_t line, const std::string& log_source) = 0;
	};

}
