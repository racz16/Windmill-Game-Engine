#pragma once

namespace wm {

#define GL_ID_OF(object) std::any_cast<GLuint>(object->get_native_handle())

#ifdef WM_BUILD_DEBUG
	#define GL_LABEL(type, id, label) glObjectLabel(type, id, -1, label.c_str())
	#define GL_LABEL_2(object, label) object->set_debug_label(label)
	#define GL_GROUP_START(name) glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, name)
	#define GL_GROUP_STOP() glPopDebugGroup()
	#define GL_MARKER(name) GL_GROUP_START(name);GL_GROUP_STOP()
#else
	#define GL_LABEL(type, id, label)
	#define GL_LABEL_2(object, label)
	#define GL_GROUP_START(name)
	#define GL_GROUP_STOP()
	#define GL_MARKER(name)
#endif

}
