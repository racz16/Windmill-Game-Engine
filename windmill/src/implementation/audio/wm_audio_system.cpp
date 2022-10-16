#include "wm_audio_system.h"
#include "defines/log_defines.h"

namespace wm {

	wm_audio_system::wm_audio_system() {
		device = alcOpenDevice(nullptr);
		WM_ASSERT(device);
		context = alcCreateContext(device, nullptr);
		WM_ASSERT(context);
		WM_ASSERT(alcMakeContextCurrent(context) == ALC_TRUE);

		//stb_vorbis* sv = stb_vorbis_open_filename("res/audio/rain.ogg", nullptr, nullptr);
		//stb_vorbis_info svi = stb_vorbis_get_info(sv);

		//stb_vorbis_close(sv);

		WM_LOG_INFO_1("audio system created");
	}

	wm_audio_system::~wm_audio_system() {
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(context);
		context = nullptr;
		alcCloseDevice(device);
		device = nullptr;
		WM_LOG_INFO_1("audio system destroyed");
	}

}
