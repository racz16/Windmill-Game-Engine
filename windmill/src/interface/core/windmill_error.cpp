#include "windmill_error.h"

namespace wm {

	windmill_error::windmill_error(const std::string& message, const std::string& function, const int32_t line): std::runtime_error(message), function(function), line(line) { }

	std::string windmill_error::get_function() const {
		return function;
	}

	int32_t windmill_error::get_line() const {
		return line;
	}

}
