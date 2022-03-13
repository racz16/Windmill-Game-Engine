#include "wm_transform.h"

#include "core/engine.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

namespace wm {

	wm_transform::wm_transform() {}

	wm_transform::wm_transform(const int32_t id): id(id) {}

	void wm_transform::invalidate() {
		valid = false;
		auto node = get_node();
		for(int32_t i = 0; i < node->get_children_count(); i++) {
			node->get_child(i)->get_transform()->invalidate();
		}
	}

	void wm_transform::refresh() {
		if(!valid) {
			refresh_model_matrix_based_on_relative();
			refresh_absolute_values();
			refresh_inverse_model_matrix();
			refresh_direction_vectors();
			valid = true;
		}
	}

	void wm_transform::refresh_model_matrix_based_on_relative() {
		model_matrix = glm::translate(glm::identity<glm::mat4>(), relative_position);
		model_matrix = model_matrix * glm::toMat4(relative_rotation);
		model_matrix = glm::scale(model_matrix, relative_scale);
		auto parent_transform = get_parent_transform();
		if(parent_transform.is_valid()) {
			model_matrix = parent_transform->get_model_matrix() * model_matrix;
		}
	}

	void wm_transform::refresh_absolute_values() {
		if(get_parent_transform().is_valid()) {
			set_position_rotation_scale(model_matrix, absolute_position, absolute_rotation, absolute_scale);
		} else {
			absolute_position = relative_position;
			absolute_rotation = relative_rotation;
			absolute_scale = relative_scale;
		}
	}

	void wm_transform::refresh_based_on_absolute(const ptr_view<transform> parent_transform) {
		invalidate();
		refresh_model_matrix_based_on_absolute();
		refresh_relative_values(parent_transform);
		refresh_inverse_model_matrix();
		refresh_direction_vectors();
		valid = true;
	}

	void wm_transform::refresh_model_matrix_based_on_absolute() {
		model_matrix = glm::translate(glm::identity<glm::mat4>(), absolute_position);
		model_matrix = model_matrix * glm::toMat4(absolute_rotation);
		model_matrix = glm::scale(model_matrix, absolute_scale);
	}

	void wm_transform::refresh_relative_values(const ptr_view<transform> parent_transform) {
		glm::mat4 local_model_matrix = parent_transform->get_inverse_model_matrix() * model_matrix;
		set_position_rotation_scale(local_model_matrix, relative_position, relative_rotation, relative_scale);
	}

	void wm_transform::refresh_inverse_model_matrix() {
		inverse_model_matrix = glm::scale(glm::identity<glm::mat4>(), 1.0f / absolute_scale);
		inverse_model_matrix = inverse_model_matrix * glm::toMat4(glm::inverse(relative_rotation));
		inverse_model_matrix = glm::translate(inverse_model_matrix, -absolute_position);
	}

	void wm_transform::refresh_direction_vectors() {
		right = glm::normalize(glm::vec3(model_matrix[0][0], model_matrix[1][0], model_matrix[2][0]));
		up = glm::normalize(glm::vec3(model_matrix[0][1], model_matrix[1][1], model_matrix[2][1]));
		forward = -glm::normalize(glm::vec3(model_matrix[0][2], model_matrix[1][2], model_matrix[2][2]));
	}

	void wm_transform::set_position_rotation_scale(const glm::mat4& matrix, glm::vec3& position, glm::quat& rotation, glm::vec3& scale) {
		glm::vec3 skew;
		glm::vec4 perspective;
 		glm::decompose(matrix, scale, rotation, position, skew, perspective);
	}

	ptr<node> wm_transform::get_node() const {
		return engine::get_scene_system()->get_node(get_ptr());
	}

	ptr_view<transform> wm_transform::get_parent_transform() const {
		auto node = get_node();
		auto parent = node->get_parent();
		if(parent.is_valid()) {
			return parent->get_transform();
		}
		return ptr<transform>(nullptr);
	}

	glm::vec3 wm_transform::get_relative_position() const {
		return relative_position;
	}

	void wm_transform::set_relative_position(const glm::vec3& position) {
		relative_position = position;
		invalidate();
	}

	glm::vec3 wm_transform::get_absolute_position() {
		refresh();
		return absolute_position;
	}

	void wm_transform::set_absolute_position(const glm::vec3& position) {
		auto parent_transform = get_parent_transform();
		if(parent_transform.is_valid()) {
			refresh();
			absolute_position = position;
			refresh_based_on_absolute(parent_transform);
		} else {
			set_relative_position(position);
		}
	}

	glm::quat wm_transform::get_relative_rotation() const {
		return relative_rotation;
	}

	void wm_transform::set_relative_rotation(const glm::quat& rotation) {
		relative_rotation = rotation;
		invalidate();
	}

	void wm_transform::set_relative_rotation(const glm::vec3& axis, const float angle) {
		set_relative_rotation(glm::angleAxis(glm::radians(angle), axis));
	}

	glm::quat wm_transform::get_absolute_rotation() {
		refresh();
		return absolute_rotation;
	}

	void wm_transform::set_absolute_rotation(const glm::quat& rotation) {
		auto parent_transform = get_parent_transform();
		if(parent_transform.is_valid()) {
			refresh();
			absolute_rotation = rotation;
			refresh_based_on_absolute(parent_transform);
		} else {
			set_relative_rotation(rotation);
		}
	}

	void wm_transform::set_absolute_rotation(const glm::vec3& axis, const float angle) {
		set_absolute_rotation(glm::angleAxis(glm::radians(angle), axis));
	}

	glm::vec3 wm_transform::get_relative_scale() const {
		return relative_scale;
	}

	void wm_transform::set_relative_scale(const glm::vec3& scale) {
		relative_scale = scale;
		invalidate();
	}

	glm::vec3 wm_transform::get_absolute_scale() {
		refresh();
		return absolute_scale;
	}

	void wm_transform::set_absolute_scale(const glm::vec3& scale) {
		auto parent_transform = get_parent_transform();
		if(parent_transform.is_valid()) {
			refresh();
			absolute_scale = scale;
			refresh_based_on_absolute(parent_transform);
		} else {
			set_relative_scale(scale);
		}
	}

	glm::mat4 wm_transform::get_model_matrix() {
		refresh();
		return model_matrix;
	}

	glm::mat4 wm_transform::get_inverse_model_matrix() {
		refresh();
		return inverse_model_matrix;
	}

	glm::vec3 wm_transform::get_forward() {
		refresh();
		return forward;
	}

	glm::vec3 wm_transform::get_right() {
		refresh();
		return right;
	}

	glm::vec3 wm_transform::get_up() {
		refresh();
		return up;
	}

	ptr<transform> wm_transform::get_ptr() const {
		if(id == -1) {
			return ptr<transform>(nullptr);
		} else {
			return ptr<transform>(id);
		}
	}

}
