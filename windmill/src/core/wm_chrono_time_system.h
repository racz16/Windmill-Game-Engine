#pragma once

#include <chrono>

#include "wm_base_system.h"
#include "core/time_system.h"
#include "core/engine.h"

using namespace std::chrono_literals;

namespace wm {

	class wm_chrono_time_system: public wm_base_system, public time_system {
	private:
		std::chrono::time_point<std::chrono::steady_clock> last_time_point = std::chrono::steady_clock::now();
		std::chrono::nanoseconds frame_time = 0ns;
		std::chrono::nanoseconds frame_time_sum = 0ns;
		int32_t frame_count = 0;
		double fps = 0.0;
		double average_frame_time = 0.0;
		double delta_time = 0.0;
	public:
		wm_chrono_time_system();
		void update() override;
		double get_time() const override;
		double get_delta_time() const override;
		double get_fps() const override;
		double get_frame_time() const override;
		double get_average_frame_time() const override;
		~wm_chrono_time_system();

		bool is_active() const override { return wm_base_system::is_active(); }
		void set_active(const bool active) override { wm_base_system::set_active(active); }
	};

}
