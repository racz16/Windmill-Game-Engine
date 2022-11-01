#pragma once

#include "../component.h"
#include "../../audio/audio_buffer.h"
#include "../../audio/audio_source_state.h"

namespace wm {

	class WM_PUBLIC audio_source_component: public component {
	public:
		static ptr<audio_source_component> create();
		static key<audio_source_component> get_key();

		virtual void update() = 0;
		virtual std::any get_native_id() const = 0;
		virtual ptr<audio_buffer> get_buffer() const = 0;
		virtual void set_buffer(const ptr<audio_buffer> buffer) = 0;
		virtual ptr<sound> get_sound() const = 0;
		virtual float get_volume() const = 0;
		virtual void set_volume(const float volume) = 0;
		virtual float get_speed() const = 0;
		virtual void set_speed(const float speed) = 0;
		virtual bool is_loop() const = 0;
		virtual void set_loop(const bool loop) = 0;
		virtual void play() const = 0;
		virtual void stop() const = 0;
		virtual void pause() const = 0;
		virtual void rewind() const = 0;
		virtual audio_source_state get_state() const = 0;
		virtual glm::vec3 get_position() const = 0;
		virtual glm::vec3 get_forward() const = 0;
		virtual glm::vec3 get_velocity() const = 0;
		virtual float get_max_distance() const = 0;
		virtual void set_max_distance(const float max_distance) = 0;
		virtual float get_rolloff_factor() const = 0;
		virtual void set_rolloff_factor(const float rolloff_factor) = 0;
		virtual float get_reference_distance() const = 0;
		virtual void set_reference_distance(const float reference_distance) = 0;
		virtual float get_min_volume() const = 0;
		virtual void set_min_volume(const float min_volume) = 0;
		virtual float get_max_volume() const = 0;
		virtual void set_max_volume(const float max_volume) = 0;
		virtual float get_cone_outer_volume() const = 0;
		virtual void set_cone_outer_volume(const float cone_outer_volume) = 0;
		virtual float get_cone_inner_angle() const = 0;
		virtual void set_cone_inner_angle(const float cone_inner_angle) = 0;
		virtual float get_cone_outer_angle() const = 0;
		virtual void set_cone_outer_angle(const float cone_outer_angle) = 0;
	};

}
