#include "component_event.h"
#include "defines/code_generation_defines.h"

namespace wm {

	WM_GET_KEY(component_event, "WM_COMPONENT_EVENT");

	component_event::component_event(const ptr<wm::component> component): event(true), component(component) {}

	ptr<wm::component> component_event::get_component() const {
		return component;
	}
}
