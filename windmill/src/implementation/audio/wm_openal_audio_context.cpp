#include "wm_openal_audio_context.h"
#include "defines/log_defines.h"
#include "component/audio/audio_listener_component.h"

namespace wm {

	void wm_openal_audio_context::handle_openal_error(const std::string& function, const int32_t line) {
		auto error = alGetError();
		if(error != AL_NO_ERROR) {
			std::string message = "OpenAL error: ";
			if(error == AL_INVALID_NAME) {
				message += "INVALID_NAME";
			} else if(error == AL_INVALID_ENUM) {
				message += "INVALID_ENUM";
			} else if(error == AL_INVALID_VALUE) {
				message += "INVALID_VALUE";
			} else if(error == AL_INVALID_OPERATION) {
				message += "INVALID_OPERATION";
			} else if(error == AL_OUT_OF_MEMORY) {
				message += "OUT_OF_MEMORY";
			}
			throw wm::windmill_error(message, function, line);
		}
	}

	void wm_openal_audio_context::handle_openal_error(ALCdevice* device, const std::string& function, const int32_t line) {
		auto error = alcGetError(device);
		if(error != ALC_NO_ERROR) {
			std::string message = "OpenAL Context error: ";
			if(error == ALC_INVALID_DEVICE) {
				message += "INVALID_DEVICE";
			} else if(error == AL_INVALID_ENUM) {
				message += "INVALID_ENUM";
			} else if(error == AL_INVALID_VALUE) {
				message += "INVALID_VALUE";
			} else if(error == ALC_INVALID_CONTEXT) {
				message += "INVALID_CONTEXT";
			} else if(error == AL_OUT_OF_MEMORY) {
				message += "OUT_OF_MEMORY";
			}
			throw wm::windmill_error(message, function, line);
		}
	}

	ALenum wm_openal_audio_context::distance_model_enum_to_int(const audio_distance_model distance_model) {
		switch(distance_model) {
			case audio_distance_model::inverse_distance: return AL_INVERSE_DISTANCE;
			case audio_distance_model::inverse_distance_clamped: return AL_INVERSE_DISTANCE_CLAMPED;
			case audio_distance_model::linear_distance: return AL_LINEAR_DISTANCE;
			case audio_distance_model::linear_distance_clamped: return AL_LINEAR_DISTANCE_CLAMPED;
			case audio_distance_model::exponent_distance: return AL_EXPONENT_DISTANCE;
			case audio_distance_model::exponent_distance_clamped: return AL_EXPONENT_DISTANCE_CLAMPED;
			case audio_distance_model::none: return AL_NONE;
			default: WM_THROW_ERROR("invalid audio distance model");
		}
	}

	audio_distance_model wm_openal_audio_context::distance_model_int_to_enum(const ALint distance_model) {
		switch(distance_model) {
			case AL_INVERSE_DISTANCE: return audio_distance_model::inverse_distance;
			case AL_INVERSE_DISTANCE_CLAMPED: return audio_distance_model::inverse_distance_clamped;
			case AL_LINEAR_DISTANCE: return audio_distance_model::linear_distance;
			case AL_LINEAR_DISTANCE_CLAMPED: return audio_distance_model::linear_distance_clamped;
			case AL_EXPONENT_DISTANCE: return audio_distance_model::exponent_distance;
			case AL_EXPONENT_DISTANCE_CLAMPED: return audio_distance_model::exponent_distance_clamped;
			case AL_NONE: return audio_distance_model::none;
			default: WM_THROW_ERROR("invalid audio distance model");
		}
	}

	audio_source_state wm_openal_audio_context::source_state_int_to_enum(const ALint state) {
		switch(state) {
			case AL_INITIAL: return audio_source_state::initial;
			case AL_PLAYING: return audio_source_state::playing;
			case AL_PAUSED: return audio_source_state::paused;
			case AL_STOPPED: return audio_source_state::stopped;
			default: WM_THROW_ERROR("invalid audio source state");
		}
	}

	ALenum wm_openal_audio_context::source_state_enum_to_int(const audio_source_state state) {
		switch(state) {
			case audio_source_state::initial: return AL_INITIAL;
			case audio_source_state::playing: return AL_PLAYING;
			case audio_source_state::paused: return AL_PAUSED;
			case audio_source_state::stopped: return AL_STOPPED;
			default: WM_THROW_ERROR("invalid audio source state");
		}
	}

	wm_openal_audio_context::wm_openal_audio_context() {
		device = alcOpenDevice(nullptr);
		WM_ASSERT(device);
		context = alcCreateContext(device, nullptr);
		WM_ASSERT_OPENAL_ALC(device);
		WM_ASSERT(context);
		WM_ASSERT(alcMakeContextCurrent(context) == ALC_TRUE);
		WM_ASSERT_OPENAL_ALC(device);

		WM_LOG_INFO_2("OpenAL vendor: " + get_vendor());
		WM_LOG_INFO_2("OpenAL version: " + get_version());
		WM_LOG_INFO_2("OpenAL renderer: " + get_renderer());
		WM_LOG_INFO_1("OpenAL audio context created");
	}

	void wm_openal_audio_context::update_listener() const {
		if(engine::get_parameters().contains(audio_listener_component::get_main_audio_listener_key())) {
			auto listener = engine::get_parameters().get(audio_listener_component::get_main_audio_listener_key());
			if(listener->is_active() && listener->get_node().is_valid()) {
				listener->update();
				return;
			}
		}
		alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
		WM_ASSERT_OPENAL();
		alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
		WM_ASSERT_OPENAL();
		ALfloat orientation[] = {0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f};
		alListenerfv(AL_ORIENTATION, orientation);
		WM_ASSERT_OPENAL();
	}

	void wm_openal_audio_context::update_source(const ptr<audio_source_component> source) const {
		source->update();
	}

	float wm_openal_audio_context::get_volume() const {
		float volume;
		alGetListenerf(AL_GAIN, &volume);
		WM_ASSERT_OPENAL();
		return volume;
	}

	void wm_openal_audio_context::set_volume(const float volume) {
		alListenerf(AL_GAIN, volume);
		WM_ASSERT_OPENAL();
	}

	float wm_openal_audio_context::get_doppler_factor() const {
		ALfloat doppler_factor = alGetFloat(AL_DOPPLER_FACTOR);
		WM_ASSERT_OPENAL();
		return doppler_factor;
	}

	void wm_openal_audio_context::set_doppler_factor(const float doppler_factor) {
		alDopplerFactor(doppler_factor);
		WM_ASSERT_OPENAL();
	}

	float wm_openal_audio_context::get_speed_of_sound() const {
		ALfloat speed_of_sound = alGetFloat(AL_SPEED_OF_SOUND);
		WM_ASSERT_OPENAL();
		return speed_of_sound;
	}

	void wm_openal_audio_context::set_speed_of_sound(const float speed_of_sound) {
		alSpeedOfSound(speed_of_sound);
		WM_ASSERT_OPENAL();
	}

	audio_distance_model wm_openal_audio_context::get_distance_model() const {
		ALint distance_model = alGetInteger(AL_DISTANCE_MODEL);
		WM_ASSERT_OPENAL();
		return distance_model_int_to_enum(distance_model);
	}

	void wm_openal_audio_context::set_distance_model(const audio_distance_model distance_model) {
		alDistanceModel(distance_model_enum_to_int(distance_model));
		WM_ASSERT_OPENAL();
	}

	std::string wm_openal_audio_context::get_vendor() const {
		auto vendor = alGetString(AL_VENDOR);
		WM_ASSERT_OPENAL();
		return vendor;
	}

	std::string wm_openal_audio_context::get_version() const {
		auto version = alGetString(AL_VERSION);
		WM_ASSERT_OPENAL();
		return version;
	}

	std::string wm_openal_audio_context::get_renderer() const {
		auto renderer = alGetString(AL_RENDERER);
		WM_ASSERT_OPENAL();
		return renderer;
	}

	wm_openal_audio_context::~wm_openal_audio_context() {
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(context);
		context = nullptr;
		alcCloseDevice(device);
		device = nullptr;

		WM_LOG_INFO_1("OpenAL audio context destroyed");
	}

}
