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
#include "../windmill/src/interface/core/base_ptr.h"
#include "../windmill/src/interface/core/defines.h"
#include "../windmill/src/interface/core/engine.h"
#include "../windmill/src/interface/core/key.h"
#include "../windmill/src/interface/core/parameter_container.h"
#include "../windmill/src/interface/core/ptr.h"
#include "../windmill/src/interface/core/ptr_meta.h"
#include "../windmill/src/interface/core/ptr_view.h"
#include "../windmill/src/interface/core/system.h"
#include "../windmill/src/interface/core/time_system.h"
#include "../windmill/src/interface/core/utility.h"
//
//DEBUG
//
#include "../windmill/src/interface/debug/stopwatch.h"
//log
#include "../windmill/src/interface/debug/log/log_level.h"
#include "../windmill/src/interface/debug/log/log_system.h"
#include "../windmill/src/interface/debug/log/log_writer.h"
#include "../windmill/src/interface/debug/log/scoped_time_logger.h"
//
//EVENT
//
#include "../windmill/src/interface/event/event_listener.h"
#include "../windmill/src/interface/event/event_system.h"
#include "../windmill/src/interface/event/event.h"
//
//WINDOW
//
#include "../windmill/src/interface/window/cursor_mode.h"
#include "../windmill/src/interface/window/cursor_shape.h"
#include "../windmill/src/interface/window/video_mode.h"
#include "../windmill/src/interface/window/window_system.h"
#include "../windmill/src/interface/window/window.h"
//event
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
#include "../windmill/src/interface/window/input/keyboard_button_action.h"
#include "../windmill/src/interface/window/input/keyboard_button.h"
#include "../windmill/src/interface/window/input/mouse_button.h"
