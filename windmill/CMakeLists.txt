set(WM_WINDMILL "windmill")
add_library("${WM_WINDMILL}" SHARED "src/interface/core/defines.h" "src/interface/core/engine.h"   "src/interface/debug/log/log_system.h" "src/interface/debug/log/log_system.cpp" "src/interface/debug/log/log_writer.h" "src/debug/log/wm_console_log_writer.h" "src/debug/log/wm_console_log_writer.cpp" "src/debug/log/wm_log_system.h" "src/debug/log/wm_log_system.cpp" "src/debug/log/wm_base_log_writer.h" "src/debug/log/wm_base_log_writer.cpp" "src/interface/debug/log/log_level.h"    "src/interface/debug/log/log_writer.cpp"    "src/debug/log/wm_file_log_writer.h" "src/debug/log/wm_file_log_writer.cpp"  "src/interface/debug/stopwatch.h" "src/interface/debug/stopwatch.cpp" "src/debug/wm_stopwatch.h" "src/debug/wm_stopwatch.cpp" "src/interface/debug/log/scoped_time_logger.h" "src/interface/debug/log/scoped_time_logger.cpp" "src/debug/log/wm_scoped_time_logger.h" "src/debug/log/wm_scoped_time_logger.cpp" "src/window/wm_glfw_window_system.h" "src/window/wm_glfw_window_system.cpp" "src/interface/window/window_system.h" "src/interface/window/window_system.cpp"    "src/interface/core/system.h" "src/interface/core/key.h" "src/interface/core/engine.cpp" "src/core/wm_base_system.h" "src/core/wm_base_system.cpp"   "src/interface/core/application.h" "src/interface/core/application.cpp"    "src/interface/core/utility.h" "src/interface/core/utility.cpp"          "src/interface/window/window.h" "src/window/wm_glfw_window.h" "src/window/wm_glfw_window.cpp" "src/interface/window/video_mode.h" "src/interface/window/video_mode.cpp" "src/interface/core/ptr.h" "src/interface/core/ptr_meta.h" "src/interface/core/ptr_meta.cpp" "src/interface/core/time_system.h" "src/interface/core/time_system.cpp" "src/core/wm_chrono_time_system.h" "src/core/wm_chrono_time_system.cpp")
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
target_precompile_headers("${WM_WINDMILL}" PRIVATE "<iostream>" "<vector>" "<string>" "<memory>")

#install
install(TARGETS glfw)
install(TARGETS "${WM_WINDMILL}")
install(FILES "${WM_SOURCE_DIR}/include/windmill/windmill.h" DESTINATION "include/windmill")
