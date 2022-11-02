#pragma once

#include "../defines/general_defines.h"

namespace wm {

	class WM_PUBLIC sound {
	private:
		std::vector<uint16_t> data;
		int32_t frequency_hz;
		int32_t channel_count;
		float length_seconds;
	public:
		sound(const std::vector<uint16_t>& data, const int32_t frequency_hz, const int32_t channel_count, const float length_seconds);
		std::vector<uint16_t> get_data() const;
		int32_t get_frequency_hz() const;
		int32_t get_channel_count() const;
		float get_length_seconds() const;
	};

}
