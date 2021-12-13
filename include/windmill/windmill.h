#pragma once

#include <vector>
#include <chrono>
#include <any>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

//
//CORE
//
#include "../windmill/src/interface/core/application.h"
#include "../windmill/src/interface/core/engine.h"
#include "../windmill/src/interface/core/key.h"
#include "../windmill/src/interface/core/parameter_container.h"
#include "../windmill/src/interface/core/system.h"
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
#include "../windmill/src/interface/defines/debug_defines.h"
#include "../windmill/src/interface/defines/general_defines.h"
#include "../windmill/src/interface/defines/log_defines.h"
//
//EVENT
//
#include "../windmill/src/interface/event/event_listener.h"
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
#include "../windmill/src/interface/ptr/base_ptr.h"
#include "../windmill/src/interface/ptr/ptr.h"
#include "../windmill/src/interface/ptr/ptr_meta.h"
#include "../windmill/src/interface/ptr/ptr_view.h"
//
//RENDERING
//
#include "../windmill/src/interface/rendering/rendering_system.h"
//
//TIME
//
#include "../windmill/src/interface/time/time_system.h"
//
//WINDOW
//
#include "../windmill/src/interface/window/cursor_mode.h"
#include "../windmill/src/interface/window/cursor_shape.h"
#include "../windmill/src/interface/window/video_mode.h"
#include "../windmill/src/interface/window/window_system.h"
#include "../windmill/src/interface/window/window.h"
//event
#include "../windmill/src/interface/window/event/gamepad_connection_event.h"
#include "../windmill/src/interface/window/event/keyboard_button_event.h"
#include "../windmill/src/interface/window/event/keyboard_character_event.h"
#include "../windmill/src/interface/window/event/mouse_button_event.h"
#include "../windmill/src/interface/window/event/mouse_enter_leave_event.h"
#include "../windmill/src/interface/window/event/mouse_position_event.h"
#include "../windmill/src/interface/window/event/mouse_scroll_event.h"
#include "../windmill/src/interface/window/event/window_closed_event.h"
#include "../windmill/src/interface/window/event/window_content_scale_event.h"
#include "../windmill/src/interface/window/event/window_drag_and_drop_event.h"
#include "../windmill/src/interface/window/event/window_event.h"
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
