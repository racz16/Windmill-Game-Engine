#pragma once

#include "component/audio/audio_source_component.h"
#include "audio/audio_buffer.h"
#include "audio/audio_source.h"

namespace wm {

	class wm_audio_source_component: public audio_source_component {
	private:
		ptr<audio_source> source{nullptr};
		glm::vec3 previous_position;
	protected:
		ptr<audio_source_component> get_ptr() const;
		void added_to_node(const ptr<node> node);
		void removed_from_node(const ptr<node> node);
	public:
		void initialize(const int32_t id) override;
		void update() override;
		std::any get_native_id() const override;
		ptr<wm::audio_buffer> get_buffer() const override;
		void set_buffer(const ptr<wm::audio_buffer> buffer) override;
		ptr<sound> get_sound() const override;
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
		glm::vec3 get_forward() const override;
		glm::vec3 get_velocity() const override;
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
		~wm_audio_source_component();
	};

}
