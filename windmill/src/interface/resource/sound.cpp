#include "sound.h"

namespace wm {

	sound::sound(const std::vector<uint16_t>& data, const int32_t frequency_hz, const int32_t channel_count, float length_seconds)
		: data(data), frequency_hz(frequency_hz), channel_count(channel_count), length_seconds(length_seconds) {}

	std::vector<uint16_t> sound::get_data() const {
		return data;
	}

	int32_t sound::get_frequency_hz() const {
		return frequency_hz;
	}

	int32_t sound::get_channel_count() const {
		return channel_count;
	}

	float sound::get_length_seconds() const {
		return length_seconds;
	}

}
