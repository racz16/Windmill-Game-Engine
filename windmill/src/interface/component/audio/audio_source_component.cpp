#include "audio_source_component.h"
#include "defines/code_generation_defines.h"

#include "../../../implementation/component/audio/wm_audio_source_component.h"

namespace wm {

	WM_CREATE_WITH_INIT(audio_source_component, wm_audio_source_component);

	WM_GET_KEY(audio_source_component, "WM_AUDIO_SOURCE_COMPONENT");

}
