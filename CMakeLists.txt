cmake_minimum_required(VERSION 3.7)

project("Windmill-Game-Engine")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
	string(REPLACE "/EHsc" "/EHa" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	string(REPLACE "/GR" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	add_compile_options("/fp:fast")
elseif(GNU)
	add_compile_options("-ffast-math")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(WM_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(BUILD_SHARED_LIBS ON)
option(WM_BUILD_EXAMPLES "Build the Windmill example programs" ON)

#GLFW
set(WM_LIB_PATH_GLFW "${WM_SOURCE_DIR}/lib/glfw")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory("${WM_LIB_PATH_GLFW}")

#GLM
set(WM_LIB_PATH_GLM "${WM_SOURCE_DIR}/lib/glm")
add_subdirectory("${WM_LIB_PATH_GLM}")

#windmill
add_subdirectory("windmill")

#examples
if(WM_BUILD_EXAMPLES)
	add_subdirectory("examples/example_1")
endif()
