set(WINDMILL "windmill")
add_library("${WINDMILL}" SHARED "src/wm_engine.h" "src/wm_engine.cpp" "src/wm_utility.h" "src/interface/engine.h" "src/interface/engine.cpp")
target_compile_definitions("${WINDMILL}" PRIVATE "EXPORT_WINDMILL")

#GLFW
target_link_libraries("${WINDMILL}" PUBLIC glfw)
target_include_directories("${WINDMILL}" PUBLIC "${LIB_PATH_GLFW}/include")

#GLM
target_compile_definitions("${WINDMILL}" PUBLIC "GLM_FORCE_DEPTH_ZERO_TO_ONE")
target_compile_definitions("${WINDMILL}" PUBLIC "GLM_FORCE_MESSAGES")
target_compile_definitions("${WINDMILL}" PUBLIC "GLM_FORCE_DEFAULT_ALIGNED_GENTYPES")
target_compile_definitions("${WINDMILL}" PUBLIC "GLM_FORCE_INTRINSICS")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
	target_compile_definitions("${WINDMILL}" PUBLIC "GLM_FORCE_INLINE")
endif()
target_include_directories("${WINDMILL}" PUBLIC "${LIB_PATH_GLM}")

#precompile headers
target_precompile_headers("${WINDMILL}" PRIVATE "<iostream>" "<vector>" "<string>" "<memory>")

#install
install(TARGETS "${WINDMILL}")
install(FILES "${CMAKE_SOURCE_DIR}/include/windmill/windmill.h" DESTINATION "include/windmill")
