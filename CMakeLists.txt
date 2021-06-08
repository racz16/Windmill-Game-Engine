cmake_minimum_required(VERSION 3.8)

project("Windmill-Game-Engine")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(BUILD_SHARED_LIBS ON)
option(WM_BUILD_EXAMPLES "Build the Windmill example programs" ON)

#GLFW
set(LIB_PATH_GLFW "${CMAKE_SOURCE_DIR}/lib/glfw")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("${LIB_PATH_GLFW}")

#GLM
set(LIB_PATH_GLM "${CMAKE_SOURCE_DIR}/lib/glm")
add_subdirectory("${LIB_PATH_GLM}")

#windmill
add_subdirectory("windmill")

#examples
if(WM_BUILD_EXAMPLES)
	add_subdirectory("examples/example_1")
endif()
