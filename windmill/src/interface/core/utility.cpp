#include "utility.h"

namespace wm {

	utility::utility() { }

	std::string utility::get_border(const int32_t size, const std::string& start, const std::string& stop) {
		std::string result = "";
		for(int32_t i = 0; i < size; i++) {
			result += "          ";
		}
		return start + result + stop;
	}

}
