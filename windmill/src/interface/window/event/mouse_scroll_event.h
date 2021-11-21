#pragma once

#include "window_event.h"
#include "../../core/key.h"

namespace wm {

	class WM_PUBLIC mouse_scroll_event: public window_event {
	private:
		glm::dvec2 offset;
	public:
		static key<mouse_scroll_event> get_key();

		mouse_scroll_event(const ptr<wm::window> window, const glm::dvec2 offset);
		glm::dvec2 get_offset() const;
	};

}
