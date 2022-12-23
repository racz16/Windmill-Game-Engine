#pragma once

namespace wm {

	struct wm_gpu_matrices_ubo {
		glm::mat4 model;
		glm::mat3 inverse_model;
		glm::mat4 view;
		glm::mat4 projection;
	};

}
