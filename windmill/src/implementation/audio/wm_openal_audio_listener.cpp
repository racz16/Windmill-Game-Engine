#include "wm_openal_audio_listener.h"
#include "wm_openal_audio_context.h"

namespace wm {

	float wm_openal_audio_listener::get_volume() const {
		float volume;
		alGetListenerf(AL_GAIN, &volume);
		WM_ASSERT_OPENAL();
		return volume;
	}

	void wm_openal_audio_listener::set_volume(const float volume) {
		alListenerf(AL_GAIN, volume);
		WM_ASSERT_OPENAL();
	}

	glm::vec3 wm_openal_audio_listener::get_position() const {
		glm::vec3 position;
		alGetListener3f(AL_POSITION, &position.x, &position.y, &position.z);
		WM_ASSERT_OPENAL();
		return position;
	}

	void wm_openal_audio_listener::set_position(const glm::vec3& position) {
		alListener3f(AL_POSITION, position.x, position.y, position.z);
		WM_ASSERT_OPENAL();
	}

	glm::vec3 wm_openal_audio_listener::get_velocity() const {
		glm::vec3 velocity;
		alGetListener3f(AL_VELOCITY, &velocity.x, &velocity.y, &velocity.z);
		WM_ASSERT_OPENAL();
		return velocity;
	}

	void wm_openal_audio_listener::set_velocity(const glm::vec3& velocity) {
		alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
		WM_ASSERT_OPENAL();
	}

	glm::vec3 wm_openal_audio_listener::get_forward() const {
		ALfloat orientation[6];
		alGetListenerfv(AL_ORIENTATION, orientation);
		WM_ASSERT_OPENAL();
		return glm::vec3(orientation[0], orientation[1], orientation[2]);
	}

	glm::vec3 wm_openal_audio_listener::get_up() const {
		ALfloat orientation[6];
		alGetListenerfv(AL_ORIENTATION, orientation);
		WM_ASSERT_OPENAL();
		return glm::vec3(orientation[3], orientation[4], orientation[5]);
	}

	void wm_openal_audio_listener::set_orientation(const glm::vec3& forward, const glm::vec3& up) {
		ALfloat orientation[] = {forward.x, forward.y, forward.z, up.x, up.y, up.z};
		alListenerfv(AL_ORIENTATION, orientation);
		WM_ASSERT_OPENAL();
	}

}
