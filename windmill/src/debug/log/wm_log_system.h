#pragma once

#include "debug/log/log_system.h"
#include "debug/log/log_level.h"
#include "debug/log/log_writer.h"
#include "../../core/wm_base_system.h"

namespace wm {

	class wm_log_system: public wm_base_system, public log_system {
	private:
		std::vector<ptr<log_writer>> log_writers {};
	public:
		void add_log_writer(const ptr<log_writer> writer) override;
		size_t get_log_writer_count() const override;
		ptr<log_writer> get_log_writer(const int32_t index) const override;
		void remove_log_writer(const ptr<log_writer> log_writer) override;
		void clear_log_writers() override;
		void log_messaage(const log_level level, const std::string& message, const std::string& function, const int32_t line, const std::string& log_source) override;

		void update() override { wm_base_system::update(); }
		bool is_active() const override { return wm_base_system::is_active(); }
		void set_active(const bool active) { wm_base_system::set_active(active); }
	};

}
