#include "wm_openal_audio_source.h"
#include "wm_openal_audio_context.h"

namespace wm {

	wm_openal_audio_source::wm_openal_audio_source() {
		alGenSources(1, &id);
		WM_ASSERT_OPENAL();
	}

	std::any wm_openal_audio_source::get_native_id() const {
		return id;
	}

	ptr<audio_buffer> wm_openal_audio_source::get_buffer() const {
		return buffer;
	}

	void wm_openal_audio_source::set_buffer(const ptr<audio_buffer> buffer) {
		this->buffer = buffer;
		alSourcei(id, AL_BUFFER, std::any_cast<ALuint>(buffer->get_native_id()));
		WM_ASSERT_OPENAL();
	}

	float wm_openal_audio_source::get_volume() const {
		ALfloat volume;
		alGetSourcef(id, AL_GAIN, &volume);
		WM_ASSERT_OPENAL();
		return volume;
	}

	void wm_openal_audio_source::set_volume(const float volume) {
		alSourcef(id, AL_GAIN, volume);
		WM_ASSERT_OPENAL();
	}

	float wm_openal_audio_source::get_speed() const {
		ALfloat speed;
		alGetSourcef(id, AL_PITCH, &speed);
		WM_ASSERT_OPENAL();
		return speed;
	}

	void wm_openal_audio_source::set_speed(const float speed) {
		alSourcef(id, AL_PITCH, speed);
		WM_ASSERT_OPENAL();
	}

	bool wm_openal_audio_source::is_loop() const {
		ALint loop;
		alGetSourcei(id, AL_LOOPING, &loop);
		WM_ASSERT_OPENAL();
		return loop == AL_TRUE;
	}

	void wm_openal_audio_source::set_loop(const bool loop) {
		alSourcei(id, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
		WM_ASSERT_OPENAL();
	}

	void wm_openal_audio_source::play() const {
		alSourcePlay(id);
		WM_ASSERT_OPENAL();
	}

	void wm_openal_audio_source::stop() const {
		alSourceStop(id);
		WM_ASSERT_OPENAL();
	}

	void wm_openal_audio_source::pause() const {
		alSourcePause(id);
		WM_ASSERT_OPENAL();
	}

	void wm_openal_audio_source::rewind() const {
		alSourceRewind(id);
		WM_ASSERT_OPENAL();
	}

	audio_source_state wm_openal_audio_source::get_state() const {
		ALint state;
		alGetSourcei(id, AL_SOURCE_STATE, &state);
		WM_ASSERT_OPENAL();
		return wm_openal_audio_context::source_state_int_to_enum(state);
	}

	glm::vec3 wm_openal_audio_source::get_position() const {
		glm::vec3 position;
		alGetSource3f(id, AL_POSITION, &position.x, &position.y, &position.z);
		WM_ASSERT_OPENAL();
		return position;
	}

	void wm_openal_audio_source::set_position(const glm::vec3& position) {
		alSource3f(id, AL_POSITION, position.x, position.y, position.z);
		WM_ASSERT_OPENAL();
	}

	glm::vec3 wm_openal_audio_source::get_forward() const {
		glm::vec3 forward;
		alGetSource3f(id, AL_DIRECTION, &forward.x, &forward.y, &forward.z);
		WM_ASSERT_OPENAL();
		return forward;
	}

	void wm_openal_audio_source::set_forward(const glm::vec3& forward) {
		alSource3f(id, AL_DIRECTION, forward.x, forward.y, forward.z);
		WM_ASSERT_OPENAL();
	}

	glm::vec3 wm_openal_audio_source::get_velocity() const {
		glm::vec3 velocity;
		alGetSource3f(id, AL_VELOCITY, &velocity.x, &velocity.y, &velocity.z);
		WM_ASSERT_OPENAL();
		return velocity;
	}

	void wm_openal_audio_source::set_velocity(const glm::vec3& velocity) {
		alSource3f(id, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
		WM_ASSERT_OPENAL();
	}

	float wm_openal_audio_source::get_max_distance() const {
		float max_distance;
		alGetSourcef(id, AL_MAX_DISTANCE, &max_distance);
		WM_ASSERT_OPENAL();
		return max_distance;
	}

	void wm_openal_audio_source::set_max_distance(const float max_distance) {
		alSourcef(id, AL_MAX_DISTANCE, max_distance);
		WM_ASSERT_OPENAL();
	}

	float wm_openal_audio_source::get_rolloff_factor() const {
		float rolloff_factor;
		alGetSourcef(id, AL_ROLLOFF_FACTOR, &rolloff_factor);
		WM_ASSERT_OPENAL();
		return rolloff_factor;
	}

	void wm_openal_audio_source::set_rolloff_factor(const float rolloff_factor) {
		alSourcef(id, AL_ROLLOFF_FACTOR, rolloff_factor);
		WM_ASSERT_OPENAL();
	}

	float wm_openal_audio_source::get_reference_distance() const {
		float reference_distance;
		alGetSourcef(id, AL_REFERENCE_DISTANCE, &reference_distance);
		WM_ASSERT_OPENAL();
		return reference_distance;
	}

	void wm_openal_audio_source::set_reference_distance(const float reference_distance) {
		alSourcef(id, AL_REFERENCE_DISTANCE, reference_distance);
		WM_ASSERT_OPENAL();
	}

	float wm_openal_audio_source::get_min_volume() const {
		float min_volume;
		alGetSourcef(id, AL_MIN_GAIN, &min_volume);
		WM_ASSERT_OPENAL();
		return min_volume;
	}

	void wm_openal_audio_source::set_min_volume(const float min_volume) {
		alSourcef(id, AL_MIN_GAIN, min_volume);
		WM_ASSERT_OPENAL();
	}

	float wm_openal_audio_source::get_max_volume() const {
		float max_volume;
		alGetSourcef(id, AL_MAX_GAIN, &max_volume);
		WM_ASSERT_OPENAL();
		return max_volume;
	}

	void wm_openal_audio_source::set_max_volume(const float max_volume) {
		alSourcef(id, AL_MAX_GAIN, max_volume);
		WM_ASSERT_OPENAL();
	}

	float wm_openal_audio_source::get_cone_outer_volume() const {
		float cone_outer_volume;
		alGetSourcef(id, AL_CONE_OUTER_GAIN, &cone_outer_volume);
		WM_ASSERT_OPENAL();
		return cone_outer_volume;
	}

	void wm_openal_audio_source::set_cone_outer_volume(const float cone_outer_volume) {
		alSourcef(id, AL_CONE_OUTER_GAIN, cone_outer_volume);
		WM_ASSERT_OPENAL();
	}

	float wm_openal_audio_source::get_cone_inner_angle() const {
		float cone_inner_angle;
		alGetSourcef(id, AL_CONE_INNER_ANGLE, &cone_inner_angle);
		WM_ASSERT_OPENAL();
		return cone_inner_angle;
	}

	void wm_openal_audio_source::set_cone_inner_angle(const float cone_inner_angle) {
		alSourcef(id, AL_CONE_INNER_ANGLE, cone_inner_angle);
		WM_ASSERT_OPENAL();
	}

	float wm_openal_audio_source::get_cone_outer_angle() const {
		float cone_outer_angle;
		alGetSourcef(id, AL_CONE_OUTER_ANGLE, &cone_outer_angle);
		WM_ASSERT_OPENAL();
		return cone_outer_angle;
	}

	void wm_openal_audio_source::set_cone_outer_angle(const float cone_outer_angle) {
		alSourcef(id, AL_CONE_OUTER_ANGLE, cone_outer_angle);
		WM_ASSERT_OPENAL();
	}

	wm_openal_audio_source::~wm_openal_audio_source() {
		alDeleteSources(1, &id);
	}

}
