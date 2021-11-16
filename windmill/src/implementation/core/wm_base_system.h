#pragma once

#include "core/system.h"

namespace wm {

	class wm_base_system: public system {
	private:
		bool active = true;
	public:
		void update() override;
		bool is_active() const override;
		void set_active(const bool active) override;
	};

}
