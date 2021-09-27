#pragma warning(disable:4996) //localtime_s is an optional part of C++ and not available everywhere

#include "wm_file_log_writer.h"
#include "core/defines.h"
#include <filesystem>
#include <ctime>
#include <regex>

namespace wm {

	wm_file_log_writer::wm_file_log_writer(const log_level max_log_level, const std::string& path)
		: wm_base_log_writer(max_log_level), file_name(path + "/" + compute_file_name()) {
		WM_ASSERT(regex_match(path, std::regex(".*?[^/\\\\]")));
		std::filesystem::create_directories(path);
		file_stream.open(file_name, std::ios::out | std::ios::app);
	}

	void wm_file_log_writer::log_message(const log_level level, const std::string& message, const std::string& function, const uint32_t line, const std::string& log_source) {
		if(level > get_max_log_level()) {
			return;
		}
		if(level == log_level::Error) {
			entry = "[ERROR]";
		} else if(level == log_level::Warning) {
			entry = "[WARNING]";
		} else if(level == log_level::Debug) {
			entry = "[DEBUG]";
		} else if(level == log_level::Info_1) {
			entry = "[INFO 1]";
		} else if(level == log_level::Info_2) {
			entry = "[INFO 2]";
		} else {
			entry = "[INFO 3]";
		}
		entry += "[" + log_source + "] " + function + ":" + std::to_string(line) + "\n" + message + "\n";
		file_stream << entry;
		if(is_force_flush()) {
			file_stream << std::endl;
		} else {
			file_stream << "\n";
		}
	}

	std::string wm_file_log_writer::compute_file_name() const {
		const auto now = time(nullptr);
		const auto date = *localtime(&now);
		return std::to_string(date.tm_year + 1900) + "_" +
			std::to_string(date.tm_mon + 1) + "_" +
			std::to_string(date.tm_mday) + "_" +
			std::to_string(date.tm_hour) + "_" +
			std::to_string(date.tm_min) + "_" +
			std::to_string(now) + ".txt";
	}

	wm_file_log_writer::~wm_file_log_writer() {
		file_stream.close();
	}

}
