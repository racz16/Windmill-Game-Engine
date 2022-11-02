#include "audio_listener.h"
#include "../defines/code_generation_defines.h"
#include "../../implementation/audio/wm_openal_audio_listener.h"

namespace wm {

	WM_CREATE(audio_listener, wm_openal_audio_listener);

	WM_GET_KEY(audio_listener, "WM_AUDIO_LISTENER");

}
