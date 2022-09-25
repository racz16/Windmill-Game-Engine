#include "wm_camera_component.h"

#include "core\engine.h"
#include "defines/code_generation_defines.h"
#include "defines/log_defines.h"

#include <glm/gtx/quaternion.hpp>

namespace wm {

	const ptr<camera_component> wm_camera_component::get_ptr() const {
		return get_ptr_impl().convert<camera_component>();
	}

	void wm_camera_component::refresh() {
		if(!valid) {
			auto node = get_node();
			if(node.is_valid()) {
				view_matrix = glm::mat4(1.0f) * glm::toMat4(glm::inverse(node->get_transform()->get_absolute_rotation()));
				view_matrix = glm::translate(view_matrix, -node->get_transform()->get_absolute_position());
			} else {
				view_matrix = glm::mat4(1.0f);
			}
			if(projection_mode == wm::projection_mode::perspective) {
				projection_matrix = glm::perspective(glm::radians(field_of_view), scale.x / scale.y, near_plane, far_plane);
			} else {
				projection_matrix = glm::ortho(-scale.x, scale.x, -scale.y, scale.y);
			}
			valid = true;
		}
	}

	projection_mode wm_camera_component::get_projection_mode() const {
		return projection_mode;
	}

	void wm_camera_component::set_projection_mode(const wm::projection_mode projection_mode) {
		this->projection_mode = projection_mode;
		valid = false;
		WM_LOG_INFO_3("camera projection mode changed");
	}

	float wm_camera_component::get_field_of_view() const {
		return field_of_view;
	}

	void wm_camera_component::set_field_of_view(const float field_of_view) {
		WM_ASSERT(field_of_view > 0.0f && field_of_view < 180.0f);
		this->field_of_view = field_of_view;
		valid = false;
		WM_LOG_INFO_3("camera field of view changed to " + std::to_string(field_of_view));
	}

	float wm_camera_component::get_near_plane() const {
		return near_plane;
	}

	void wm_camera_component::set_near_plane(const float near_plane) {
		WM_ASSERT(near_plane > 0.0f && near_plane < far_plane);
		this->near_plane = near_plane;
		valid = false;
		WM_LOG_INFO_3("camera near plane changed to " + std::to_string(near_plane));
	}

	float wm_camera_component::get_far_plane() const {
		return far_plane;
	}

	void wm_camera_component::set_far_plane(const float far_plane) {
		WM_ASSERT(far_plane > near_plane);
		this->far_plane = far_plane;
		valid = false;
		WM_LOG_INFO_3("camera far plane changed to " + std::to_string(far_plane));
	}

	glm::vec2 wm_camera_component::get_scale() const {
		return scale;
	}

	void wm_camera_component::set_scale(const glm::vec2& scale) {
		WM_ASSERT(scale.x != 0.0f && scale.y != 0.0f);
		this->scale = scale;
		valid = false;
		WM_LOG_INFO_3(utility::to_string(scale, "camera scale changed to"));
	}

	glm::mat4 wm_camera_component::get_view_matrix() {
		refresh();
		return view_matrix;
	}

	glm::mat4 wm_camera_component::get_projection_matrix() {
		refresh();
		return projection_matrix;
	}

	ptr<node> wm_camera_component::get_node() const {
		return engine::get_scene_system()->get_node_by_component<component>(get_ptr().convert<component>());
	}

	void wm_camera_component::added_to_node(const ptr<node> node) {
		transform_change_event_listener = engine::get_event_system()->add_event_listener<transform_changed_event>(transform_changed_event::get_key(), [this](const transform_changed_event event) {
			this->valid = false;
		}, node->get_transform());
	}

	void wm_camera_component::removed_from_node(const ptr<node> node) {
		transform_change_event_listener.destroy();
	}

}
