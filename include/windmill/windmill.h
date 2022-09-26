#pragma once

#include <vector>
#include <unordered_set>
#include <chrono>
#include <any>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

//
//COMPONENT
//
#include "../windmill/src/interface/component/component.h"
//camera
#include "../windmill/src/interface/component/camera/camera_component.h"
#include "../windmill/src/interface/component/camera/projection_mode.h"
//event
#include "../windmill/src/interface/component/event/component_changed_event.h"
#include "../windmill/src/interface/component/event/component_destroyed_event.h"
#include "../windmill/src/interface/component/event/component_event.h"
//
//CORE
//
#include "../windmill/src/interface/core/application.h"
#include "../windmill/src/interface/core/engine.h"
#include "../windmill/src/interface/core/key.h"
#include "../windmill/src/interface/core/parameter_container.h"
#include "../windmill/src/interface/core/system.h"
#include "../windmill/src/interface/core/tag.h"
#include "../windmill/src/interface/core/utility.h"
#include "../windmill/src/interface/core/windmill_error.h"
//
//DEBUG
//
#include "../windmill/src/interface/debug/scoped_time_logger.h"
#include "../windmill/src/interface/debug/stopwatch.h"
//
//DEFINES
//
#include "../windmill/src/interface/defines/code_generation_defines.h"
#include "../windmill/src/interface/defines/debug_defines.h"
#include "../windmill/src/interface/defines/general_defines.h"
#include "../windmill/src/interface/defines/log_defines.h"
//
//EVENT
//
#include "../windmill/src/interface/event/event_listener.h"
#include "../windmill/src/interface/event/event_listener_base.h"
#include "../windmill/src/interface/event/event_system.h"
#include "../windmill/src/interface/event/event.h"
//
//LOG
//
#include "../windmill/src/interface/log/log_level.h"
#include "../windmill/src/interface/log/log_system.h"
#include "../windmill/src/interface/log/log_writer.h"
//
//PTR
//
#include "../windmill/src/interface/ptr/array_allocator_system.h"
#include "../windmill/src/interface/ptr/ptr.h"
#include "../windmill/src/interface/ptr/ptr_meta.h"
#include "../windmill/src/interface/ptr/vector_array.h"
//
//RENDERING
//
#include "../windmill/src/interface/rendering/rendering_system.h"
//
//SCENE
//
#include "../windmill/src/interface/scene/node.h"
#include "../windmill/src/interface/scene/scene_system.h"
#include "../windmill/src/interface/scene/transform.h"
//event
//node
#include "../windmill/src/interface/scene/event/node/node_child_changed_event.h"
#include "../windmill/src/interface/scene/event/node/node_destroyed_event.h"
#include "../windmill/src/interface/scene/event/node/node_event.h"
#include "../windmill/src/interface/scene/event/node/node_parent_changed_event.h"
#include "../windmill/src/interface/scene/event/node/node_tag_changed_event.h"
//transform
#include "../windmill/src/interface/scene/event/transform/transform_changed_event.h"
#include "../windmill/src/interface/scene/event/transform/transform_changed_type.h"
#include "../windmill/src/interface/scene/event/transform/transform_destroyed_event.h"
#include "../windmill/src/interface/scene/event/transform/transform_event.h"
//
//TIME
//
#include "../windmill/src/interface/time/time_system.h"
//
//WINDOW
//
#include "../windmill/src/interface/window/cursor_mode.h"
#include "../windmill/src/interface/window/cursor_shape.h"
#include "../windmill/src/interface/window/standard_cursor_shape.h"
#include "../windmill/src/interface/window/video_mode.h"
#include "../windmill/src/interface/window/window_system.h"
//event
#include "../windmill/src/interface/window/event/gamepad_connection_event.h"
#include "../windmill/src/interface/window/event/keyboard_button_event.h"
#include "../windmill/src/interface/window/event/keyboard_character_event.h"
#include "../windmill/src/interface/window/event/mouse_button_event.h"
#include "../windmill/src/interface/window/event/mouse_hover_event.h"
#include "../windmill/src/interface/window/event/mouse_position_event.h"
#include "../windmill/src/interface/window/event/mouse_scroll_event.h"
#include "../windmill/src/interface/window/event/window_closed_event.h"
#include "../windmill/src/interface/window/event/window_content_scale_event.h"
#include "../windmill/src/interface/window/event/window_drag_and_drop_event.h"
#include "../windmill/src/interface/window/event/window_focus_event.h"
#include "../windmill/src/interface/window/event/window_framebuffer_size_event.h"
#include "../windmill/src/interface/window/event/window_maximization_event.h"
#include "../windmill/src/interface/window/event/window_minimization_event.h"
#include "../windmill/src/interface/window/event/window_position_event.h"
#include "../windmill/src/interface/window/event/window_refresh_required_event.h"
#include "../windmill/src/interface/window/event/window_size_event.h"
//input
#include "../windmill/src/interface/window/input/axis_state.h"
#include "../windmill/src/interface/window/input/button_action.h"
#include "../windmill/src/interface/window/input/button_state.h"
#include "../windmill/src/interface/window/input/gamepad_axis.h"
#include "../windmill/src/interface/window/input/gamepad_button.h"
#include "../windmill/src/interface/window/input/keyboard_button.h"
#include "../windmill/src/interface/window/input/mouse_button.h"
#include "../windmill/src/interface/window/input/position_state.h"
#include "../windmill/src/interface/window/input/window_input_handler.h"
