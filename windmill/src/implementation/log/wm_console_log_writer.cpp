#include "defines/general_defines.h"

#include "wm_console_log_writer.h"

namespace wm {

	wm_console_log_writer::wm_console_log_writer(const log_level max_log_level): wm_base_log_writer(max_log_level) {
	#ifdef WM_PLATFORM_WINDOWS
		SetConsoleOutputCP(CP_UTF8);
	#endif
		std::ios::sync_with_stdio(false);
		std::cin.tie(NULL);
	}

	void wm_console_log_writer::log_message(const log_level level, const std::string& message, const std::string& function, const int32_t line, const std::string& log_source) {
		if(level > get_max_log_level()) {
			return;
		}
		if(level == log_level::error) {
			entry = "\033[31m[ERROR]";
		} else if(level == log_level::warning) {
			entry = "\033[33m[WARNING]";
		} else if(level == log_level::debug) {
			entry = "\033[32m[DEBUG]";
		} else if(level == log_level::info_1) {
			entry = "\033[36m[INFO 1]";
		} else if(level == log_level::info_2) {
			entry = "\033[34m[INFO 2]";
		} else {
			entry = "\033[35m[INFO 3]";
		}
		entry += "[" + log_source + "] " + function + ":" + std::to_string(line) + "\033[37m" + "\n" + message;
		std::cout << entry;
		if(is_force_flush()) {
			std::cout << std::endl;
		} else {
			std::cout << "\n";
		}
	}

}
