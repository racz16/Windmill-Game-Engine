#pragma once

#include "audio/audio_system.h"
#include "audio/audio_context.h"

namespace wm {

	class wm_audio_system: public audio_system {
	private:
		ptr<audio_context> context{nullptr};
		std::vector<ptr<audio_source_component>> audio_source_components;
	public:
		wm_audio_system();
		void update() override;
		void add_audio_source_component(const ptr<audio_source_component> audio_source_component) override;
		void remove_audio_source_component(const ptr<audio_source_component> audio_source_component) override;
		float get_volume() const override;
		void set_volume(const float value) override;
		float get_doppler_factor() const override;
		void set_doppler_factor(const float value) override;
		float get_speed_of_sound() const override;
		void set_speed_of_sound(const float value) override;
		audio_distance_model get_distance_model() const override;
		void set_distance_model(const audio_distance_model distance_model) override;
		std::string get_vendor() const override;
		std::string get_version() const override;
		std::string get_renderer() const override;
		~wm_audio_system();
	};

}
