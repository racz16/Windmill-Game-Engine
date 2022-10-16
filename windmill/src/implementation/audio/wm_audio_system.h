#pragma once

#include "audio/audio_system.h"

namespace wm {

	class wm_audio_system: public audio_system {
	private:
		ALCdevice* device;
		ALCcontext* context;
	public:
		wm_audio_system();
		~wm_audio_system();
	};

}
