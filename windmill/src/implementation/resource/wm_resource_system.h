#pragma once

#include "resource/resource_system.h"

namespace wm {

	class wm_resource_system: public resource_system {
	public:
		wm_resource_system();
		ptr<image> get_image(const std::string& file_path) override;
		ptr<mesh> get_mesh(const std::string& file_path) override;
		~wm_resource_system();
	};

}
