#include "audio_buffer.h"
#include "../defines/code_generation_defines.h"
#include "../../implementation/audio/wm_openal_audio_buffer.h"

namespace wm {

	WM_CREATE(audio_buffer, wm_openal_audio_buffer);

	WM_GET_KEY(audio_buffer, "WM_AUDIO_BUFFER");

}
