#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/quaternion.hpp>

#include "defines.h"

namespace wm {

	class WM_PUBLIC utility {
	private:

		utility();

		static std::string get_border(const int32_t size, const std::string& start, const std::string& stop);

		template<int32_t H, int32_t W, class T>
		static void add_matrix_row(const glm::mat<H, W, T, glm::defaultp>& matrix, const int32_t y, std::string& result) {
			result += "│ ";
			for(int32_t x = 0; x < W; x++) {
				result += std::to_string(matrix[x][y]);
				if(x != W - 1) {
					result += "  ";
				}
			}
			result += " │\n";
		}

	public:

		template<int32_t S, class T>
		static std::string to_string(const glm::vec<S, T, glm::defaultp>& vector, const std::string& message = "") {
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

		template<int32_t H, int32_t W, class T>
		static std::string to_string(const glm::mat<H, W, T, glm::defaultp>& matrix, const std::string& message = "") {
			auto result = message != "" ? message + "\n" : "";
			result += get_border(W, "┌", "┐") + "\n";
			for(int32_t y = 0; y < H; y++) {
				add_matrix_row(matrix, y, result);
			}
			result += get_border(W, "└", "┘");
			return result;
		}

		template<class T>
		static std::string to_string(const glm::qua<T, glm::defaultp>& quaternion, const std::string& message = "") {
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

	};

}
