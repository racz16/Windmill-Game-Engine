#include "wm_transform.h"

#include "core/engine.h"
#include "defines/log_defines.h"
#include "defines/code_generation_defines.h"
#include "defines/log_defines.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

namespace wm {

	void wm_transform::initialize(const int32_t id) {
		this->id = id;
		WM_LOG_INFO_3("transform created");
	}

	void wm_transform::add_event_listeners() {
		if(!node_event_listener.is_valid()) {
			node_event_listener = engine::get_event_system()->add_event_listener<node_parent_changed_event>(node_parent_changed_event::get_key(), [this](const node_parent_changed_event event) {
				this->handle_node_hierarchy_changes(event);
			}, get_node());
		}
	}

	void wm_transform::handle_node_hierarchy_changes(const node_parent_changed_event event) {
		valid = false;
		if(event.get_old_parent().is_valid()) {
			parent_event_listener.destroy();
		}
		if(event.get_new_parent().is_valid()) {
			parent_event_listener = engine::get_event_system()->add_event_listener<transform_changed_event>(transform_changed_event::get_key(), [this](const transform_changed_event event) {
				this->handle_parent_transform_changes();
			}, get_parent_transform());
		}
		WM_LOG_INFO_3("transform parent changed");
	}

	void wm_transform::handle_parent_transform_changes() {
		valid = false;
		emit_event(transform_changed_type::parent_changed);
		WM_LOG_INFO_3("transform parent changed");
	}

	void wm_transform::emit_event(const transform_changed_type type) const {
		transform_changed_event event(type, get_ptr());
		engine::get_event_system()->emit_event<transform_changed_event, transform>(transform_changed_event::get_key(), event, get_ptr());
	}

	void wm_transform::refresh(const bool is_absolute_set) {
		if(!valid) {
			refresh_model_matrix_based_on_relative();
			refresh_absolute_values();
			refresh_inverse_model_matrix();
			refresh_direction_vectors();
			valid = !is_absolute_set;
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
		inverse_model_matrix = inverse_model_matrix * glm::toMat4(glm::inverse(absolute_rotation));
		inverse_model_matrix = glm::translate(inverse_model_matrix, -absolute_position);
	}

	void wm_transform::refresh_direction_vectors() {
		right = glm::normalize(glm::vec3(model_matrix[0][0], model_matrix[1][0], -model_matrix[2][0]));
		up = glm::normalize(glm::vec3(model_matrix[0][1], model_matrix[1][1], -model_matrix[2][1]));
		forward = glm::normalize(glm::vec3(model_matrix[0][2], model_matrix[1][2], -model_matrix[2][2]));
	}

	void wm_transform::set_position_rotation_scale(const glm::mat4& matrix, glm::vec3& position, glm::quat& rotation, glm::vec3& scale) {
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(matrix, scale, rotation, position, skew, perspective);
	}

	ptr<node> wm_transform::get_node() const {
		return engine::get_scene_system()->get_node_by_transform(get_ptr());
	}

	ptr_view<transform> wm_transform::get_parent_transform() const {
		auto node = get_node();
		auto parent = node->get_parent();
		if(parent.is_valid()) {
			return parent->get_transform();
		}
		return ptr_view<transform>(nullptr);
	}

	glm::vec3 wm_transform::get_relative_position() const {
		return relative_position;
	}

	void wm_transform::set_relative_position(const glm::vec3& position) {
		relative_position = position;
		valid = false;
		emit_event(transform_changed_type::position_changed);
		WM_LOG_INFO_3("transform position changed");
	}

	glm::vec3 wm_transform::get_absolute_position() {
		refresh();
		return absolute_position;
	}

	void wm_transform::set_absolute_position(const glm::vec3& position) {
		auto parent_transform = get_parent_transform();
		if(parent_transform.is_valid()) {
			refresh(true);
			absolute_position = position;
			refresh_based_on_absolute(parent_transform);
			emit_event(transform_changed_type::position_changed);
			WM_LOG_INFO_3("transform position changed");
		} else {
			set_relative_position(position);
		}
	}

	void wm_transform::move(const glm::vec3& movement) {
		set_relative_position(relative_position + movement);
	}

	glm::quat wm_transform::get_relative_rotation() const {
		return relative_rotation;
	}

	void wm_transform::set_relative_rotation(const glm::quat& rotation) {
		relative_rotation = rotation;
		valid = false;
		emit_event(transform_changed_type::rotation_changed);
		WM_LOG_INFO_3("transform rotation changed");
	}

	void wm_transform::set_relative_rotation(const glm::vec3& axis, const float angle) {
		WM_ASSERT(glm::length(axis) != 0.0f);
		set_relative_rotation(glm::angleAxis(glm::radians(angle), axis));
	}

	glm::quat wm_transform::get_absolute_rotation() {
		refresh();
		return absolute_rotation;
	}

	void wm_transform::set_absolute_rotation(const glm::quat& rotation) {
		auto parent_transform = get_parent_transform();
		if(parent_transform.is_valid()) {
			refresh(true);
			absolute_rotation = rotation;
			refresh_based_on_absolute(parent_transform);
			emit_event(transform_changed_type::rotation_changed);
			WM_LOG_INFO_3("transform rotation changed");
		} else {
			set_relative_rotation(rotation);
		}
	}

	void wm_transform::set_absolute_rotation(const glm::vec3& axis, const float angle) {
		WM_ASSERT(glm::length(axis) != 0.0f);
		set_absolute_rotation(glm::angleAxis(glm::radians(angle), axis));
	}

	void wm_transform::rotate(const glm::quat& rotation) {
		set_relative_rotation(relative_rotation * rotation);
	}

	void wm_transform::rotate(const glm::vec3& axis, const float angle) {
		rotate(glm::angleAxis(glm::radians(angle), axis));
	}

	glm::vec3 wm_transform::get_relative_scale() const {
		return relative_scale;
	}

	void wm_transform::set_relative_scale(const glm::vec3& scale) {
		WM_LOG_WARNING_IF(scale[0] <= 0.0f || scale[1] <= 0.0f || scale[2] <= 0.0f, "zero or negative transform scale");
		relative_scale = scale;
		valid = false;
		emit_event(transform_changed_type::scale_changed);
		WM_LOG_INFO_3("transform scale changed");
	}

	glm::vec3 wm_transform::get_absolute_scale() {
		refresh();
		return absolute_scale;
	}

	void wm_transform::set_absolute_scale(const glm::vec3& scale) {
		WM_LOG_WARNING_IF(scale[0] <= 0.0f || scale[1] <= 0.0f || scale[2] <= 0.0f, "zero or negative transform scale");
		auto parent_transform = get_parent_transform();
		if(parent_transform.is_valid()) {
			refresh(true);
			absolute_scale = scale;
			refresh_based_on_absolute(parent_transform);
			emit_event(transform_changed_type::scale_changed);
			WM_LOG_INFO_3("transform scale changed");
		} else {
			set_relative_scale(scale);
		}
	}

	void wm_transform::scale(const glm::vec3& scale) {
		set_relative_scale(relative_scale * scale);
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

	wm_transform::~wm_transform() {
		node_event_listener.destroy();
		parent_event_listener.destroy();
		auto transform = get_ptr();
		transform_destroyed_event event(transform);
		engine::get_event_system()->emit_event(transform_destroyed_event::get_key(), event, transform);
		WM_LOG_INFO_2("transform destroyed");
	}

}
