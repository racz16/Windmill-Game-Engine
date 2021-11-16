#pragma once

#include "time/time_system.h"

#include "../core/wm_base_system.h"

using namespace std::chrono_literals;

namespace wm {

	class wm_chrono_time_system: public wm_base_system, public time_system {
	private:
		static const double DEFAULT_FRAME_TIME;

		std::chrono::time_point<std::chrono::steady_clock> last_time_point = std::chrono::steady_clock::now();
		std::chrono::nanoseconds frame_time = 0ns;
		std::chrono::nanoseconds frame_time_sum = 0ns;
		int32_t frame_count = 0;
		double fps = 0.0;
		double average_frame_time = 0.0;
		std::vector<double> delta_time_history;
		int32_t oldest_delta_time_index = 0;
		double delta_time = 0.0;

		void every_frame_update();
		void every_second_update();
	public:
		wm_chrono_time_system(const int32_t delta_time_histroy_size = 5);
		void update() override;
		double get_time() const override;
		double get_delta_time() const override;
		double get_fps() const override;
		double get_frame_time() const override;
		double get_average_frame_time() const override;
		~wm_chrono_time_system() override;

		bool is_active() const override { return wm_base_system::is_active(); }
		void set_active(const bool active) override { wm_base_system::set_active(active); }
	};

}
