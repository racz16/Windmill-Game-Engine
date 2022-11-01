#include "audio_source.h"
#include "../defines/code_generation_defines.h"
#include "../../implementation/audio/wm_openal_audio_source.h"

namespace wm {

	WM_CREATE(audio_source, wm_openal_audio_source);

	WM_GET_KEY(audio_source, "WM_AUDIO_SOURCE");

}
