#pragma once

#include "../core/key.h"
#include "../ptr/ptr.h"

namespace wm {

	class WM_PUBLIC audio_listener {
	public:
		static ptr<audio_listener> create();
		static key<audio_listener> get_key();

		virtual float get_volume() const = 0;
		virtual void set_volume(const float volume) = 0;
		virtual glm::vec3 get_position() const = 0;
		virtual void set_position(const glm::vec3& position) = 0;
		virtual glm::vec3 get_velocity() const = 0;
		virtual void set_velocity(const glm::vec3& velocity) = 0;
		virtual glm::vec3 get_forward() const = 0;
		virtual glm::vec3 get_up() const = 0;
		virtual void set_orientation(const glm::vec3& forward, const glm::vec3& up) = 0;
		virtual ~audio_listener() = default;
	};

}
