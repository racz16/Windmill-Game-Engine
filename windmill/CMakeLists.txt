set(WM_WINDMILL "windmill")
add_library("${WM_WINDMILL}" SHARED
	#
	# IMPLEMENTATION
	#
	# AUDIO
		"src/implementation/audio/wm_audio_system.cpp"
		"src/implementation/audio/wm_audio_system.h"
		"src/implementation/audio/wm_openal_audio_buffer.cpp"
		"src/implementation/audio/wm_openal_audio_buffer.h"
		"src/implementation/audio/wm_openal_audio_context.cpp"
		"src/implementation/audio/wm_openal_audio_context.h"
		"src/implementation/audio/wm_openal_audio_listener.cpp"
		"src/implementation/audio/wm_openal_audio_listener.h"
		"src/implementation/audio/wm_openal_audio_source.cpp"
		"src/implementation/audio/wm_openal_audio_source.h"
	# COMPONENT
		# audio
			"src/implementation/component/audio/wm_audio_listener_component.cpp"
			"src/implementation/component/audio/wm_audio_listener_component.h"
			"src/implementation/component/audio/wm_audio_source_component.cpp"
			"src/implementation/component/audio/wm_audio_source_component.h"
		# camera
			"src/implementation/component/camera/wm_camera_component.cpp"
			"src/implementation/component/camera/wm_camera_component.h"
	# DEBUG
		"src/implementation/debug/wm_scoped_time_logger.cpp"
		"src/implementation/debug/wm_scoped_time_logger.h"
		"src/implementation/debug/wm_stopwatch.h"
		"src/implementation/debug/wm_stopwatch.cpp"
	# EVENT
		"src/implementation/event/wm_event_system.cpp"
		"src/implementation/event/wm_event_system.h"
	# LOG
		"src/implementation/log/wm_base_log_writer.cpp"
		"src/implementation/log/wm_base_log_writer.h"
		"src/implementation/log/wm_console_log_writer.cpp"
		"src/implementation/log/wm_console_log_writer.h"
		"src/implementation/log/wm_file_log_writer.cpp"
		"src/implementation/log/wm_file_log_writer.h"
		"src/implementation/log/wm_log_system.cpp"
		"src/implementation/log/wm_log_system.h"
	# RENDERING
		"src/implementation/rendering/wm_gpu_matrices_ubo.h"
		"src/implementation/rendering/wm_gpu_vertex.h"
		# direct3d11
			# source code added only on Windows
		# opengl
			"src/implementation/rendering/opengl/wm_gl_buffer.cpp"
			"src/implementation/rendering/opengl/wm_gl_buffer.h"
			"src/implementation/rendering/opengl/wm_gl_defines.h"
			"src/implementation/rendering/opengl/wm_gl_mesh.cpp"
			"src/implementation/rendering/opengl/wm_gl_mesh.h"
			"src/implementation/rendering/opengl/wm_opengl_rendering_context.cpp"
			"src/implementation/rendering/opengl/wm_opengl_rendering_context.h"
			"src/implementation/rendering/opengl/wm_opengl_rendering_system.cpp"
			"src/implementation/rendering/opengl/wm_opengl_rendering_system.h"
		# vulkan
			"src/implementation/rendering/vulkan/wm_vk_buffer.cpp"
			"src/implementation/rendering/vulkan/wm_vk_buffer.h"
			"src/implementation/rendering/vulkan/wm_vk_defines.h"
			"src/implementation/rendering/vulkan/wm_vk_memory_type.cpp"
			"src/implementation/rendering/vulkan/wm_vk_memory_type.h"
			"src/implementation/rendering/vulkan/wm_vk_mesh.cpp"
			"src/implementation/rendering/vulkan/wm_vk_mesh.h"
			"src/implementation/rendering/vulkan/wm_vulkan_rendering_context.cpp"
			"src/implementation/rendering/vulkan/wm_vulkan_rendering_context.h"
			"src/implementation/rendering/vulkan/wm_vulkan_rendering_system.cpp"
			"src/implementation/rendering/vulkan/wm_vulkan_rendering_system.h"
	# RESOURCE
		"src/implementation/resource/wm_resource_system.cpp"
		"src/implementation/resource/wm_resource_system.h"
	# SCENE
		"src/implementation/scene/wm_node.cpp"
		"src/implementation/scene/wm_node.h"
		"src/implementation/scene/wm_simple_scene_system.cpp"
		"src/implementation/scene/wm_simple_scene_system.h"
		"src/implementation/scene/wm_transform.cpp"
		"src/implementation/scene/wm_transform.h"
	# TIME
		"src/implementation/time/wm_chrono_time_system.cpp"
		"src/implementation/time/wm_chrono_time_system.h"
	# WINDOW
		"src/implementation/window/wm_glfw_window_system.cpp"
		"src/implementation/window/wm_glfw_window_system.h"
		# input
			"src/implementation/window/input/wm_glfw_window_input_handler.cpp"
			"src/implementation/window/input/wm_glfw_window_input_handler.h"
	#
	# INTERFACE
	#
	# AUDIO
	"src/interface/audio/audio_buffer.cpp"
	"src/interface/audio/audio_buffer.h"
	"src/interface/audio/audio_context.cpp"
	"src/interface/audio/audio_context.h"
	"src/interface/audio/audio_distance_model.h"
	"src/interface/audio/audio_listener.cpp"
	"src/interface/audio/audio_listener.h"
	"src/interface/audio/audio_source.cpp"
	"src/interface/audio/audio_source.h"
	"src/interface/audio/audio_source_state.h"
	"src/interface/audio/audio_system.cpp"
	"src/interface/audio/audio_system.h"
	# COMPONENT
		"src/interface/component/component.cpp"
		"src/interface/component/component.h"
		# audio
			"src/interface/component/audio/audio_source_component.cpp"
			"src/interface/component/audio/audio_source_component.h"
			"src/interface/component/audio/audio_listener_component.cpp"
			"src/interface/component/audio/audio_listener_component.h"
		# camera
			"src/interface/component/camera/camera_component.cpp"
			"src/interface/component/camera/camera_component.h"
			"src/interface/component/camera/projection_mode.h"
		# event
			"src/interface/component/event/component_changed_event.cpp"
			"src/interface/component/event/component_changed_event.h"
			"src/interface/component/event/component_destroyed_event.cpp"
			"src/interface/component/event/component_destroyed_event.h"
			"src/interface/component/event/component_event.cpp"
			"src/interface/component/event/component_event.h"
	# CORE
		"src/interface/core/application.cpp"
		"src/interface/core/application.h"
		"src/interface/core/engine.h"
		"src/interface/core/engine.cpp"
		"src/interface/core/key.h"
		"src/interface/core/parameter_container.cpp"
		"src/interface/core/parameter_container.h"
		"src/interface/core/system.cpp"
		"src/interface/core/system.h"
		"src/interface/core/tag.cpp"
		"src/interface/core/tag.h"
		"src/interface/core/utility.cpp"
		"src/interface/core/utility.h"
		"src/interface/core/windmill_error.cpp"
		"src/interface/core/windmill_error.h"
	# DEBUG
		"src/interface/debug/scoped_time_logger.cpp"
		"src/interface/debug/scoped_time_logger.h"
		"src/interface/debug/stopwatch.cpp"
		"src/interface/debug/stopwatch.h"
	# DEFINES
		"src/interface/defines/code_generation_defines.h"
		"src/interface/defines/debug_defines.h"
		"src/interface/defines/general_defines.h"
		"src/interface/defines/log_defines.h"
	# EVENT
		"src/interface/event/event.cpp"
		"src/interface/event/event.h"
		"src/interface/event/event_listener.h"
		"src/interface/event/event_listener_base.cpp"
		"src/interface/event/event_listener_base.h"
		"src/interface/event/event_system.cpp"
		"src/interface/event/event_system.h"
	# LOG
		"src/interface/log/log_level.h"
		"src/interface/log/log_system.cpp"
		"src/interface/log/log_system.h"
		"src/interface/log/log_writer.cpp"
		"src/interface/log/log_writer.h"
	# PTR
		"src/interface/ptr/array_allocator_system.cpp"
		"src/interface/ptr/array_allocator_system.h"
		"src/interface/ptr/ptr.h"
		"src/interface/ptr/ptr_meta.cpp"
		"src/interface/ptr/ptr_meta.h"
		"src/interface/ptr/ptr_view.h"
		"src/interface/ptr/vector_array.h"
	# RENDERING
		"src/interface/rendering/gpu_buffer.cpp"
		"src/interface/rendering/gpu_buffer.h"
		"src/interface/rendering/gpu_mesh.cpp"
		"src/interface/rendering/gpu_mesh.h"
		"src/interface/rendering/gpu_resource.h"
		"src/interface/rendering/rendering_api.h"
		"src/interface/rendering/rendering_context.cpp"
		"src/interface/rendering/rendering_context.h"
		"src/interface/rendering/rendering_system.cpp"
		"src/interface/rendering/rendering_system.h"
		"src/interface/rendering/usage_frequency.h"
	# RESOURCE
		"src/interface/resource/image.cpp"
		"src/interface/resource/image.h"
		"src/interface/resource/mesh.cpp"
		"src/interface/resource/mesh.h"
		"src/interface/resource/resource_system.cpp"
		"src/interface/resource/resource_system.h"
		"src/interface/resource/sound.cpp"
		"src/interface/resource/sound.h"
		"src/interface/resource/vertex.cpp"
		"src/interface/resource/vertex.h"
	# SCENE
		"src/interface/scene/node.cpp"
		"src/interface/scene/node.h"
		"src/interface/scene/scene_system.cpp"
		"src/interface/scene/scene_system.h"
		"src/interface/scene/transform.cpp"
		"src/interface/scene/transform.h"
		# event
			# node
				"src/interface/scene/event/node/node_child_changed_event.cpp"
				"src/interface/scene/event/node/node_child_changed_event.h"
				"src/interface/scene/event/node/node_destroyed_event.cpp"
				"src/interface/scene/event/node/node_destroyed_event.h"
				"src/interface/scene/event/node/node_event.cpp"
				"src/interface/scene/event/node/node_event.h"
				"src/interface/scene/event/node/node_parent_changed_event.cpp"
				"src/interface/scene/event/node/node_parent_changed_event.h"
				"src/interface/scene/event/node/node_tag_changed_event.cpp"
				"src/interface/scene/event/node/node_tag_changed_event.h"
			# transform
				"src/interface/scene/event/transform/transform_changed_event.cpp"
				"src/interface/scene/event/transform/transform_changed_event.h"
				"src/interface/scene/event/transform/transform_changed_type.h"
				"src/interface/scene/event/transform/transform_destroyed_event.cpp"
				"src/interface/scene/event/transform/transform_destroyed_event.h"
				"src/interface/scene/event/transform/transform_event.cpp"
				"src/interface/scene/event/transform/transform_event.h"
	# TIME
		"src/interface/time/time_system.cpp"
		"src/interface/time/time_system.h"
	# WINDOW
		"src/interface/window/cursor_mode.h"
		"src/interface/window/cursor_shape.h"
		"src/interface/window/standard_cursor_shape.h"
		"src/interface/window/video_mode.cpp"
		"src/interface/window/video_mode.h"
		"src/interface/window/vsync_mode.h"
		"src/interface/window/window_system.cpp"
		"src/interface/window/window_system.h"
		# event
			"src/interface/window/event/gamepad_connection_event.cpp"
			"src/interface/window/event/gamepad_connection_event.h"
			"src/interface/window/event/keyboard_button_event.cpp"
			"src/interface/window/event/keyboard_button_event.h"
			"src/interface/window/event/keyboard_character_event.cpp"
			"src/interface/window/event/keyboard_character_event.h"
			"src/interface/window/event/mouse_button_event.cpp"
			"src/interface/window/event/mouse_button_event.h"
			"src/interface/window/event/mouse_hover_event.cpp"
			"src/interface/window/event/mouse_hover_event.h"
			"src/interface/window/event/mouse_position_event.cpp"
			"src/interface/window/event/mouse_position_event.h"
			"src/interface/window/event/mouse_scroll_event.h"
			"src/interface/window/event/mouse_scroll_event.cpp"
			"src/interface/window/event/window_closed_event.cpp"
			"src/interface/window/event/window_closed_event.h"
			"src/interface/window/event/window_content_scale_event.cpp"
			"src/interface/window/event/window_content_scale_event.h"
			"src/interface/window/event/window_drag_and_drop_event.cpp"
			"src/interface/window/event/window_drag_and_drop_event.h"
			"src/interface/window/event/window_focus_event.cpp"
			"src/interface/window/event/window_focus_event.h"
			"src/interface/window/event/window_framebuffer_size_event.cpp"
			"src/interface/window/event/window_framebuffer_size_event.h"
			"src/interface/window/event/window_maximization_event.cpp"
			"src/interface/window/event/window_maximization_event.h"
			"src/interface/window/event/window_minimization_event.cpp"
			"src/interface/window/event/window_minimization_event.h"
			"src/interface/window/event/window_position_event.cpp"
			"src/interface/window/event/window_position_event.h"
			"src/interface/window/event/window_refresh_required_event.h"
			"src/interface/window/event/window_refresh_required_event.cpp"
			"src/interface/window/event/window_size_event.cpp"
			"src/interface/window/event/window_size_event.h"
		# input
			"src/interface/window/input/axis_state.cpp"
			"src/interface/window/input/axis_state.h"
			"src/interface/window/input/button_action.h"
			"src/interface/window/input/button_state.cpp"
			"src/interface/window/input/button_state.h"
			"src/interface/window/input/gamepad_axis.h"
			"src/interface/window/input/gamepad_button.h"
			"src/interface/window/input/keyboard_button.h"
			"src/interface/window/input/mouse_button.h"
			"src/interface/window/input/position_state.cpp"
			"src/interface/window/input/position_state.h"
			"src/interface/window/input/window_input_handler.h"
	#
	# IMGUI
	#
	"../lib/imgui/imgui.cpp"
	"../lib/imgui/imgui_demo.cpp"
	"../lib/imgui/imgui_draw.cpp"
	"../lib/imgui/imgui_tables.cpp"
	"../lib/imgui/imgui_widgets.cpp"
	# backends
		"../lib/imgui/backends/imgui_impl_glfw.cpp"
		"../lib/imgui/backends/imgui_impl_opengl3.cpp"
		"../lib/imgui/backends/imgui_impl_vulkan.cpp"
)
target_include_directories("${WM_WINDMILL}" PRIVATE "${WM_SOURCE_DIR}/windmill/src/interface")
target_compile_definitions("${WM_WINDMILL}" PRIVATE "WM_EXPORT")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
	target_compile_definitions("${WM_WINDMILL}" PUBLIC "WM_BUILD_RELEASE")
else()
	target_compile_definitions("${WM_WINDMILL}" PUBLIC "WM_BUILD_DEBUG")
endif()

#ASSIMP
target_link_libraries("${WM_WINDMILL}" PRIVATE assimp)
target_include_directories("${WM_WINDMILL}" PRIVATE "${WM_LIB_PATH_ASSIMP}/include")
target_include_directories("${WM_WINDMILL}" PRIVATE "${CMAKE_BINARY_DIR}/lib/assimp/include")

#GLFW
target_compile_definitions("${WM_WINDMILL}" PUBLIC "GLFW_INCLUDE_NONE")
target_link_libraries("${WM_WINDMILL}" PRIVATE glfw)
target_include_directories("${WM_WINDMILL}" PRIVATE "${WM_LIB_PATH_GLFW}/include")

#GLM
target_compile_definitions("${WM_WINDMILL}" PUBLIC "GLM_FORCE_RADIANS")
target_compile_definitions("${WM_WINDMILL}" PUBLIC "GLM_FORCE_DEPTH_ZERO_TO_ONE")
target_compile_definitions("${WM_WINDMILL}" PUBLIC "GLM_FORCE_MESSAGES")
target_compile_definitions("${WM_WINDMILL}" PUBLIC "GLM_FORCE_DEFAULT_ALIGNED_GENTYPES")
target_compile_definitions("${WM_WINDMILL}" PUBLIC "GLM_FORCE_INTRINSICS")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
	target_compile_definitions("${WM_WINDMILL}" PUBLIC "GLM_FORCE_INLINE")
endif()
target_include_directories("${WM_WINDMILL}" PUBLIC "${WM_LIB_PATH_GLM}")

#STB
target_include_directories("${WM_WINDMILL}" PUBLIC "${WM_LIB_PATH_STB}")

#IMGUI
target_compile_definitions("${WM_WINDMILL}" PUBLIC "IM_VEC2_CLASS_EXTRA=constexpr ImVec2(const glm::vec2& f) : x(f.x), y(f.y) {} operator glm::vec2() const { return glm::vec2(x,y)\; }")
target_compile_definitions("${WM_WINDMILL}" PUBLIC "IM_VEC4_CLASS_EXTRA=constexpr ImVec4(const glm::vec4& f) : x(f.x), y(f.y), z(f.z), w(f.w) {} operator glm::vec4() const { return glm::vec4(x,y,z,w)\; }")
target_include_directories("${WM_WINDMILL}" PUBLIC "${WM_LIB_PATH_IMGUI}")

#VULKAN
find_package(Vulkan REQUIRED)
if(Vulkan_FOUND)
	target_link_libraries("${WM_WINDMILL}" PRIVATE Vulkan::Vulkan)
	target_include_directories("${WM_WINDMILL}" PRIVATE "${Vulkan_INCLUDE_DIRS}")
	
	function(WM_COMPILE_GLSL_SHADERS WM_GLSL_COMPILER)
		cmake_parse_arguments(PARSE_ARGV 1 WM_ARG "" "BINARY_GENERATION_FLAG;DEBUG_INFO_FLAG;OPTIMIZATION_FLAG;OUTPUT_FLAG" "")
		set(WM_GLSL_FILES "lambertian/vertex.vert" "lambertian/fragment.frag")
		foreach(WM_GLSL_FILE IN LISTS WM_GLSL_FILES)
			set(WM_GLSL_FILE_PATH "${WM_SOURCE_DIR}/windmill/res/shader/glsl/${WM_GLSL_FILE}")
			get_filename_component(WM_GLSL_FILE_DIRECTORY "${WM_GLSL_FILE_PATH}" DIRECTORY)
			get_filename_component(WM_GLSL_FILE_NAME "${WM_GLSL_FILE_PATH}" NAME)
			set(WM_SPV_FILE_PATH "${WM_GLSL_FILE_DIRECTORY}/$<IF:$<CONFIG:DEBUG>,_debug_,_release_>${WM_GLSL_FILE_NAME}.spv")
			add_custom_command(
				MAIN_DEPENDENCY "${WM_GLSL_FILE_PATH}"
				OUTPUT "${WM_SPV_FILE_PATH}"
				COMMAND "${WM_GLSL_COMPILER}"
					"${WM_ARG_BINARY_GENERATION_FLAG}"
					"${WM_ARG_DEBUG_INFO_FLAG}"
					"${WM_ARG_OPTIMIZATION_FLAG}"
					"${WM_ARG_OUTPUT_FLAG}"
					"${WM_SPV_FILE_PATH}"
					"${WM_GLSL_FILE_PATH}"
			)
			list(APPEND WM_SPV_FILE_PATHS "${WM_SPV_FILE_PATH}")
		endforeach()
		add_custom_target(WM_GLSL_COMPILATION DEPENDS "${WM_SPV_FILE_PATHS}")
		add_dependencies("${WM_WINDMILL}" WM_GLSL_COMPILATION)
	endfunction()

	find_program(GLSLC glslc)
	if(GLSLC)
		message(STATUS "glslc found")
		wm_compile_glsl_shaders("${GLSLC}"
			DEBUG_INFO_FLAG "$<$<CONFIG:DEBUG>:-g>"
			OPTIMIZATION_FLAG "$<IF:$<CONFIG:DEBUG>,-O0,-O>"
			OUTPUT_FLAG "-o"
		)
	else()
		find_program(GLSLANG_VALIDATOR glslangValidator)
		if(GLSLANG_VALIDATOR)
			message(STATUS "glslangValidator found")
			wm_compile_glsl_shaders("${GLSLANG_VALIDATOR}"
				BINARY_GENERATION_FLAG "-V"
				DEBUG_INFO_FLAG "$<$<CONFIG:DEBUG>:-g>"
				OPTIMIZATION_FLAG "$<$<CONFIG:DEBUG>:-Od>"
				OUTPUT_FLAG "-o"
			)
		else()
			message(FATAL_ERROR "Couldn't find glslc or glslangValidator")
		endif()
	endif()
	
else()
	message(FATAL_ERROR "Couldn't find Vulkan")
endif()

#DIRECT3D11
if(WIN32)
	target_sources("${WM_WINDMILL}" PRIVATE
		"src/implementation/rendering/direct3d11/wm_direct3d11_rendering_context.cpp"
		"src/implementation/rendering/direct3d11/wm_direct3d11_rendering_context.h"
		"src/implementation/rendering/direct3d11/wm_direct3d11_rendering_system.cpp"
		"src/implementation/rendering/direct3d11/wm_direct3d11_rendering_system.h"
		"src/implementation/rendering/direct3d11/wm_dx11_buffer.cpp"
		"src/implementation/rendering/direct3d11/wm_dx11_buffer.h"
		"src/implementation/rendering/direct3d11/wm_dx11_defines.h"
		"src/implementation/rendering/direct3d11/wm_dx11_mesh.cpp"
		"src/implementation/rendering/direct3d11/wm_dx11_mesh.h"
		# imgui
		"../lib/imgui/backends/imgui_impl_dx11.cpp"
	)
	target_link_libraries("${WM_WINDMILL}" PRIVATE d3d11 dxgi d3dcompiler)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		target_link_libraries("${WM_WINDMILL}" PRIVATE dxguid)
	endif()
	if(NOT WM_CI)
		find_program(FXC fxc REQUIRED)
		message(STATUS "fxc found")
		set(WM_HLSL_FILES "lambertian/vertex.hlsl" "lambertian/pixel.hlsl")
		foreach(WM_HLSL_FILE IN LISTS WM_HLSL_FILES)
			set(WM_HLSL_FILE_PATH "${WM_SOURCE_DIR}/windmill/res/shader/hlsl/${WM_HLSL_FILE}")
			get_filename_component(WM_HLSL_FILE_DIRECTORY "${WM_HLSL_FILE_PATH}" DIRECTORY)
			get_filename_component(WM_HLSL_FILE_NAME "${WM_HLSL_FILE_PATH}" NAME)
			set(WM_CSO_FILE_PATH "${WM_HLSL_FILE_DIRECTORY}/$<IF:$<CONFIG:DEBUG>,_debug_,_release_>${WM_HLSL_FILE_NAME}.cso")
			if("${WM_HLSL_FILE}" MATCHES ".*vertex.*")
				set(WM_HLSL_SHADER_MODEL "vs_5_0")
			else()
				set(WM_HLSL_SHADER_MODEL "ps_5_0")
			endif()
			add_custom_command(
				MAIN_DEPENDENCY "${WM_HLSL_FILE_PATH}"
				OUTPUT "${WM_CSO_FILE_PATH}"
				COMMAND "${FXC}" "/T" "${WM_HLSL_SHADER_MODEL}" "$<$<CONFIG:DEBUG>:/Zi>" "$<IF:$<CONFIG:DEBUG>,/Od,/O1>" "/Fo" "${WM_CSO_FILE_PATH}" "${WM_HLSL_FILE_PATH}"
			)
			list(APPEND WM_CSO_FILE_PATHS "${WM_CSO_FILE_PATH}")
		endforeach()
		add_custom_target(WM_HLSL_COMPILATION DEPENDS "${WM_CSO_FILE_PATHS}")
		add_dependencies("${WM_WINDMILL}" WM_HLSL_COMPILATION)
	endif()
endif()

#OPENAL
target_link_libraries("${WM_WINDMILL}" PRIVATE OpenAL)
target_include_directories("${WM_WINDMILL}" PRIVATE "${WM_LIB_PATH_OPENAL}/include")

#LIBSNDFILE
target_link_libraries("${WM_WINDMILL}" PRIVATE sndfile)
target_include_directories("${WM_WINDMILL}" PRIVATE "${WM_LIB_PATH_LIBSNDFILE}/include")

#GLAD
target_link_libraries("${WM_WINDMILL}" PRIVATE glad)
target_include_directories("${WM_WINDMILL}" PRIVATE "${WM_LIB_PATH_GLAD}/include")

#precompile headers
set(PRECOMPILED_HEADERS
	"<string>"
	"<vector>"
	"<array>"
	"<unordered_map>"
	"<unordered_set>"
	"<iostream>"
	"<memory>"
	"<chrono>"
	"<any>"
	"<utility>"
	"<fstream>"
	"<functional>"
	"<glm/glm.hpp>"
	"<glm/ext.hpp>"
	"<vulkan/vulkan.h>"
	"<GLFW/glfw3.h>"
	"<stb_image.h>"
	"<assimp/Importer.hpp>"
	"<assimp/scene.h>"
	"<assimp/postprocess.h>"
	"<AL/al.h>"
	"<AL/alc.h>"
	"<sndfile.h>"
	"<glad/glad.h>"
)
if(WIN32)
	target_compile_definitions("${WM_WINDMILL}" PRIVATE "NOMINMAX" "GLFW_EXPOSE_NATIVE_WIN32")
	list(APPEND PRECOMPILED_HEADERS "<Windows.h>" "<dxgi.h>" "<d3d11_1.h>" "<d3dcompiler.h>" "<GLFW/glfw3native.h>")
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		list(APPEND PRECOMPILED_HEADERS "<dxgidebug.h>")
	endif()
endif()
target_precompile_headers("${WM_WINDMILL}" PRIVATE "${PRECOMPILED_HEADERS}")
#resources
add_custom_command(
	OUTPUT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res"
	COMMAND ${CMAKE_COMMAND} -E create_symlink "${WM_SOURCE_DIR}/windmill/res" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res"
	VERBATIM
)
add_custom_target(WM_RES_SYMLINK DEPENDS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res")
add_dependencies("${WM_WINDMILL}" WM_RES_SYMLINK)
