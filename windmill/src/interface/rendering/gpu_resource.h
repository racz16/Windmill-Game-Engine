#pragma once

#include "../defines/general_defines.h"

namespace wm {

	class WM_PUBLIC gpu_resource {
	public:
		virtual std::any get_native_handle() const = 0;
		virtual int32_t get_data_size() const = 0;
	#ifdef WM_BUILD_DEBUG
		virtual void set_debug_label(const std::string& label) = 0;
	#endif
		virtual ~gpu_resource() = default;
	};

}
