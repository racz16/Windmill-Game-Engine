#include "core/engine.h"
#include "defines/log_defines.h"
#include "core/utility.h"
#include "window/event/gamepad_connection_event.h"
#include "window/event/window_position_event.h"
#include "window/event/mouse_position_event.h"
#include "window/event/window_closed_event.h"
#include "window/event/window_size_event.h"
#include "window/event/window_framebuffer_size_event.h"
#include "window/event/window_content_scale_event.h"
#include "window/event/window_minimization_event.h"
#include "window/event/window_maximization_event.h"
#include "window/event/window_focus_event.h"
#include "window/event/window_refresh_required_event.h"
#include "window/event/keyboard_button_event.h"
#include "window/event/keyboard_character_event.h"
#include "window/event/mouse_button_event.h"
#include "window/event/mouse_scroll_event.h"
#include "window/event/window_drag_and_drop_event.h"
#include "window/event/mouse_hover_event.h"

#include "wm_glfw_window_system.h"

namespace wm {

	wm_glfw_window_system::wm_glfw_window_system(const glm::ivec2& size, const std::string& title, const bool fullscreen, const bool visible): title(title), fullscreen(fullscreen), input_handler(nullptr) {
	#ifdef WM_BUILD_DEBUG
		set_error_callback();
	#endif
		initialize_glfw();
		set_window_hints(visible);
		create_window(size);
		initialize_input();
		add_window_event_handlers();
		add_input_event_handlers();
		WM_LOG_INFO_1("GLFW window system created");
	}

	void wm_glfw_window_system::set_error_callback() const {
		glfwSetErrorCallback([](int error, const char* description) {
			std::string message = "GLFW error: ";
			if(error == GLFW_NOT_INITIALIZED) {
				message += "NOT_INITIALIZED";
			} else if(error == GLFW_NO_CURRENT_CONTEXT) {
				message += "NO_CURRENT_CONTEXT";
			} else if(error == GLFW_INVALID_ENUM) {
				message += "INVALID_ENUM";
			} else if(error == GLFW_INVALID_VALUE) {
				message += "INVALID_VALUE";
			} else if(error == GLFW_OUT_OF_MEMORY) {
				message += "OUT_OF_MEMORY";
			} else if(error == GLFW_API_UNAVAILABLE) {
				message += "API_UNAVAILABLE";
			} else if(error == GLFW_VERSION_UNAVAILABLE) {
				message += "VERSION_UNAVAILABLE";
			} else if(error == GLFW_PLATFORM_ERROR) {
				message += "PLATFORM_ERROR";
			} else if(error == GLFW_FORMAT_UNAVAILABLE) {
				message += "FORMAT_UNAVAILABLE";
			} else if(error == GLFW_NO_WINDOW_CONTEXT) {
				message += "NO_WINDOW_CONTEXT";
			}
			message += std::string(", ") + std::string(description);
			WM_LOG_ERROR(message, __FUNCTION__, __LINE__);
		});
	}

	void wm_glfw_window_system::initialize_glfw() const {
		const auto glfw_init = glfwInit();
		WM_ASSERT(glfw_init == GLFW_TRUE);
		glfwSetJoystickCallback([](int jid, int event) {
			engine::get_event_system()->emit_event(gamepad_connection_event::get_key(), gamepad_connection_event(event == GLFW_CONNECTED, jid));
		});
		WM_LOG_INFO_2("GLFW initialized");
	}

	void wm_glfw_window_system::set_window_hints(const bool visible) {
		glfwWindowHint(GLFW_VISIBLE, visible);
		auto selected_api = rendering_system::get_rendering_api();
		if(selected_api == rendering_api::vulkan) {
			WM_ASSERT(glfwVulkanSupported());
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		} else if(selected_api == rendering_api::direct3d11) {
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		} else if(selected_api == rendering_api::opengl) {
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_SAMPLES, 8);
		#ifdef WM_BUILD_DEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		#endif
		}
	}

	void wm_glfw_window_system::create_window(const glm::ivec2& size) {
		const auto monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
		window_handle = glfwCreateWindow(size.x, size.y, title.c_str(), monitor, nullptr);
		WM_ASSERT(window_handle);
		WM_LOG_INFO_2("GLFW window created");
	}

	void wm_glfw_window_system::initialize_input() {
		glfwSetInputMode(window_handle, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
		if(glfwRawMouseMotionSupported()) {
			glfwSetInputMode(window_handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}
		input_handler = ptr<wm_glfw_window_input_handler>(new wm_glfw_window_input_handler(window_handle));
	}

	void wm_glfw_window_system::add_window_event_handlers() {
		glfwSetWindowCloseCallback(window_handle, [](GLFWwindow* window) {
			const auto event = window_closed_event();
			engine::get_event_system()->emit_event<window_closed_event>(window_closed_event::get_key(), event);
		});
		glfwSetWindowSizeCallback(window_handle, [](GLFWwindow* window, int width, int height) {
			const auto event = window_size_event(glm::ivec2(width, height));
			engine::get_event_system()->emit_event<window_size_event>(window_size_event::get_key(), event);
		});
		glfwSetFramebufferSizeCallback(window_handle, [](GLFWwindow* window, int width, int height) {
			const auto event = window_framebuffer_size_event(glm::ivec2(width, height));
			engine::get_event_system()->emit_event<window_framebuffer_size_event>(window_framebuffer_size_event::get_key(), event);
		});
		glfwSetWindowContentScaleCallback(window_handle, [](GLFWwindow* window, float x_scale, float y_scale) {
			const auto event = window_content_scale_event(glm::vec2(x_scale, y_scale));
			engine::get_event_system()->emit_event<window_content_scale_event>(window_content_scale_event::get_key(), event);
		});
		glfwSetWindowPosCallback(window_handle, [](GLFWwindow* window, int x_pos, int y_pos) {
			const auto event = window_position_event(glm::ivec2(x_pos, y_pos));
			engine::get_event_system()->emit_event<window_position_event>(window_position_event::get_key(), event);
		});
		glfwSetWindowIconifyCallback(window_handle, [](GLFWwindow* window, int iconified) {
			const auto event = window_minimization_event(iconified == GLFW_TRUE);
			engine::get_event_system()->emit_event<window_minimization_event>(window_minimization_event::get_key(), event);
		});
		glfwSetWindowMaximizeCallback(window_handle, [](GLFWwindow* window, int maximized) {
			const auto event = window_maximization_event(maximized == GLFW_TRUE);
			engine::get_event_system()->emit_event<window_maximization_event>(window_maximization_event::get_key(), event);
		});
		glfwSetWindowFocusCallback(window_handle, [](GLFWwindow* window, int focused) {
			const auto event = window_focus_event(focused == GLFW_TRUE);
			engine::get_event_system()->emit_event<window_focus_event>(window_focus_event::get_key(), event);
		});
		glfwSetWindowRefreshCallback(window_handle, [](GLFWwindow* window) {
			const auto event = window_refresh_required_event();
			engine::get_event_system()->emit_event<window_refresh_required_event>(window_refresh_required_event::get_key(), event);
		});
	}

	void wm_glfw_window_system::add_input_event_handlers() {
		glfwSetKeyCallback(window_handle, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			const auto keyboard_button = static_cast<wm::keyboard_button>(key);
			const auto state = button_state(static_cast<button_action>(action));
			const bool shift = mods & GLFW_MOD_SHIFT;
			const bool ctrl = mods & GLFW_MOD_CONTROL;
			const bool alt = mods & GLFW_MOD_ALT;
			const bool super = mods & GLFW_MOD_SUPER;
			const bool caps_lock = mods & GLFW_MOD_CAPS_LOCK;
			const bool num_lock = mods & GLFW_MOD_NUM_LOCK;
			const auto event = keyboard_button_event(keyboard_button, state, scancode, shift, ctrl, alt, super, caps_lock, num_lock);
			engine::get_event_system()->emit_event<keyboard_button_event>(keyboard_button_event::get_key(), event);
		});
		glfwSetCharCallback(window_handle, [](GLFWwindow* window, unsigned int code_point) {
			const auto event = keyboard_character_event(code_point);
			engine::get_event_system()->emit_event<keyboard_character_event>(keyboard_character_event::get_key(), event);
		});
		glfwSetCursorPosCallback(window_handle, [](GLFWwindow* window, double x_pos, double y_pos) {
			const auto event = mouse_position_event(glm::dvec2(x_pos, y_pos));
			engine::get_event_system()->emit_event<mouse_position_event>(mouse_position_event::get_key(), event);
		});
		glfwSetCursorEnterCallback(window_handle, [](GLFWwindow* window, int entered) {
			const auto event = mouse_hover_event(entered);
			engine::get_event_system()->emit_event<mouse_hover_event>(mouse_hover_event::get_key(), event);
		});
		glfwSetMouseButtonCallback(window_handle, [](GLFWwindow* window, int button, int action, int mods) {
			const auto keyboard_button = static_cast<wm::mouse_button>(button);
			const auto press = action == GLFW_PRESS;
			const bool shift = mods & GLFW_MOD_SHIFT;
			const bool ctrl = mods & GLFW_MOD_CONTROL;
			const bool alt = mods & GLFW_MOD_ALT;
			const bool super = mods & GLFW_MOD_SUPER;
			const bool caps_lock = mods & GLFW_MOD_CAPS_LOCK;
			const bool num_lock = mods & GLFW_MOD_NUM_LOCK;
			const auto event = mouse_button_event(keyboard_button, press, shift, ctrl, alt, super, caps_lock, num_lock);
			engine::get_event_system()->emit_event<mouse_button_event>(mouse_button_event::get_key(), event);
		});
		glfwSetScrollCallback(window_handle, [](GLFWwindow* window, double x_offset, double y_offset) {
			const auto event = mouse_scroll_event(glm::dvec2(x_offset, y_offset));
			engine::get_event_system()->emit_event<mouse_scroll_event>(mouse_scroll_event::get_key(), event);
		});
		glfwSetDropCallback(window_handle, [](GLFWwindow* window, int count, const char** paths) {
			std::vector<std::string> file_paths;
			for(int32_t i = 0; i < count; i++) {
				file_paths.push_back(paths[i]);
			}
			const auto event = window_drag_and_drop_event(file_paths);
			engine::get_event_system()->emit_event<window_drag_and_drop_event>(window_drag_and_drop_event::get_key(), event);
		});
	}

	void wm_glfw_window_system::update() {
		glfwPollEvents();
		input_handler->update();
		WM_LOG_INFO_3("GLFW window system updated");
	}

	std::any wm_glfw_window_system::get_native_id() const {
		return window_handle;
	}

	bool wm_glfw_window_system::is_closing() const {
		return glfwWindowShouldClose(window_handle);
	}

	void wm_glfw_window_system::set_closing(const bool closing) {
		glfwSetWindowShouldClose(window_handle, closing);
		WM_LOG_INFO_2("GLFW window set to closing");
	}

	bool wm_glfw_window_system::is_visible() const {
		return glfwGetWindowAttrib(window_handle, GLFW_VISIBLE);
	}

	void wm_glfw_window_system::set_visibility(const bool visibility) {
		if(visibility) {
			glfwShowWindow(window_handle);
		} else {
			glfwHideWindow(window_handle);
		}
		WM_LOG_INFO_2("GLFW window visibility state changed to " + std::to_string(visibility));
	}

	glm::ivec2 wm_glfw_window_system::get_position() const {
		int32_t x_pos, y_pos;
		glfwGetWindowPos(window_handle, &x_pos, &y_pos);
		return glm::ivec2(x_pos, y_pos);
	}

	void wm_glfw_window_system::set_position(const glm::ivec2& position) {
		glfwSetWindowPos(window_handle, position.x, position.y);
		WM_LOG_INFO_2("GLFW window position changed to " + utility::to_string(position));
	}

	std::string wm_glfw_window_system::get_title() const {
		return this->title;
	}

	void wm_glfw_window_system::set_title(const std::string& title) {
		this->title = title;
		glfwSetWindowTitle(window_handle, title.c_str());
		WM_LOG_INFO_2("GLFW window title changed to " + title);
	}

	bool wm_glfw_window_system::is_minimized() const {
		return glfwGetWindowAttrib(window_handle, GLFW_ICONIFIED);
	}

	void wm_glfw_window_system::set_minimization(const bool minimization) {
		if(minimization) {
			glfwIconifyWindow(window_handle);
		} else {
			glfwRestoreWindow(window_handle);
		}
		WM_LOG_INFO_2("GLFW window minimization state changed to " + std::to_string(minimization));
	}

	bool wm_glfw_window_system::is_maximized() const {
		return glfwGetWindowAttrib(window_handle, GLFW_MAXIMIZED);
	}

	void wm_glfw_window_system::set_maximization(const bool maximization) {
		if(maximization) {
			glfwMaximizeWindow(window_handle);
		} else {
			glfwRestoreWindow(window_handle);
		}
		WM_LOG_INFO_2("GLFW window maximization state changed to " + std::to_string(maximization));
	}

	glm::ivec2 wm_glfw_window_system::get_aspect_ratio() const {
		return aspect_ratio;
	}

	void wm_glfw_window_system::set_aspect_ratio(const glm::ivec2& aspect_ratio) {
		this->aspect_ratio = aspect_ratio;
		glfwSetWindowAspectRatio(window_handle, aspect_ratio.x, aspect_ratio.y);
		WM_LOG_INFO_2("GLFW window aspect ratio changed to " + std::to_string(aspect_ratio.x) + ":" + std::to_string(aspect_ratio.y));
	}

	glm::ivec2 wm_glfw_window_system::get_minimum_size() const {
		return minimum_size;
	}

	void wm_glfw_window_system::set_minimum_size(const glm::ivec2& minimum_size) {
		this->minimum_size = minimum_size;
		glfwSetWindowSizeLimits(window_handle, minimum_size.x, minimum_size.y, maximum_size.x, maximum_size.y);
		WM_LOG_INFO_2("GLFW window min size changed to " + utility::to_string(minimum_size));
	}

	glm::ivec2 wm_glfw_window_system::get_maximum_size() const {
		return maximum_size;
	}

	void wm_glfw_window_system::set_maximum_size(const glm::ivec2& maximum_size) {
		this->maximum_size = maximum_size;
		glfwSetWindowSizeLimits(window_handle, minimum_size.x, minimum_size.y, maximum_size.x, maximum_size.y);
		WM_LOG_INFO_2("GLFW window max size changed to " + utility::to_string(maximum_size));
	}

	glm::ivec2 wm_glfw_window_system::get_size() const {
		int32_t width, height;
		glfwGetWindowSize(window_handle, &width, &height);
		return glm::ivec2(width, height);
	}

	void wm_glfw_window_system::set_size(const glm::ivec2& size) {
		glfwSetWindowSize(window_handle, size.x, size.y);
		WM_LOG_INFO_2("GLFW window size changed to " + utility::to_string(size));
	}

	glm::ivec2 wm_glfw_window_system::get_framebuffer_size() const {
		int32_t width, height;
		glfwGetFramebufferSize(window_handle, &width, &height);
		return glm::ivec2(width, height);
	}

	glm::ivec4 wm_glfw_window_system::get_frame_size() const {
		int32_t left, top, right, bottom;
		glfwGetWindowFrameSize(window_handle, &left, &top, &right, &bottom);
		return glm::ivec4(left, top, right, bottom);
	}

	glm::vec2 wm_glfw_window_system::get_content_scale() const {
		float x_scale, y_scale;
		glfwGetWindowContentScale(window_handle, &x_scale, &y_scale);
		return glm::vec2(x_scale, y_scale);
	}

	bool wm_glfw_window_system::is_fullscreen() const {
		return fullscreen;
	}

	void wm_glfw_window_system::set_fullscreen(const bool fullscreen) {
		this->fullscreen = fullscreen;
		set_window_monitor();
		WM_LOG_INFO_2("GLFW window fullscreen state changed to " + std::to_string(fullscreen));
	}

	int32_t wm_glfw_window_system::get_refresh_rate() const {
		return refresh_rate;
	}

	void wm_glfw_window_system::set_refresh_rate(const int32_t refresh_rate) {
		this->refresh_rate = refresh_rate;
		set_window_monitor();
		WM_LOG_INFO_2("GLFW window refresh rate changed to " + std::to_string(refresh_rate));
	}

	void wm_glfw_window_system::set_window_monitor() {
		const auto size = get_size();
		const auto position = get_position();
		const auto monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
		glfwSetWindowMonitor(window_handle, monitor, position.x, position.y, size.x, size.y, refresh_rate);
	}

	bool wm_glfw_window_system::is_resizable() const {
		return glfwGetWindowAttrib(window_handle, GLFW_RESIZABLE);
	}

	void wm_glfw_window_system::set_resizable(const bool resizable) {
		glfwSetWindowAttrib(window_handle, GLFW_RESIZABLE, resizable);
		WM_LOG_INFO_2("GLFW window resizable state changed to " + std::to_string(resizable));
	}

	bool wm_glfw_window_system::is_decorated() const {
		return glfwGetWindowAttrib(window_handle, GLFW_DECORATED);
	}

	void wm_glfw_window_system::set_decorated(const bool decorated) {
		glfwSetWindowAttrib(window_handle, GLFW_DECORATED, decorated);
		WM_LOG_INFO_2("GLFW window decorated state changed to " + std::to_string(decorated));
	}

	bool wm_glfw_window_system::is_always_on_top() const {
		return glfwGetWindowAttrib(window_handle, GLFW_FLOATING);
	}

	void wm_glfw_window_system::set_always_on_top(const bool always_on_top) {
		glfwSetWindowAttrib(window_handle, GLFW_FLOATING, always_on_top);
		WM_LOG_INFO_2("GLFW window always on top state changed to " + std::to_string(always_on_top));
	}

	bool wm_glfw_window_system::is_focus_on_show() const {
		return glfwGetWindowAttrib(window_handle, GLFW_FOCUS_ON_SHOW);
	}

	void wm_glfw_window_system::set_focus_on_show(const bool focus_on_show) {
		glfwSetWindowAttrib(window_handle, GLFW_FOCUS_ON_SHOW, focus_on_show);
		WM_LOG_INFO_2("GLFW window focus on show state changed to " + std::to_string(focus_on_show));
	}

	bool wm_glfw_window_system::is_focused() const {
		return glfwGetWindowAttrib(window_handle, GLFW_FOCUSED);
	}

	void wm_glfw_window_system::focus() {
		glfwFocusWindow(window_handle);
		WM_LOG_INFO_2("GLFW window requested focus");
	}

	void wm_glfw_window_system::request_attention() {
		glfwRequestWindowAttention(window_handle);
		WM_LOG_INFO_2("GLFW window requested attention");
	}

	cursor_mode wm_glfw_window_system::get_cursor_mode() const {
		const auto mode = glfwGetInputMode(window_handle, GLFW_CURSOR);
		switch(mode) {
			case GLFW_CURSOR_NORMAL: return cursor_mode::normal;
			case GLFW_CURSOR_HIDDEN: return cursor_mode::hidden;
			case GLFW_CURSOR_DISABLED: return cursor_mode::disabled;
			default: WM_THROW_ERROR("invalid cursor mode");
		}
	}

	void wm_glfw_window_system::set_cursor_mode(const cursor_mode mode) {
		switch(mode) {
			case cursor_mode::normal:
				glfwSetInputMode(window_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				break;
			case cursor_mode::hidden:
				glfwSetInputMode(window_handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				break;
			case cursor_mode::disabled:
				glfwSetInputMode(window_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				break;
			default: WM_THROW_ERROR("invalid cursor mode");
		}
		WM_LOG_INFO_2("GLFW window cursor mode changed");
	}

	cursor_shape wm_glfw_window_system::get_cursor_shape() const {
		return cursor_shape;
	}

	void wm_glfw_window_system::set_cursor_shape(const wm::standard_cursor_shape cursor_shape) {
		destroy_cursor();
		this->cursor_shape = static_cast<wm::cursor_shape>(cursor_shape);
		switch(cursor_shape) {
			case standard_cursor_shape::normal:
				cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
				break;
			case standard_cursor_shape::text:
				cursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
				break;
			case standard_cursor_shape::crosshair:
				cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
				break;
			case standard_cursor_shape::hand:
				cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
				break;
			case standard_cursor_shape::horizontal_resize:
				cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
				break;
			case standard_cursor_shape::vertical_resize:
				cursor = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
				break;
			default:
				WM_THROW_ERROR("invalid cursor shape");
		}
		glfwSetCursor(window_handle, cursor);
		WM_LOG_INFO_2("GLFW window cursor shape changed");
	}

	void wm_glfw_window_system::set_cursor_shape(const std::string& file_path) {
		auto image = engine::get_resource_system()->get_image(file_path);
		GLFWimage glfw_image{image->get_size().x, image->get_size().y, image->get_pixels()};
		destroy_cursor();
		cursor_shape = cursor_shape::custom;
		cursor = glfwCreateCursor(&glfw_image, 0, 0);
		glfwSetCursor(window_handle, cursor);
		image.destroy();
		WM_LOG_INFO_2("GLFW window cursor shape changed");
	}

	void wm_glfw_window_system::destroy_cursor() {
		if(cursor != nullptr) {
			glfwDestroyCursor(cursor);
			cursor = nullptr;
		}
	}

	void wm_glfw_window_system::set_icon(const std::string& file_path) const {
		auto image = engine::get_resource_system()->get_image(file_path);
		GLFWimage glfw_image{image->get_size().x, image->get_size().y, image->get_pixels()};

		glfwSetWindowIcon(window_handle, 1, &glfw_image);

		image.destroy();
	}

	ptr_view<window_input_handler> wm_glfw_window_system::get_input_handler() const {
		return input_handler.convert<window_input_handler>();
	}

	std::vector<const char*> wm_glfw_window_system::get_required_extensions() const {
		uint32_t window_extension_count;
		const auto window_extensions = glfwGetRequiredInstanceExtensions(&window_extension_count);
		return std::vector<const char*>(window_extensions, window_extensions + window_extension_count);
	}

	void wm_glfw_window_system::create_surface(const void* context, void* surface) {
		WM_ASSERT_VULKAN(glfwCreateWindowSurface(*reinterpret_cast<const VkInstance*>(context), window_handle, nullptr, reinterpret_cast<VkSurfaceKHR*>(surface)));
		WM_LOG_INFO_2("GLFW window Vulkan surface created");
	}

	void wm_glfw_window_system::swap_buffers() {
		glfwSwapBuffers(window_handle);
	}

	vsync_mode wm_glfw_window_system::get_vsync_mode() const {
		return sync_mode;
	}

	void wm_glfw_window_system::set_vsync_mode(const vsync_mode mode) {
		glfwSwapInterval(static_cast<int>(mode));
		sync_mode = mode;
	}

	void wm_glfw_window_system::make_context_current() {
		glfwMakeContextCurrent(window_handle);
	}

	window_system::get_function_address_t wm_glfw_window_system::get_function_address() const {
		return reinterpret_cast<window_system::get_function_address_t>(glfwGetProcAddress);
	}

	video_mode wm_glfw_window_system::get_current_video_mode() const {
		const auto vm = glfwGetVideoMode(glfwGetPrimaryMonitor());
		return video_mode(
			glm::ivec2(vm->width, vm->height),
			glm::ivec3(vm->redBits, vm->greenBits, vm->blueBits),
			vm->refreshRate
		);
	}

	std::vector<video_mode> wm_glfw_window_system::get_all_video_modes() const {
		int vm_count;
		const auto vms = glfwGetVideoModes(glfwGetPrimaryMonitor(), &vm_count);
		std::vector<video_mode> video_modes;
		for(int32_t i = 0; i < vm_count; i++) {
			const auto vm = vms[i];
			video_modes.push_back(video_mode(
				glm::ivec2(vm.width, vm.height),
				glm::ivec3(vm.redBits, vm.greenBits, vm.blueBits),
				vm.refreshRate
			));
		}
		return video_modes;
	}

	void wm_glfw_window_system::destroy_window() {
		input_handler.destroy();
		destroy_cursor();
		glfwDestroyWindow(window_handle);
		window_handle = nullptr;
		WM_LOG_INFO_1("GLFW window destroyed");
	}

	wm_glfw_window_system::~wm_glfw_window_system() {
		destroy_window();
		glfwTerminate();
		WM_LOG_INFO_1("GLFW window system destroyed");
	}

#ifdef WM_PLATFORM_WINDOWS
	std::any wm_glfw_window_system::get_win32_handle() const {
		return glfwGetWin32Window(window_handle);
	}
#endif

}
