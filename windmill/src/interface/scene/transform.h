#pragma once

#include "../ptr/ptr.h"
#include "../core/key.h"
#include "../defines/general_defines.h"

namespace wm {

	class WM_PUBLIC transform {
	private:
		static bool type_added;
	public:
		static ptr<transform> create();
		static key<transform> get_key();

		virtual void invalidate() = 0;

		virtual glm::vec3 get_relative_position() const = 0;
		virtual void set_relative_position(const glm::vec3& position) = 0;
		virtual glm::vec3 get_absolute_position() = 0;
		virtual void set_absolute_position(const glm::vec3& position) = 0;

		virtual glm::quat get_relative_rotation() const = 0;
		virtual void set_relative_rotation(const glm::quat& rotation) = 0;
		virtual void set_relative_rotation(const glm::vec3& axis, const float angle) = 0;
		virtual glm::quat get_absolute_rotation() = 0;
		virtual void set_absolute_rotation(const glm::quat& rotation) = 0;
		virtual void set_absolute_rotation(const glm::vec3& axis, const float angle) = 0;

		virtual glm::vec3 get_relative_scale() const = 0;
		virtual void set_relative_scale(const glm::vec3& scale) = 0;
		virtual glm::vec3 get_absolute_scale() = 0;
		virtual void set_absolute_scale(const glm::vec3& scale) = 0;

		virtual glm::mat4 get_model_matrix() = 0;
		virtual glm::mat4 get_inverse_model_matrix() = 0;

		virtual glm::vec3 get_forward() = 0;
		virtual glm::vec3 get_right() = 0;
		virtual glm::vec3 get_up() = 0;

		virtual ~transform() {}
	};

}
