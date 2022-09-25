#include "node.h"
#include "../core/engine.h"
#include "defines/code_generation_defines.h"

#include "../../implementation/scene/wm_node.h"

namespace wm {

	WM_CREATE_WITH_INIT(node, wm_node);

	WM_GET_KEY(node, "WM_NODE");

	WM_GET_PTR(node, node);

	void node::initialize(const int32_t id) {
		this->id = id;
	}

	void node::initialize_node(const ptr<transform> transform) {
		engine::get_scene_system()->initialize_node(get_ptr(), transform);
	}

	node::~node() {
		auto node = get_ptr();
		engine::get_scene_system()->cleanup(node);
	}

}
