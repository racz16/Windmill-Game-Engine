#pragma once

#include "audio/audio_context.h"

namespace wm {

	class wm_openal_audio_context: public audio_context {
	private:
		ALCdevice* device;
		ALCcontext* context;
	public:
		static void handle_openal_error(const std::string& function, const int32_t line);
		static void handle_openal_error(ALCdevice* device, const std::string& function, const int32_t line);
		static ALenum source_state_enum_to_int(const audio_source_state state);
		static audio_source_state source_state_int_to_enum(const ALint state);
		static ALenum distance_model_enum_to_int(const audio_distance_model distance_model);
		static audio_distance_model distance_model_int_to_enum(const ALint distance_model);

		wm_openal_audio_context();
		void update_listener() const override;
		void update_source(const ptr<audio_source_component> source) const override;
		float get_volume() const override;
		void set_volume(const float volume) override;
		float get_doppler_factor() const override;
		void set_doppler_factor(const float doppler_factor) override;
		float get_speed_of_sound() const override;
		void set_speed_of_sound(const float speed_of_sound) override;
		audio_distance_model get_distance_model() const override;
		void set_distance_model(const audio_distance_model distance_model) override;
		std::string get_vendor() const override;
		std::string get_version() const override;
		std::string get_renderer() const override;
		~wm_openal_audio_context();
	};

}
