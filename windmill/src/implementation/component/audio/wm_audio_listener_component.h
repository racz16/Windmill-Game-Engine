#pragma once

#include "component/audio/audio_listener_component.h"
#include "scene/event/transform/transform_changed_event.h"
#include "audio/audio_listener.h"

namespace wm {

	class wm_audio_listener_component: public audio_listener_component {
	private:
		ptr<audio_listener> listener{nullptr};
		glm::vec3 previous_position;
	protected:
		const ptr<audio_listener_component> get_ptr() const;
	public:
		wm_audio_listener_component();
		void update() override;
		bool is_the_main_audio_listener() const override;
		~wm_audio_listener_component();
	};

}
