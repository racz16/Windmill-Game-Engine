#pragma once

#include "scene/transform.h"
#include "scene/node.h"

namespace wm {

	class wm_transform: public transform {
	private:
		glm::vec3 relative_position {0.0f};
		glm::vec3 absolute_position {0.0f};
		glm::quat relative_rotation {1.0f, 0.0f, 0.0f, 0.0f};
		glm::quat absolute_rotation {1.0f, 0.0f, 0.0f, 0.0f};
		glm::vec3 relative_scale {1.0f, 1.0f, 1.0f};
		glm::vec3 absolute_scale {1.0f, 1.0f, 1.0f};
		glm::mat4 model_matrix {1.0f};
		glm::mat4 inverse_model_matrix {1.0f};
		glm::vec3 forward {0.0f, 0.0f, -1.0f};
		glm::vec3 right {1.0f, 0.0f, 0.0f};
		glm::vec3 up {0.0f, 1.0f, 0.0f};
		bool valid = true;
		int32_t id = -1;

		void refresh();
		void refresh_model_matrix_based_on_relative();
		void refresh_absolute_values();
		void refresh_based_on_absolute(const ptr_view<transform> parent_transform);
		void refresh_model_matrix_based_on_absolute();
		void refresh_relative_values(const ptr_view<transform> parent_transform);
		void refresh_inverse_model_matrix();
		void refresh_direction_vectors();
		void set_position_rotation_scale(const glm::mat4& matrix, glm::vec3& position, glm::quat& rotation, glm::vec3& scale);
		ptr<node> get_node() const;
		ptr_view<transform> get_parent_transform() const;
	public:
		wm_transform();
		wm_transform(const int32_t id);

		void invalidate() override;

		glm::vec3 get_relative_position() const override;
		void set_relative_position(const glm::vec3& position) override;
		glm::vec3 get_absolute_position() override;
		void set_absolute_position(const glm::vec3& position) override;

		glm::quat get_relative_rotation() const override;
		void set_relative_rotation(const glm::quat& rotation) override;
		void set_relative_rotation(const glm::vec3& axis, const float angle) override;
		glm::quat get_absolute_rotation() override;
		void set_absolute_rotation(const glm::quat& rotation) override;
		void set_absolute_rotation(const glm::vec3& axis, const float angle) override;

		glm::vec3 get_relative_scale() const override;
		void set_relative_scale(const glm::vec3& scale) override;
		glm::vec3 get_absolute_scale() override;
		void set_absolute_scale(const glm::vec3& scale) override;

		glm::mat4 get_model_matrix() override;
		glm::mat4 get_inverse_model_matrix() override;

		glm::vec3 get_forward() override;
		glm::vec3 get_right() override;
		glm::vec3 get_up() override;

		ptr<transform> get_ptr() const;
	};

}
