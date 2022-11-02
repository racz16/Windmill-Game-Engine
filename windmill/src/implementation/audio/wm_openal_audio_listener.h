#pragma once

#include "audio/audio_listener.h"

namespace wm {

	class wm_openal_audio_listener: public audio_listener {
	public:
		float get_volume() const override;
		void set_volume(const float volume) override;
		glm::vec3 get_position() const override;
		void set_position(const glm::vec3& position) override;
		glm::vec3 get_velocity() const override;
		void set_velocity(const glm::vec3& velocity) override;
		glm::vec3 get_forward() const override;
		glm::vec3 get_up() const override;
		void set_orientation(const glm::vec3& forward, const glm::vec3& up) override;
	};

}
