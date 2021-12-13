#pragma once

#include <stdexcept>

#include "../defines/general_defines.h"

namespace wm {

//https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-2-c4275
//C4275 can be ignored in Visual C++ if you are deriving from a type in the C++ Standard Library, compiling a debug release (/MTd) and where the compiler error message refers to _Container_base.
#pragma warning(suppress: 4275)
	class WM_PUBLIC windmill_error: public std::runtime_error {
	private:
		std::string function;
		int32_t line;
	public:
		windmill_error(const std::string message, const std::string function, const int32_t line);
		std::string get_function() const;
		int32_t get_line() const;
	};

}
