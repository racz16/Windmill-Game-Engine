#pragma once

#include "wm_direct3d11_rendering_context.h"

namespace wm {

#define DX_ID_OF(object, type) std::any_cast<type>(object->get_native_handle())

#ifdef WM_BUILD_DEBUG
	#define DX_CALL(expression) expression; wm_direct3d11_rendering_context::dxgi_message_callback(__FUNCTION__, __LINE__)
	#define DX_LABEL(object, label) wm_direct3d11_rendering_context::set_debug_label(object, label)
	#define DX_LABEL_2(object, label) object->set_debug_label(label)
	#define DX_GROUP_START(name) wm_direct3d11_rendering_context::start_group(name)
	#define DX_GROUP_STOP() wm_direct3d11_rendering_context::stop_group()
	#define DX_MARKER(name) wm_direct3d11_rendering_context::set_marker(name)
#else
	#define DX_CALL(expression) expression
	#define DX_LABEL(object, label)
	#define DX_LABEL_2(object, label)
	#define DX_GROUP_START(name)
	#define DX_GROUP_STOP()
	#define DX_MARKER(name)
#endif

}
