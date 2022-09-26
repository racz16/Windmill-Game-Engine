#pragma once

#include "../../event/event.h"
#include "../../ptr/ptr.h"
#include "../../core/key.h"

namespace wm {

	class component;

	class WM_PUBLIC component_event: public event {
	private:
		ptr<wm::component> component;
	public:
		static key<component_event> get_key();

		component_event(const ptr<wm::component> component);
		ptr<wm::component> get_component() const;
	};

}
