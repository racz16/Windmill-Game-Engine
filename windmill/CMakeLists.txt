set(WM_WINDMILL "windmill")
add_library("${WM_WINDMILL}" SHARED
	"src/interface/core/engine.h"
	"src/interface/log/log_system.h"
	"src/interface/log/log_system.cpp"
	"src/interface/log/log_writer.h"
	"src/implementation/log/wm_console_log_writer.h"
	"src/implementation/log/wm_console_log_writer.cpp"
	"src/implementation/log/wm_log_system.h"
	"src/implementation/log/wm_log_system.cpp"
	"src/implementation/log/wm_base_log_writer.h"
	"src/implementation/log/wm_base_log_writer.cpp"
	"src/interface/log/log_level.h"
	"src/interface/log/log_writer.cpp"
	"src/implementation/log/wm_file_log_writer.h"
	"src/implementation/log/wm_file_log_writer.cpp"
	"src/interface/debug/stopwatch.h"
	"src/interface/debug/stopwatch.cpp"
	"src/implementation/debug/wm_stopwatch.h"
	"src/implementation/debug/wm_stopwatch.cpp"
	"src/interface/debug/scoped_time_logger.h"
	"src/interface/debug/scoped_time_logger.cpp"
	"src/implementation/debug/wm_scoped_time_logger.h"
	"src/implementation/debug/wm_scoped_time_logger.cpp"
	"src/implementation/window/wm_glfw_window_system.h"
	"src/implementation/window/wm_glfw_window_system.cpp"
	"src/interface/window/window_system.h"
	"src/interface/window/window_system.cpp"
	"src/interface/core/system.h"
	"src/interface/core/key.h"
	"src/interface/core/engine.cpp"
	"src/interface/core/application.h"
	"src/interface/core/application.cpp"
	"src/interface/core/utility.h"
	"src/interface/core/utility.cpp"
	"src/interface/window/video_mode.h"
	"src/interface/window/video_mode.cpp"
	"src/interface/ptr/ptr.h"
	"src/interface/ptr/ptr_meta.h"
	"src/interface/ptr/ptr_meta.cpp"
	"src/interface/time/time_system.h"
	"src/interface/time/time_system.cpp"
	"src/implementation/time/wm_chrono_time_system.h"
	"src/implementation/time/wm_chrono_time_system.cpp"
	"src/interface/core/parameter_container.h"
	"src/interface/event/event.h"
	"src/interface/event/event_system.h"
	"src/interface/event/event_listener.h"
	"src/interface/event/event_system.cpp"
	"src/implementation/event/wm_event_system.h"
	"src/implementation/event/wm_event_system.cpp"
	"src/interface/window/event/window_position_event.h"
	"src/interface/window/event/mouse_position_event.h"
	"src/interface/event/event.cpp"
	"src/interface/window/event/window_closed_event.h"
	"src/interface/window/event/window_closed_event.cpp"
	"src/interface/window/event/mouse_position_event.cpp"
	"src/interface/window/event/window_position_event.cpp"
	"src/interface/window/event/window_size_event.h"
	"src/interface/window/event/window_size_event.cpp"
	"src/interface/window/event/window_framebuffer_size_event.cpp"
	"src/interface/window/event/window_framebuffer_size_event.h"
	"src/interface/window/event/window_content_scale_event.h"
	"src/interface/window/event/window_content_scale_event.cpp"
	"src/interface/window/event/window_minimization_event.h"
	"src/interface/window/event/window_minimization_event.cpp"
	"src/interface/window/event/window_maximization_event.cpp"
	"src/interface/window/event/window_maximization_event.h"
	"src/interface/window/event/window_focus_event.cpp"
	"src/interface/window/event/window_focus_event.h"
	"src/interface/window/event/window_refresh_required_event.cpp"
	"src/interface/window/event/window_refresh_required_event.h"
	"src/interface/window/input/keyboard_button.h"
	"src/interface/window/input/button_action.h"
	"src/interface/window/event/keyboard_button_event.h"
	"src/interface/window/event/keyboard_button_event.cpp"
	"src/interface/window/event/keyboard_character_event.h"
	"src/interface/window/event/keyboard_character_event.cpp"
	"src/interface/window/event/mouse_button_event.h"
	"src/interface/window/event/mouse_button_event.cpp"
	"src/interface/window/input/mouse_button.h"
	"src/interface/window/event/mouse_scroll_event.h"
	"src/interface/window/event/mouse_scroll_event.cpp"
	"src/interface/window/event/window_drag_and_drop_event.h"
	"src/interface/window/event/window_drag_and_drop_event.cpp"
	"src/interface/window/event/mouse_hover_event.h"
	"src/interface/window/event/mouse_hover_event.cpp"
	"src/interface/window/cursor_mode.h"
	"src/interface/window/standard_cursor_shape.h"
	"src/interface/window/input/window_input_handler.h"
	"src/implementation/window/input/wm_glfw_window_input_handler.h"
	"src/implementation/window/input/wm_glfw_window_input_handler.cpp"
	"src/interface/window/input/button_state.h"
	"src/interface/window/input/button_state.cpp"
	"src/interface/window/input/position_state.h"
	"src/interface/window/input/position_state.cpp"
	"src/interface/window/input/gamepad_button.h"
	"src/interface/window/input/gamepad_axis.h"
	"src/interface/window/input/axis_state.h"
	"src/interface/window/input/axis_state.cpp"
	"src/interface/window/event/gamepad_connection_event.h"
	"src/interface/window/event/gamepad_connection_event.cpp"
	"src/interface/rendering/rendering_system.h"
	"src/interface/rendering/rendering_system.cpp"
	"src/implementation/rendering/vulkan/wm_vulkan_rendering_system.h"
	"src/implementation/rendering/vulkan/wm_vulkan_rendering_system.cpp"
	"src/interface/core/windmill_error.h"
	"src/interface/core/windmill_error.cpp"
	"src/interface/defines/log_defines.h"
	"src/interface/defines/debug_defines.h"
	"src/implementation/resource/wm_resource_system.h"
	"src/implementation/resource/wm_resource_system.cpp"
	"src/interface/resource/image.h"
	"src/interface/resource/resource_system.h"
	"src/interface/resource/resource_system.cpp"
	"src/interface/resource/image.cpp"
	"src/interface/resource/mesh.h"
	"src/interface/resource/vertex.h"
	"src/interface/resource/vertex.cpp"
	"src/interface/resource/mesh.cpp"
	"src/interface/window/cursor_shape.h"
	"src/interface/ptr/vector_array.h"
	"src/interface/ptr/array_allocator_system.h"
	"src/interface/ptr/array_allocator_system.cpp"
	"src/interface/scene/node.h"
	"src/interface/scene/node.cpp"
	"src/interface/scene/scene_system.h"
	"src/interface/scene/scene_system.cpp"
	"src/implementation/scene/wm_simple_scene_system.h"
	"src/implementation/scene/wm_simple_scene_system.cpp"
	"src/implementation/scene/wm_node.h"
	"src/implementation/scene/wm_node.cpp"
	"src/interface/scene/transform.h"
	"src/interface/scene/transform.cpp"
	"src/implementation/scene/wm_transform.h"
	"src/implementation/scene/wm_transform.cpp"
	"src/interface/component/component.h"
	"src/interface/component/camera/camera_component.h"
	"src/interface/component/camera/projection_mode.h"
	"src/interface/component/camera/camera_component.cpp"
	"src/implementation/component/camera/wm_camera_component.h"
	"src/implementation/component/camera/wm_camera_component.cpp"
	"src/interface/defines/code_generation_defines.h"
	"src/interface/scene/event/transform/transform_changed_event.h"
	"src/interface/scene/event/transform/transform_changed_event.cpp"
	"src/interface/scene/event/transform/transform_changed_type.h"
	"src/interface/scene/event/node/node_parent_changed_event.h"
	"src/interface/scene/event/node/node_parent_changed_event.cpp"
	"src/interface/core/system.cpp"
	"src/interface/component/component.cpp"
	"src/interface/core/tag.h"
	"src/interface/core/tag.cpp"
	"src/interface/scene/event/transform/transform_event.h"
	"src/interface/scene/event/transform/transform_event.cpp"
	"src/interface/scene/event/node/node_destroyed_event.h"
	"src/interface/scene/event/transform/transform_destroyed_event.h"
	"src/interface/scene/event/transform/transform_destroyed_event.cpp"
	"src/interface/scene/event/node/node_event.h"
	"src/interface/scene/event/node/node_event.cpp"
	"src/interface/scene/event/node/node_tag_changed_event.h"
	"src/interface/scene/event/node/node_tag_changed_event.cpp"
	"src/interface/scene/event/node/node_child_changed_event.cpp"
	"src/interface/scene/event/node/node_child_changed_event.h"
	"src/interface/scene/event/node/node_destroyed_event.cpp"
	"src/interface/component/event/component_event.h"
	"src/interface/component/event/component_event.cpp"
	"src/interface/component/event/component_destroyed_event.h"
	"src/interface/component/event/component_destroyed_event.cpp"
	"src/interface/component/event/component_changed_event.h"
	"src/interface/component/event/component_changed_event.cpp"
	"src/interface/event/event_listener_base.h"
	"src/interface/event/event_listener_base.cpp"
	"src/interface/audio/audio_system.h"
	"src/interface/audio/audio_system.cpp"
	"src/implementation/audio/wm_audio_system.h"
	"src/implementation/audio/wm_audio_system.cpp"
	"src/interface/resource/sound.h"
	"src/interface/resource/sound.cpp"
	"src/interface/component/audio/audio_listener_component.h"
	"src/interface/component/audio/audio_listener_component.cpp"
	"src/implementation/component/audio/wm_audio_listener_component.h"
	"src/implementation/component/audio/wm_audio_listener_component.cpp"
	"src/interface/audio/audio_distance_model.h"
	"src/interface/audio/audio_buffer.h"
	"src/interface/audio/audio_buffer.cpp"
	"src/implementation/audio/wm_openal_audio_buffer.h"
	"src/implementation/audio/wm_openal_audio_buffer.cpp"
	"src/interface/component/audio/audio_source_component.h"
	"src/interface/component/audio/audio_source_component.cpp"
	"src/implementation/component/audio/wm_audio_source_component.h"
	"src/implementation/component/audio/wm_audio_source_component.cpp"
	"src/interface/audio/audio_source_state.h"
	"src/interface/audio/audio_source.h"
	"src/interface/audio/audio_source.cpp"
	"src/implementation/audio/wm_openal_audio_source.h"
	"src/implementation/audio/wm_openal_audio_source.cpp"
	"src/interface/audio/audio_context.h"
	"src/interface/audio/audio_context.cpp"
	"src/implementation/audio/wm_openal_audio_context.h"
	"src/implementation/audio/wm_openal_audio_context.cpp"
	"src/interface/audio/audio_listener.h"
	"src/interface/audio/audio_listener.cpp"
	"src/implementation/audio/wm_openal_audio_listener.h"
	"src/implementation/audio/wm_openal_audio_listener.cpp"
	"src/interface/core/parameter_container.cpp"
	"src/interface/ptr/ptr_view.h"
	"src/implementation/rendering/opengl/wm_opengl_rendering_system.h"
	"src/implementation/rendering/opengl/wm_opengl_rendering_system.cpp"
	"src/interface/rendering/rendering_api.h"
	"src/interface/window/vsync_mode.h"
	"src/implementation/rendering/opengl/wm_opengl_rendering_context.h"
	"src/implementation/rendering/opengl/wm_opengl_rendering_context.cpp"
	"src/interface/rendering/rendering_context.h"
	"src/interface/rendering/rendering_context.cpp"
	"src/implementation/rendering/vulkan/wm_vulkan_rendering_context.h"
	"src/implementation/rendering/vulkan/wm_vulkan_rendering_context.cpp"
	"src/implementation/rendering/wm_gpu_matrices_ubo.h"
	"src/implementation/rendering/wm_gpu_vertex.h"

	"../lib/imgui/imgui.cpp"
	"../lib/imgui/imgui_demo.cpp"
	"../lib/imgui/imgui_draw.cpp"
	"../lib/imgui/imgui_tables.cpp"
	"../lib/imgui/imgui_widgets.cpp"
	"../lib/imgui/backends/imgui_impl_glfw.cpp"
	"../lib/imgui/backends/imgui_impl_opengl3.cpp"
	"../lib/imgui/backends/imgui_impl_dx11.cpp"
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
target_include_directories("${WM_WINDMILL}" PUBLIC "${WM_LIB_PATH_IMGUI}")

#VULKAN
find_package(Vulkan REQUIRED)
if(Vulkan_FOUND)
	target_link_libraries("${WM_WINDMILL}" PRIVATE Vulkan::Vulkan)
	target_include_directories("${WM_WINDMILL}" PRIVATE "${Vulkan_INCLUDE_DIRS}")
	if(Vulkan_glslc_FOUND)
		message(STATUS "glslc found")
		set(WM_GLSL_FILES "imgui/vertex.vert" "imgui/fragment.frag" "lambertian/vertex.vert" "lambertian/fragment.frag")
		foreach(WM_GLSL_FILE IN LISTS WM_GLSL_FILES)
			set(WM_GLSL_FILE_PATH "${WM_SOURCE_DIR}/windmill/res/shader/glsl/${WM_GLSL_FILE}")
			get_filename_component(WM_GLSL_FILE_DIRECTORY "${WM_GLSL_FILE_PATH}" DIRECTORY)
			get_filename_component(WM_GLSL_FILE_NAME "${WM_GLSL_FILE_PATH}" NAME)
			set(WM_SPV_FILE_PATH "${WM_GLSL_FILE_DIRECTORY}/bin/$<$<CONFIG:DEBUG>:debug_>${WM_GLSL_FILE_NAME}.spv")
			add_custom_command(
				MAIN_DEPENDENCY "${WM_GLSL_FILE_PATH}"
				OUTPUT "${WM_SPV_FILE_PATH}"
				COMMAND Vulkan::glslc "$<$<CONFIG:DEBUG>:-g>" "$<IF:$<CONFIG:DEBUG>,-O0,-O>" "-o" "${WM_SPV_FILE_PATH}" "${WM_GLSL_FILE_PATH}"
			)
			list(APPEND WM_SPV_FILE_PATHS "${WM_SPV_FILE_PATH}")
		endforeach()
		add_custom_target(WM_GLSL_COMPILATION DEPENDS "${WM_SPV_FILE_PATHS}")
		add_dependencies("${WM_WINDMILL}" WM_GLSL_COMPILATION)
	else()
		message(FATAL_ERROR "Couldn't find glslc")
	endif()
else()
	message(FATAL_ERROR "Couldn't find Vulkan")
endif()

#DIRECT3D11
if(WIN32)
	target_sources("${WM_WINDMILL}" PRIVATE
		"src/implementation/rendering/direct3d11/wm_direct3d11_rendering_system.h"
		"src/implementation/rendering/direct3d11/wm_direct3d11_rendering_system.cpp"
		"src/implementation/rendering/direct3d11/wm_direct3d11_rendering_context.h"
		"src/implementation/rendering/direct3d11/wm_direct3d11_rendering_context.cpp"
	)
	target_link_libraries("${WM_WINDMILL}" PRIVATE d3d11 dxgi d3dcompiler)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		target_link_libraries("${WM_WINDMILL}" PRIVATE dxguid)
	endif()
	find_program(FXC fxc REQUIRED)
	message(STATUS "fxc found")
	set(WM_HLSL_FILES "lambertian/vertex.hlsl" "lambertian/pixel.hlsl")
	foreach(WM_HLSL_FILE IN LISTS WM_HLSL_FILES)
		set(WM_HLSL_FILE_PATH "${WM_SOURCE_DIR}/windmill/res/shader/hlsl/${WM_HLSL_FILE}")
		get_filename_component(WM_HLSL_FILE_DIRECTORY "${WM_HLSL_FILE_PATH}" DIRECTORY)
		get_filename_component(WM_HLSL_FILE_NAME "${WM_HLSL_FILE_PATH}" NAME)
		set(WM_CSO_FILE_PATH "${WM_HLSL_FILE_DIRECTORY}/bin/$<$<CONFIG:DEBUG>:debug_>${WM_HLSL_FILE_NAME}.cso")
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
if(WIN32)
	target_compile_definitions("${WM_WINDMILL}" PRIVATE "NOMINMAX" "GLFW_EXPOSE_NATIVE_WIN32")
	set(PLATFORM_SPECIFIC_HEADERS "<Windows.h>" "<dxgi.h>" "<d3d11_1.h>" "<d3dcompiler.h>" "<GLFW/glfw3native.h>")
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		list(APPEND PLATFORM_SPECIFIC_HEADERS "<dxgidebug.h>")
	endif()
endif()
target_precompile_headers(
	"${WM_WINDMILL}"
	PRIVATE
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
		"<imgui.h>"
		"<AL/al.h>"
		"<AL/alc.h>"
		"<sndfile.h>"
		"${PLATFORM_SPECIFIC_HEADERS}"
)
#resources
add_custom_command(
	OUTPUT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res"
	COMMAND ${CMAKE_COMMAND} -E create_symlink "${WM_SOURCE_DIR}/windmill/res" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res"
	VERBATIM
)
add_custom_target(WM_RES_SYMLINK DEPENDS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res")
add_dependencies("${WM_WINDMILL}" WM_RES_SYMLINK)
