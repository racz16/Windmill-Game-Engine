#pragma once

#include "../component/audio/audio_source_component.h"
#include "../audio/audio_distance_model.h"

namespace wm {

	class WM_PUBLIC audio_context {
	public:
		static ptr<audio_context> create();
		static key<audio_context> get_key();

		virtual void update_listener() const = 0;
		virtual void update_source(const ptr<audio_source_component> source) const = 0;
		virtual float get_volume() const = 0;
		virtual void set_volume(const float value) = 0;
		virtual float get_doppler_factor() const = 0;
		virtual void set_doppler_factor(const float value) = 0;
		virtual float get_speed_of_sound() const = 0;
		virtual void set_speed_of_sound(const float value) = 0;
		virtual audio_distance_model get_distance_model() const = 0;
		virtual void set_distance_model(const audio_distance_model distance_model) = 0;
		virtual std::string get_vendor() const = 0;
		virtual std::string get_version() const = 0;
		virtual std::string get_renderer() const = 0;
		virtual ~audio_context() = default;
	};

}
