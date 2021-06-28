#pragma once

namespace wm {

	std::string to_string(const glm::vec2& vector, const std::string& message = "") {
		auto result = message != "" ? message + "\n" : "";
		return result + "┌                    ┐" + "\n" +
			"│ " + std::to_string(vector[0]) + "  " + std::to_string(vector[1]) + " │" + "\n" +
			"└                    ┘";
	}

	std::string to_string(const glm::vec3& vector, const std::string& message = "") {
		auto result = message != "" ? message + "\n" : "";
		return result + "┌                              ┐" + "\n" +
			"│ " + std::to_string(vector[0]) + "  " + std::to_string(vector[1]) + "  " + std::to_string(vector[2]) + " │" + "\n" +
			"└                              ┘";
	}

	std::string to_string(const glm::vec4& vector, const std::string& message = "") {
		auto result = message != "" ? message + "\n" : "";
		return result + "┌                                        ┐" + "\n" +
			"│ " + std::to_string(vector[0]) + "  " + std::to_string(vector[1]) + "  " + std::to_string(vector[2]) + "  " + std::to_string(vector[3]) + " │" + "\n" +
			"└                                        ┘";
	}

	std::string to_string(const glm::quat& quaternion, const std::string& message = "") {
		auto result = message != "" ? message + "\n" : "";
		return result + "┌                                        ┐" + "\n" +
			"│ " + std::to_string(quaternion[0]) + "  " + std::to_string(quaternion[1]) + "  " + std::to_string(quaternion[2]) + "  " + std::to_string(quaternion[3]) + " │" + "\n" +
			"└                                        ┘";
	}

	std::string to_string(const glm::mat2& matrix, const std::string& message = "") {
		auto result = message != "" ? message + "\n" : "";
		return result + "┌                    ┐" + "\n" +
			"│ " + std::to_string(matrix[0][0]) + "  " + std::to_string(matrix[1][0]) + "  " + " │" + "\n" +
			"│ " + std::to_string(matrix[0][1]) + "  " + std::to_string(matrix[1][1]) + "  " + " │" + "\n" +
			"└                    ┘";
	}

	std::string to_string(const glm::mat3& matrix, const std::string& message = "") {
		auto result = message != "" ? message + "\n" : "";
		return result + "┌                              ┐" + "\n" +
			"│ " + std::to_string(matrix[0][0]) + "  " + std::to_string(matrix[1][0]) + "  " + std::to_string(matrix[2][0]) + " │" + "\n" +
			"│ " + std::to_string(matrix[0][1]) + "  " + std::to_string(matrix[1][1]) + "  " + std::to_string(matrix[2][1]) + " │" + "\n" +
			"│ " + std::to_string(matrix[0][2]) + "  " + std::to_string(matrix[1][2]) + "  " + std::to_string(matrix[2][2]) + " │" + "\n" +
			"└                              ┘";
	}

	std::string to_string(const glm::mat4& matrix, const std::string& message = "") {
		auto result = message != "" ? message + "\n" : "";
		return result + "┌                                        ┐" + "\n" +
			"│ " + std::to_string(matrix[0][0]) + "  " + std::to_string(matrix[1][0]) + "  " + std::to_string(matrix[2][0]) + "  " + std::to_string(matrix[3][0]) + " │" + "\n" +
			"│ " + std::to_string(matrix[0][1]) + "  " + std::to_string(matrix[1][1]) + "  " + std::to_string(matrix[2][1]) + "  " + std::to_string(matrix[3][1]) + " │" + "\n" +
			"│ " + std::to_string(matrix[0][2]) + "  " + std::to_string(matrix[1][2]) + "  " + std::to_string(matrix[2][2]) + "  " + std::to_string(matrix[3][2]) + " │" + "\n" +
			"│ " + std::to_string(matrix[0][3]) + "  " + std::to_string(matrix[1][3]) + "  " + std::to_string(matrix[2][3]) + "  " + std::to_string(matrix[3][3]) + " │" + "\n" +
			"└                                        ┘";
	}

}
