#pragma once

#include "component/camera/camera_component.h"
#include "scene/event/transform/transform_changed_event.h"

namespace wm {

	class wm_camera_component: public camera_component {
	private:
		wm::projection_mode projection_mode = projection_mode::perspective;
		float field_of_view = 75.0f;
		float near_plane = 0.1f;
		float far_plane = 100.0f;
		glm::vec2 scale{16.0f, 9.0f};
		glm::mat4 view_matrix = glm::identity<glm::mat4>();
		glm::mat4 projection_matrix = glm::identity<glm::mat4>();
		bool valid = false;
		ptr<event_listener<transform_changed_event>> transform_change_event_listener{nullptr};

		void refresh();
	protected:
		ptr<camera_component> get_ptr() const;
		void added_to_node(const ptr<node> node) override;
		void removed_from_node(const ptr<node> node) override;
	public:
		ptr<node> get_node() const override;
		wm::projection_mode get_projection_mode() const override;
		void set_projection_mode(const wm::projection_mode projection_mode) override;
		float get_field_of_view() const override;
		void set_field_of_view(const float field_of_view) override;
		float get_near_plane() const override;
		void set_near_plane(const float near_plane) override;
		float get_far_plane() const override;
		void set_far_plane(const float far_plane) override;
		glm::vec2 get_scale() const override;
		void set_scale(const glm::vec2& scale) override;
		glm::mat4 get_view_matrix() override;
		glm::mat4 get_projection_matrix() override;
	};

}
