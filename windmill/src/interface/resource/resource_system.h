#pragma once

#include "../core/system.h"
#include "../core/key.h"
#include "../ptr/ptr.h"
#include "image.h"
#include "mesh.h"
#include "sound.h"

namespace wm {

	class WM_PUBLIC resource_system: public system {
	public:
		static ptr<resource_system> create();
		static key<resource_system> get_key();

		virtual ptr<image> get_image(const std::string& file_path) = 0;
		virtual ptr<mesh> get_mesh(const std::string& file_path) = 0;
		virtual ptr<sound> get_sound(const std::string& file_path) = 0;
	};

}
