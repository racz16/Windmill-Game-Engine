set(WM_WINDMILL "windmill")
add_library("${WM_WINDMILL}" SHARED "src/interface/defines.h" "src/interface/engine.h" "src/interface/engine.cpp"  "src/interface/debug/log/log_system.h" "src/interface/debug/log/log_system.cpp" "src/interface/debug/log/log_writer.h" "src/debug/log/wm_console_log_writer.h" "src/debug/log/wm_console_log_writer.cpp" "src/debug/log/wm_log_system.h" "src/debug/log/wm_log_system.cpp" "src/debug/log/wm_base_log_writer.h" "src/debug/log/wm_base_log_writer.cpp" "src/interface/debug/log/log_level.h"    "src/interface/debug/log/log_writer.cpp" "src/wm_engine.h" "src/wm_engine.cpp" "src/interface/debug/to_string.h" "src/debug/log/wm_file_log_writer.h" "src/debug/log/wm_file_log_writer.cpp"  "src/interface/debug/stopwatch.h" "src/interface/debug/stopwatch.cpp" "src/debug/wm_stopwatch.h" "src/debug/wm_stopwatch.cpp" "src/interface/debug/log/scoped_time_logger.h" "src/interface/debug/log/scoped_time_logger.cpp" "src/debug/log/wm_scoped_time_logger.h" "src/debug/log/wm_scoped_time_logger.cpp" "src/window/wm_glfw_window_system.h" "src/window/wm_glfw_window_system.cpp" "src/interface/window/window_system.h" "src/interface/window/window_system.cpp" "src/interface/window/window_system_init.h"  )
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
