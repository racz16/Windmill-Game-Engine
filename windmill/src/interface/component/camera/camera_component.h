#pragma once

#include "projection_mode.h"
#include "../component.h"
#include "../../ptr/ptr.h"
#include "../../core/key.h"

namespace wm {

	class WM_PUBLIC camera_component: public component {
	public:
		static ptr<camera_component> create();
		static key<camera_component> get_key();

		virtual projection_mode get_projection_mode() const = 0;
		virtual void set_projection_mode(const projection_mode projection_mode) = 0;
		virtual float get_field_of_view() const = 0;
		virtual void set_field_of_view(const float field_of_view) = 0;
		virtual float get_near_plane() const = 0;
		virtual void set_near_plane(const float near_plane) = 0;
		virtual float get_far_plane() const = 0;
		virtual void set_far_plane(const float far_plane) = 0;
		virtual glm::vec2 get_scale() const = 0;
		virtual void set_scale(const glm::vec2& scale) = 0;
		virtual glm::mat4 get_view_matrix() = 0;
		virtual glm::mat4 get_projection_matrix() = 0;
	};

}
