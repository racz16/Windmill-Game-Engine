#pragma once

#include "../core/key.h"
#include "../ptr/ptr.h"
#include "../resource/sound.h"

namespace wm {

	class WM_PUBLIC audio_buffer {
	public:
		static ptr<audio_buffer> create();
		static key<audio_buffer> get_key();

		virtual std::any get_native_id() const = 0;
		virtual int32_t get_frequency_hz() const = 0;
		virtual int32_t get_bit_depth() const = 0;
		virtual int32_t get_channel_count() const = 0;
		virtual int32_t get_data_size_bytes() const = 0;
		virtual ptr<sound> get_sound() const = 0;
		virtual void set_sound(const ptr<sound> sound) = 0;
		virtual ~audio_buffer() = default;
	};

}
