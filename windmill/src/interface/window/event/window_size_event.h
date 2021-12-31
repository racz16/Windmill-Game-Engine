#pragma once

#include "../../event/event.h"
#include "../../core/key.h"

namespace wm {

	class WM_PUBLIC window_size_event: public event {
	private:
		glm::ivec2 size;
	public:
		static key<window_size_event> get_key();

		window_size_event(const glm::ivec2& size);
		glm::ivec2 get_new_size() const;
	};

}
