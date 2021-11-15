#pragma once

#include "window_event.h"
#include "../../core/key.h"

namespace wm {

	class WM_PUBLIC window_content_scale_event: public window_event {
	private:
		glm::vec2 scale;
	public:
		static key<window_content_scale_event> get_key();

		window_content_scale_event(const ptr<wm::window> window, const glm::vec2 scale);
		glm::vec2 get_new_scale() const;
	};

}
