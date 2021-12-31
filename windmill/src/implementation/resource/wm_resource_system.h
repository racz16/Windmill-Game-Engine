#pragma once

#include "resource/resource_system.h"

#include "../core/wm_base_system.h"

namespace wm {

	class wm_resource_system: public wm_base_system, public resource_system {
	public:
		ptr<image> get_image(const std::string& file_path) override;
		ptr<mesh> get_mesh(const std::string& file_path) override;

		void update() override { return wm_base_system::update(); }
		bool is_active() const override { return wm_base_system::is_active(); }
		void set_active(const bool active) override { wm_base_system::set_active(active); }
	};

}
