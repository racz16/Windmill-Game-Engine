set(WM_WINDMILL "windmill")
add_library("${WM_WINDMILL}" SHARED "src/interface/core/defines.h" "src/interface/core/engine.h"   "src/interface/log/log_system.h" "src/interface/log/log_system.cpp" "src/interface/log/log_writer.h" "src/implementation/log/wm_console_log_writer.h" "src/implementation/log/wm_console_log_writer.cpp" "src/implementation/log/wm_log_system.h" "src/implementation/log/wm_log_system.cpp" "src/implementation/log/wm_base_log_writer.h" "src/implementation/log/wm_base_log_writer.cpp" "src/interface/log/log_level.h"    "src/interface/log/log_writer.cpp"    "src/implementation/log/wm_file_log_writer.h" "src/implementation/log/wm_file_log_writer.cpp"  "src/interface/debug/stopwatch.h" "src/interface/debug/stopwatch.cpp" "src/implementation/debug/wm_stopwatch.h" "src/implementation/debug/wm_stopwatch.cpp" "src/interface/debug/scoped_time_logger.h" "src/interface/debug/scoped_time_logger.cpp" "src/implementation/debug/wm_scoped_time_logger.h" "src/implementation/debug/wm_scoped_time_logger.cpp" "src/implementation/window/wm_glfw_window_system.h" "src/implementation/window/wm_glfw_window_system.cpp" "src/interface/window/window_system.h" "src/interface/window/window_system.cpp"    "src/interface/core/system.h" "src/interface/core/key.h" "src/interface/core/engine.cpp" "src/implementation/core/wm_base_system.h" "src/implementation/core/wm_base_system.cpp"   "src/interface/core/application.h" "src/interface/core/application.cpp"    "src/interface/core/utility.h" "src/interface/core/utility.cpp"          "src/interface/window/window.h" "src/implementation/window/wm_glfw_window.h" "src/implementation/window/wm_glfw_window.cpp" "src/interface/window/video_mode.h" "src/interface/window/video_mode.cpp" "src/interface/ptr/ptr.h" "src/interface/ptr/ptr_meta.h" "src/interface/ptr/ptr_meta.cpp" "src/interface/time/time_system.h" "src/interface/time/time_system.cpp" "src/implementation/time/wm_chrono_time_system.h" "src/implementation/time/wm_chrono_time_system.cpp" "src/interface/core/parameter_container.h" "src/interface/ptr/ptr_view.h" "src/interface/ptr/base_ptr.h" "src/interface/event/event.h" "src/interface/event/event_system.h" "src/interface/event/event_listener.h" "src/interface/event/event_system.cpp" "src/implementation/event/wm_event_system.h" "src/implementation/event/wm_event_system.cpp" "src/interface/window/event/window_position_event.h" "src/interface/window/event/mouse_position_event.h" "src/interface/event/event.cpp"   "src/interface/window/event/window_event.h" "src/interface/window/event/window_closed_event.h" "src/interface/window/event/window_closed_event.cpp" "src/interface/window/event/mouse_position_event.cpp" "src/interface/window/event/window_position_event.cpp" "src/interface/window/event/window_size_event.h" "src/interface/window/event/window_size_event.cpp" "src/interface/window/event/window_event.cpp" "src/interface/window/event/window_framebuffer_size_event.cpp" "src/interface/window/event/window_framebuffer_size_event.h" "src/interface/window/event/window_content_scale_event.h" "src/interface/window/event/window_content_scale_event.cpp" "src/interface/window/event/window_minimization_event.h" "src/interface/window/event/window_minimization_event.cpp" "src/interface/window/event/window_maximization_event.cpp" "src/interface/window/event/window_maximization_event.h" "src/interface/window/event/window_focus_event.cpp" "src/interface/window/event/window_focus_event.h" "src/interface/window/event/window_refresh_required_event.cpp" "src/interface/window/event/window_refresh_required_event.h" "src/interface/window/input/keyboard_button.h" "src/interface/window/input/keyboard_button_action.h" "src/interface/window/event/keyboard_button_event.h" "src/interface/window/event/keyboard_button_event.cpp" "src/interface/window/event/keyboard_character_event.h" "src/interface/window/event/keyboard_character_event.cpp" "src/interface/window/event/mouse_button_event.h" "src/interface/window/event/mouse_button_event.cpp" "src/interface/window/input/mouse_button.h"  "src/interface/window/event/mouse_scroll_event.h" "src/interface/window/event/mouse_scroll_event.cpp" "src/interface/window/event/window_drag_and_drop_event.h" "src/interface/window/event/window_drag_and_drop_event.cpp" "src/interface/window/event/mouse_enter_leave_event.h" "src/interface/window/event/mouse_enter_leave_event.cpp" "src/interface/window/cursor_mode.h" "src/interface/window/cursor_shape.h")
target_include_directories("${WM_WINDMILL}" PRIVATE "${WM_SOURCE_DIR}/windmill/src/interface")
target_compile_definitions("${WM_WINDMILL}" PRIVATE "WM_EXPORT")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
	target_compile_definitions("${WM_WINDMILL}" PUBLIC "WM_BUILD_RELEASE")
else()
	target_compile_definitions("${WM_WINDMILL}" PUBLIC "WM_BUILD_DEBUG")
endif()

#GLFW
target_link_libraries("${WM_WINDMILL}" PRIVATE glfw)
target_include_directories("${WM_WINDMILL}" PRIVATE "${WM_LIB_PATH_GLFW}/include")

#GLM
target_compile_definitions("${WM_WINDMILL}" PUBLIC "GLM_FORCE_DEPTH_ZERO_TO_ONE")
target_compile_definitions("${WM_WINDMILL}" PUBLIC "GLM_FORCE_MESSAGES")
target_compile_definitions("${WM_WINDMILL}" PUBLIC "GLM_FORCE_DEFAULT_ALIGNED_GENTYPES")
target_compile_definitions("${WM_WINDMILL}" PUBLIC "GLM_FORCE_INTRINSICS")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
	target_compile_definitions("${WM_WINDMILL}" PUBLIC "GLM_FORCE_INLINE")
endif()
target_include_directories("${WM_WINDMILL}" PUBLIC "${WM_LIB_PATH_GLM}")

#precompile headers
target_precompile_headers("${WM_WINDMILL}" PRIVATE "<string>" "<vector>" "<unordered_map>" "<iostream>" "<memory>" "<chrono>" "<any>" "<utility>" "<glm/glm.hpp>" "<glm/ext.hpp>" "<GLFW/glfw3.h>")

#install
install(TARGETS glfw)
install(TARGETS "${WM_WINDMILL}")
install(FILES "${WM_SOURCE_DIR}/include/windmill/windmill.h" DESTINATION "include/windmill")
