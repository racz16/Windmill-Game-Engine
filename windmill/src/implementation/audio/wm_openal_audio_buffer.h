#pragma once

#include "audio/audio_buffer.h"

namespace wm {

	class wm_openal_audio_buffer: public audio_buffer {
	private:
		ALuint id;
		ptr<sound> sound{nullptr};
	public:
		wm_openal_audio_buffer();
		std::any get_native_id() const override;
		ptr<wm::sound> get_sound() const override;
		void set_sound(const ptr<wm::sound> sound) override;
		int32_t get_frequency_hz() const override;
		int32_t get_bit_depth() const override;
		int32_t get_channel_count() const override;
		int32_t get_data_size_bytes() const override;
		~wm_openal_audio_buffer() override;
	};

}
