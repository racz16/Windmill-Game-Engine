#pragma once

#include "../component.h"
#include "../../ptr/ptr.h"
#include "../../core/key.h"

namespace wm {

	class WM_PUBLIC audio_listener_component: public component {
	public:
		static ptr<audio_listener_component> create();
		static key<audio_listener_component> get_key();
		static key<ptr<audio_listener_component>> get_main_audio_listener_key();

		virtual void update() = 0;
		virtual bool is_the_main_audio_listener() const = 0;
	};

}
