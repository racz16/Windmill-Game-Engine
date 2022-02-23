#pragma once

#include "scene/scene_system.h"

#include "../core/wm_base_system.h"

namespace wm {

	class wm_simple_scene_system: public wm_base_system, public scene_system {
	public:
		wm_simple_scene_system();
		void update() override;
		~wm_simple_scene_system() override;

		bool is_active() const override { return wm_base_system::is_active(); }
		void set_active(const bool active) override { wm_base_system::set_active(active); }
	};

}
