#pragma once

#include "scene/transform.h"
#include "scene/event/transform/transform_changed_event.h"
#include "scene/event/transform/transform_destroyed_event.h"
#include "scene/event/node/node_parent_changed_event.h"
#include "event/event_listener.h"

namespace wm {

	class wm_transform: public transform {
	private:
		glm::vec3 relative_position{0.0f};
		glm::vec3 absolute_position{0.0f};
		glm::quat relative_rotation{1.0f, 0.0f, 0.0f, 0.0f};
		glm::quat absolute_rotation{1.0f, 0.0f, 0.0f, 0.0f};
		glm::vec3 relative_scale{1.0f, 1.0f, 1.0f};
		glm::vec3 absolute_scale{1.0f, 1.0f, 1.0f};
		glm::mat4 model_matrix{1.0f};
		glm::mat4 inverse_model_matrix{1.0f};
		glm::vec3 forward{0.0f, 0.0f, -1.0f};
		glm::vec3 right{1.0f, 0.0f, 0.0f};
		glm::vec3 up{0.0f, 1.0f, 0.0f};
		bool valid = true;

		ptr<event_listener<transform_changed_event>> parent_event_listener{nullptr};
		ptr<event_listener<node_parent_changed_event>> node_event_listener{nullptr};

		void refresh(const bool is_absolute_set = false);
		void refresh_model_matrix_based_on_relative();
		void refresh_absolute_values();
		void refresh_based_on_absolute(const ptr_view<transform> parent_transform);
		void refresh_model_matrix_based_on_absolute();
		void refresh_relative_values(const ptr_view<transform> parent_transform);
		void refresh_inverse_model_matrix();
		void refresh_direction_vectors();
		void set_position_rotation_scale(const glm::mat4& matrix, glm::vec3& position, glm::quat& rotation, glm::vec3& scale);
		ptr_view<transform> get_parent_transform() const;
		void emit_event(const transform_changed_type type) const;
		void handle_node_hierarchy_changes(const node_parent_changed_event event);
		void handle_parent_transform_changes();
	public:
		void initialize(const int32_t id);

		ptr<node> get_node() const override;

		glm::vec3 get_relative_position() const override;
		void set_relative_position(const glm::vec3& position) override;
		glm::vec3 get_absolute_position() override;
		void set_absolute_position(const glm::vec3& position) override;
		void move(const glm::vec3& movement) override;

		glm::quat get_relative_rotation() const override;
		void set_relative_rotation(const glm::quat& rotation) override;
		void set_relative_rotation(const glm::vec3& axis, const float angle) override;
		glm::quat get_absolute_rotation() override;
		void set_absolute_rotation(const glm::quat& rotation) override;
		void set_absolute_rotation(const glm::vec3& axis, const float angle) override;
		void rotate(const glm::quat& rotation) override;
		void rotate(const glm::vec3& axis, const float angle) override;

		glm::vec3 get_relative_scale() const override;
		void set_relative_scale(const glm::vec3& scale) override;
		glm::vec3 get_absolute_scale() override;
		void set_absolute_scale(const glm::vec3& scale) override;
		void scale(const glm::vec3& scale) override;

		glm::mat4 get_model_matrix() override;
		glm::mat4 get_inverse_model_matrix() override;

		glm::vec3 get_forward() override;
		glm::vec3 get_right() override;
		glm::vec3 get_up() override;

		void add_event_listeners();

		~wm_transform();
	};

}
