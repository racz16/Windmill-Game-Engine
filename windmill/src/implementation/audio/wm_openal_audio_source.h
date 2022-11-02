#pragma once

#include "audio/audio_source.h"

namespace wm {

	class wm_openal_audio_source: public audio_source {
	private:
		ALuint id;
		ptr<audio_buffer> buffer{nullptr};
	public:
		wm_openal_audio_source();
		std::any get_native_id() const override;
		ptr<audio_buffer> get_buffer() const override;
		void set_buffer(const ptr<audio_buffer> buffer) override;
		float get_volume() const override;
		void set_volume(const float volume) override;
		float get_speed() const override;
		void set_speed(const float speed) override;
		bool is_loop() const override;
		void set_loop(const bool loop) override;
		void play() const override;
		void stop() const override;
		void pause() const override;
		void rewind() const override;
		audio_source_state get_state() const override;
		glm::vec3 get_position() const override;
		void set_position(const glm::vec3& position) override;
		glm::vec3 get_forward() const override;
		void set_forward(const glm::vec3& forward) override;
		glm::vec3 get_velocity() const override;
		void set_velocity(const glm::vec3& velocity) override;
		float get_max_distance() const override;
		void set_max_distance(const float max_distance) override;
		float get_rolloff_factor() const override;
		void set_rolloff_factor(const float rolloff_factor) override;
		float get_reference_distance() const override;
		void set_reference_distance(const float reference_distance) override;
		float get_min_volume() const override;
		void set_min_volume(const float min_volume) override;
		float get_max_volume() const override;
		void set_max_volume(const float max_volume) override;
		float get_cone_outer_volume() const override;
		void set_cone_outer_volume(const float cone_outer_volume) override;
		float get_cone_inner_angle() const override;
		void set_cone_inner_angle(const float cone_inner_angle) override;
		float get_cone_outer_angle() const override;
		void set_cone_outer_angle(const float cone_outer_angle) override;
		~wm_openal_audio_source();
	};

}
