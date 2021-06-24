#pragma once

namespace wm {

	std::string to_string(const glm::vec2& vector, const std::string& message = "") {
		auto result = message != "" ? message + WM_NEW_LINE : "";
		return result + "┌                    ┐" + WM_NEW_LINE +
			"│ " + std::to_string(vector[0]) + "  " + std::to_string(vector[1]) + " │" + WM_NEW_LINE +
			"└                    ┘";
	}

	std::string to_string(const glm::vec3& vector, const std::string& message = "") {
		auto result = message != "" ? message + WM_NEW_LINE : "";
		return result + "┌                              ┐" + WM_NEW_LINE +
			"│ " + std::to_string(vector[0]) + "  " + std::to_string(vector[1]) + "  " + std::to_string(vector[2]) + " │" + WM_NEW_LINE +
			"└                              ┘";
	}

	std::string to_string(const glm::vec4& vector, const std::string& message = "") {
		auto result = message != "" ? message + WM_NEW_LINE : "";
		return result + "┌                                        ┐" + WM_NEW_LINE +
			"│ " + std::to_string(vector[0]) + "  " + std::to_string(vector[1]) + "  " + std::to_string(vector[2]) + "  " + std::to_string(vector[3]) + " │" + WM_NEW_LINE +
			"└                                        ┘";
	}

	std::string to_string(const glm::quat& quaternion, const std::string& message = "") {
		auto result = message != "" ? message + WM_NEW_LINE : "";
		return result + "┌                                        ┐" + WM_NEW_LINE +
			"│ " + std::to_string(quaternion[0]) + "  " + std::to_string(quaternion[1]) + "  " + std::to_string(quaternion[2]) + "  " + std::to_string(quaternion[3]) + " │" + WM_NEW_LINE +
			"└                                        ┘";
	}

	std::string to_string(const glm::mat2& matrix, const std::string& message = "") {
		auto result = message != "" ? message + WM_NEW_LINE : "";
		return result + "┌                    ┐" + WM_NEW_LINE +
			"│ " + std::to_string(matrix[0][0]) + "  " + std::to_string(matrix[1][0]) + "  " + " │" + WM_NEW_LINE +
			"│ " + std::to_string(matrix[0][1]) + "  " + std::to_string(matrix[1][1]) + "  " + " │" + WM_NEW_LINE +
			"└                    ┘";
	}

	std::string to_string(const glm::mat3& matrix, const std::string& message = "") {
		auto result = message != "" ? message + WM_NEW_LINE : "";
		return result + "┌                              ┐" + WM_NEW_LINE +
			"│ " + std::to_string(matrix[0][0]) + "  " + std::to_string(matrix[1][0]) + "  " + std::to_string(matrix[2][0]) +  " │" + WM_NEW_LINE +
			"│ " + std::to_string(matrix[0][1]) + "  " + std::to_string(matrix[1][1]) + "  " + std::to_string(matrix[2][1]) +  " │" + WM_NEW_LINE +
			"│ " + std::to_string(matrix[0][2]) + "  " + std::to_string(matrix[1][2]) + "  " + std::to_string(matrix[2][2]) +  " │" + WM_NEW_LINE +
			"└                              ┘";
	}

	std::string to_string(const glm::mat4& matrix, const std::string& message = "") {
		auto result = message != "" ? message + WM_NEW_LINE : "";
		return result + "┌                                        ┐" + WM_NEW_LINE +
			"│ " + std::to_string(matrix[0][0]) + "  " + std::to_string(matrix[1][0]) + "  " + std::to_string(matrix[2][0]) + "  " + std::to_string(matrix[3][0]) + " │" + WM_NEW_LINE +
			"│ " + std::to_string(matrix[0][1]) + "  " + std::to_string(matrix[1][1]) + "  " + std::to_string(matrix[2][1]) + "  " + std::to_string(matrix[3][1]) + " │" + WM_NEW_LINE +
			"│ " + std::to_string(matrix[0][2]) + "  " + std::to_string(matrix[1][2]) + "  " + std::to_string(matrix[2][2]) + "  " + std::to_string(matrix[3][2]) + " │" + WM_NEW_LINE +
			"│ " + std::to_string(matrix[0][3]) + "  " + std::to_string(matrix[1][3]) + "  " + std::to_string(matrix[2][3]) + "  " + std::to_string(matrix[3][3]) + " │" + WM_NEW_LINE +
			"└                                        ┘";
	}

}
