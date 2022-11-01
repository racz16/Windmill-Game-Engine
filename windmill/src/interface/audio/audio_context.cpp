#include "audio_context.h"
#include "../defines/code_generation_defines.h"
#include "../../implementation/audio/wm_openal_audio_context.h"

namespace wm {

	WM_CREATE(audio_context, wm_openal_audio_context);

	WM_GET_KEY(audio_context, "WM_AUDIO_CONTEXT");

}
