#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../defines.h"

namespace wm {

	void asd(glm::vec3 v) { }

	std::string get_border(const int32_t size, const std::string& start, const std::string& stop) {
		std::string result = "";
		for(int32_t i = 0; i < size; i++) {
			result += "          ";
		}
		return start + result + stop;
	}

	template<int32_t S, class T>
	std::string to_string(const glm::vec<S, T, glm::defaultp>& vector, const std::string& message = "") {
		auto result = message != "" ? message + "\n" : "";
		result += get_border(S, "┌", "┐") + "\n│ ";
		for(int32_t i = 0; i < S; i++) {
			result += std::to_string(vector[i]);
			if(i != S - 1) {
				result += "  ";
			}
		}
		result += " │\n" + get_border(S, "└", "┘");
		return result;
	}

	template<int32_t S1, int32_t S2, class T>
	std::string to_string(const glm::mat<S1, S2, T, glm::defaultp>& matrix, const std::string& message = "") {
		auto result = message != "" ? message + "\n" : "";
		result += get_border(S2, "┌", "┐") + "\n";
		for(int32_t y = 0; y < S1; y++) {
			result += "│ ";
			for(int32_t x = 0; x < S2; x++) {
				result += std::to_string(matrix[x][y]);
				if(x != S1 - 1) {
					result += "  ";
				}
			}
			result += " │\n";
		}
		result += get_border(S2, "└", "┘");
		return result;
	}

	template<class T>
	std::string to_string(const glm::qua<T, glm::defaultp>& quaternion, const std::string& message = "") {
		auto result = message != "" ? message + "\n" : "";
		result += get_border(4, "┌", "┐") + "\n│ ";
		for(int32_t i = 0; i < 4; i++) {
			result += std::to_string(quaternion[i]);
			if(i != 3) {
				result += "  ";
			}
		}
		result += " │\n" + get_border(4, "└", "┘");
		return result;
	}

}
