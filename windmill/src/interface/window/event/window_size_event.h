#pragma once

#include "window_event.h"
#include "../../core/key.h"

namespace wm {

	class WM_PUBLIC window_size_event: public window_event {
	private:
		glm::ivec2 size;
	public:
		static key<window_size_event> get_key();

		window_size_event(const ptr<wm::window> window, const glm::ivec2 size);
		glm::ivec2 get_new_size() const;
	};

}
