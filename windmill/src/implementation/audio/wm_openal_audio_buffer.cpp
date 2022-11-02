#include "wm_openal_audio_buffer.h"
#include "defines/debug_defines.h"
#include "wm_openal_audio_context.h"

namespace wm {

	wm_openal_audio_buffer::wm_openal_audio_buffer() {
		alGenBuffers(1, &id);
		WM_ASSERT_OPENAL();
	}

	std::any wm_openal_audio_buffer::get_native_id() const {
		return id;
	}

	int32_t wm_openal_audio_buffer::get_frequency_hz() const {
		ALint frequency;
		alGetBufferi(id, AL_FREQUENCY, &frequency);
		WM_ASSERT_OPENAL();
		return frequency;
	}

	int32_t wm_openal_audio_buffer::get_bit_depth() const {
		ALint bit_depth;
		alGetBufferi(id, AL_BITS, &bit_depth);
		WM_ASSERT_OPENAL();
		return bit_depth;
	}

	int32_t wm_openal_audio_buffer::get_channel_count() const {
		ALint channel_count;
		alGetBufferi(id, AL_CHANNELS, &channel_count);
		WM_ASSERT_OPENAL();
		return channel_count;
	}

	int32_t wm_openal_audio_buffer::get_data_size_bytes() const {
		ALint data_size;
		alGetBufferi(id, AL_SIZE, &data_size);
		WM_ASSERT_OPENAL();
		return data_size;
	}

	ptr<sound> wm_openal_audio_buffer::get_sound() const {
		return sound;
	}

	void wm_openal_audio_buffer::set_sound(const ptr<wm::sound> sound) {
		this->sound = sound;
		alBufferData(
			id,
			sound->get_channel_count() == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16,
			&sound->get_data().front(),
			sound->get_data().size() * sizeof(uint16_t),
			sound->get_frequency_hz()
		);
		WM_ASSERT_OPENAL();
	}

	wm_openal_audio_buffer::~wm_openal_audio_buffer() {
		alDeleteBuffers(1, &id);
	}

}
