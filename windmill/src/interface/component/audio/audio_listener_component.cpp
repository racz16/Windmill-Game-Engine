#include "audio_listener_component.h"
#include "defines/code_generation_defines.h"

#include "../../../implementation/component/audio/wm_audio_listener_component.h"

namespace wm {

	WM_CREATE_WITH_INIT(audio_listener_component, wm_audio_listener_component);

	WM_GET_KEY(audio_listener_component, "WM_AUDIO_LISTENER_COMPONENT");

	key<ptr<audio_listener_component>> audio_listener_component::get_main_audio_listener_key() {
		static const wm::key<ptr<audio_listener_component>> key("WM_MAIN_AUDIO_LISTENER_COMPONENT");
		return key;
	}

}
