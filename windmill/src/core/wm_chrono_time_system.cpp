#include "wm_chrono_time_system.h"

namespace wm {

	wm_chrono_time_system::wm_chrono_time_system() {
		WM_LOG_INFO_1("chrono time system constructed");
	}

	void wm_chrono_time_system::update() {
		const auto current_time_point = std::chrono::steady_clock::now();
		frame_time = current_time_point - last_time_point;
		frame_time_sum += frame_time;
		delta_time = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(frame_time).count();
		frame_count++;
		last_time_point = current_time_point;
		if(frame_time_sum >= 1s) {
			double frame_time_sum_in_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(frame_time_sum).count();
			fps = frame_count / frame_time_sum_in_seconds;
			average_frame_time = frame_time_sum_in_seconds / frame_count;
			frame_time_sum = 0ns;
			frame_count = 0;
		}
	}

	double wm_chrono_time_system::get_time() const {
		return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();
	}

	double wm_chrono_time_system::get_delta_time() const {
		return delta_time;
	}

	double wm_chrono_time_system::get_fps() const {
		return fps;
	}

	double wm_chrono_time_system::get_frame_time() const {
		return std::chrono::duration_cast<std::chrono::duration<double, std::nano>>(frame_time).count();
	}

	double wm_chrono_time_system::get_average_frame_time() const {
		return average_frame_time;
	}

	wm_chrono_time_system::~wm_chrono_time_system() {
		WM_LOG_INFO_1("chrono time system destructed");
	}
}
