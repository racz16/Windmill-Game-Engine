#include <numeric>

#include "defines/log_defines.h"

#include "wm_chrono_time_system.h"

namespace wm {

	const double wm_chrono_time_system::DEFAULT_FRAME_TIME = 1000.0 / 30.0;

	wm_chrono_time_system::wm_chrono_time_system(const int32_t delta_time_histroy_size): delta_time_history(delta_time_histroy_size, DEFAULT_FRAME_TIME) {
		WM_LOG_INFO_1("chrono time system created");
	}

	void wm_chrono_time_system::update() {
		every_frame_update();
		if(frame_time_sum >= 1s) {
			every_second_update();
		}
		WM_LOG_INFO_3("chrono time system updated");
	}

	void wm_chrono_time_system::every_frame_update() {
		const auto current_time_point = std::chrono::steady_clock::now();
		frame_time = current_time_point - last_time_point;
		frame_time_sum += frame_time;
		update_delta_time();
		frame_index++;
		frame_count_in_this_second++;
		last_time_point = current_time_point;
	}

	void wm_chrono_time_system::update_delta_time() {
		delta_time_history.at(oldest_delta_time_index) = frame_time > 1s ? DEFAULT_FRAME_TIME : get_frame_time();
		delta_time = 0.0;
		for(const auto delta_time_element : delta_time_history) {
			delta_time += delta_time_element;
		}
		delta_time /= delta_time_history.size();
		oldest_delta_time_index = (oldest_delta_time_index + 1) % static_cast<int32_t>(delta_time_history.size());
	}

	void wm_chrono_time_system::every_second_update() {
		double frame_time_sum_in_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(frame_time_sum).count();
		double frame_time_sum_in_milliseconds = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(frame_time_sum).count();
		fps = frame_count_in_this_second / frame_time_sum_in_seconds;
		average_frame_time = frame_time_sum_in_milliseconds / frame_count_in_this_second;
		frame_time_sum = 0ns;
		frame_count_in_this_second = 0;
	}

	double wm_chrono_time_system::get_time() const {
		return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(std::chrono::steady_clock::now().time_since_epoch()).count();
	}

	double wm_chrono_time_system::get_delta_time() const {
		return delta_time;
	}

	double wm_chrono_time_system::get_fps() const {
		return fps;
	}

	double wm_chrono_time_system::get_frame_time() const {
		return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(frame_time).count();
	}

	double wm_chrono_time_system::get_average_frame_time() const {
		return average_frame_time;
	}

	int32_t wm_chrono_time_system::get_frame_index() const {
		return frame_index;
	}

	wm_chrono_time_system::~wm_chrono_time_system() {
		WM_LOG_INFO_1("chrono time system destroyed");
	}
}
