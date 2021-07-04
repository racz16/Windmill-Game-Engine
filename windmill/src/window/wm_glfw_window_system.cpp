#include "wm_glfw_window_system.h"
#include "engine.h"
#include "debug/to_string.h"

namespace wm {

	wm_glfw_window_system::wm_glfw_window_system(const window_system_init init)
		: title(init.title), fullscreen(init.fullscreen) {
		set_error_callback();
		initialize_glfw();
		save_screen_parameters();
		set_window_hints(init);
		create_window(init);
		// TODO: callbacks
		//glfwSetKeyCallback(window_handler, [](GLFWwindow* window, int key, int scancode, int action, int mods) {});
		//glfwSetWindowCloseCallback(window, window_close_callback);
		//glfwSetWindowSizeCallback(window, window_size_callback);
		//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		//glfwSetWindowContentScaleCallback(window, window_content_scale_callback);
		//glfwSetWindowPosCallback(window, window_pos_callback);
		//glfwSetWindowIconifyCallback(window, window_iconify_callback);
		//glfwSetWindowMaximizeCallback(window, window_maximize_callback);
		//glfwSetWindowFocusCallback(window, window_focus_callback);
		//glfwSetWindowRefreshCallback(m_handle, window_refresh_callback);
		//glfwSetMonitorCallback(monitor_callback);
		WM_LOG_INFO_1("GLFW window system constructed");
	}

	void wm_glfw_window_system::set_error_callback() {
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
			WM_LOG_ERROR(message);
		});
	}

	void wm_glfw_window_system::initialize_glfw() {
		auto glfw_init = glfwInit();
		WM_ASSERT(glfw_init == GLFW_TRUE);
	}

	void wm_glfw_window_system::save_screen_parameters() {
		auto window_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		screen_size = glm::ivec2(window_mode->width, window_mode->height);
		screen_refresh_rate = window_mode->refreshRate;
	}

	void wm_glfw_window_system::set_window_hints(const window_system_init init) {
		glfwWindowHint(GLFW_RESIZABLE, init.resizable);
		glfwWindowHint(GLFW_DECORATED, init.decorated);
		glfwWindowHint(GLFW_FLOATING, init.always_on_top);
		glfwWindowHint(GLFW_CENTER_CURSOR, init.center_cursor);
		glfwWindowHint(GLFW_MAXIMIZED, init.maximized);
		glfwWindowHint(GLFW_FOCUS_ON_SHOW, init.focus_on_show);
		glfwWindowHint(GLFW_FOCUSED, init.focused);
		glfwWindowHint(GLFW_VISIBLE, init.visible);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	}

	void wm_glfw_window_system::create_window(const window_system_init init) {
		if(fullscreen) {
			window_handler = glfwCreateWindow(screen_size.x, screen_size.y, title.c_str(), glfwGetPrimaryMonitor(), nullptr);
		} else {
			window_handler = glfwCreateWindow(init.size.x, init.size.y, title.c_str(), nullptr, nullptr);
		}
		WM_ASSERT(window_handler != nullptr);
	}

	bool wm_glfw_window_system::is_closing() const {
		return glfwWindowShouldClose(window_handler);
	}

	void wm_glfw_window_system::set_closing() {
		glfwSetWindowShouldClose(window_handler, GLFW_TRUE);
		WM_LOG_INFO_2("Window set to closing");
	}

	void wm_glfw_window_system::update() {
		glfwPollEvents();
	}

	glm::ivec2 wm_glfw_window_system::get_size() const {
		int32_t width, height;
		glfwGetWindowSize(window_handler, &width, &height);
		return glm::ivec2(width, height);
	}

	void wm_glfw_window_system::set_size(const glm::ivec2 size) {
		WM_ASSERT(!is_fullscreen());
		glfwSetWindowSize(window_handler, size.x, size.y);
		WM_LOG_INFO_2("Window size changed to " + wm::to_string(size));
	}

	glm::ivec2 wm_glfw_window_system::get_framebuffer_size() const {
		int32_t width, height;
		glfwGetFramebufferSize(window_handler, &width, &height);
		return glm::ivec2(width, height);
	}

	void wm_glfw_window_system::get_frame_size() const {
		int left, top, right, bottom;
		glfwGetWindowFrameSize(window_handler, &left, &top, &right, &bottom);
	}

	glm::vec2 wm_glfw_window_system::get_content_scale() const {
		float x_scale, y_scale;
		glfwGetWindowContentScale(window_handler, &x_scale, &y_scale);
		return glm::vec2(x_scale, y_scale);
	}

	glm::ivec2 wm_glfw_window_system::get_min_size() const {
		return min_size;
	}

	void wm_glfw_window_system::set_min_size(const glm::ivec2 min_size) {
		this->min_size = min_size;
		glfwSetWindowSizeLimits(window_handler, min_size.x, min_size.y, max_size.x, max_size.y);
		WM_LOG_INFO_2("Window min size changed to " + wm::to_string(min_size));
	}

	glm::ivec2 wm_glfw_window_system::get_max_size() const {
		return max_size;
	}

	void wm_glfw_window_system::set_max_size(const glm::ivec2 max_size) {
		this->max_size = max_size;
		glfwSetWindowSizeLimits(window_handler, min_size.x, min_size.y, max_size.x, max_size.y);
		WM_LOG_INFO_2("Window max size changed to " + wm::to_string(max_size));
	}

	float wm_glfw_window_system::get_aspect_ratio_width() const {
		return aspect_ratio_width;
	}

	float wm_glfw_window_system::get_aspect_ratio_height() const {
		return aspect_ratio_height;
	}

	void wm_glfw_window_system::set_aspect_ratio(const int32_t width, const int32_t height) {
		glfwSetWindowAspectRatio(window_handler, width, height);
		WM_LOG_INFO_2("Window aspect ratio changed to " + std::to_string(width) + ":" + std::to_string(height));
	}

	glm::ivec2 wm_glfw_window_system::get_position() const {
		int32_t x_pos, y_pos;
		glfwGetWindowPos(window_handler, &x_pos, &y_pos);
		return glm::ivec2(x_pos, y_pos);
	}

	void wm_glfw_window_system::set_position(const glm::ivec2 position) {
		glfwSetWindowPos(window_handler, position.x, position.y);
		WM_LOG_INFO_2("Window position changed to " + wm::to_string(position));
	}

	std::string wm_glfw_window_system::get_title() const {
		return this->title;
	}

	void wm_glfw_window_system::set_title(const std::string& title) {
		this->title = title;
		glfwSetWindowTitle(window_handler, title.c_str());
	}

	bool wm_glfw_window_system::is_minimized() const {
		return glfwGetWindowAttrib(window_handler, GLFW_ICONIFIED);
	}

	void wm_glfw_window_system::set_minimization(const bool minimization) {
		if(minimization) {
			glfwIconifyWindow(window_handler);
		} else {
			glfwRestoreWindow(window_handler);
		}
		WM_LOG_INFO_2("Window minimization state changed to " + std::to_string(minimization));
	}

	bool wm_glfw_window_system::is_maximized() const {
		return glfwGetWindowAttrib(window_handler, GLFW_MAXIMIZED);
	}

	void wm_glfw_window_system::set_maximization(const bool maximization) {
		if(maximization) {
			glfwMaximizeWindow(window_handler);
		} else {
			glfwRestoreWindow(window_handler);
		}
		WM_LOG_INFO_2("Window maximization state changed to " + std::to_string(maximization));
	}

	bool wm_glfw_window_system::is_visible() const {
		return glfwGetWindowAttrib(window_handler, GLFW_VISIBLE);
	}

	void wm_glfw_window_system::set_visibility(const bool visibility) {
		if(visibility) {
			glfwShowWindow(window_handler);
		} else {
			glfwHideWindow(window_handler);
		}
		WM_LOG_INFO_2("Window visibility state changed to " + std::to_string(visibility));
	}

	bool wm_glfw_window_system::is_focused() const {
		return glfwGetWindowAttrib(window_handler, GLFW_FOCUSED);
	}

	void wm_glfw_window_system::focus() {
		glfwFocusWindow(window_handler);
		WM_LOG_INFO_2("Window requested focus");
	}

	void wm_glfw_window_system::request_attention() {
		glfwRequestWindowAttention(window_handler);
		WM_LOG_INFO_2("Window requested attention");
	}

	bool wm_glfw_window_system::is_hovered() const {
		return glfwGetWindowAttrib(window_handler, GLFW_HOVERED);
	}

	bool wm_glfw_window_system::is_resizable() const {
		return glfwGetWindowAttrib(window_handler, GLFW_RESIZABLE);
	}

	bool wm_glfw_window_system::is_decorated() const {
		return glfwGetWindowAttrib(window_handler, GLFW_DECORATED);
	}

	bool wm_glfw_window_system::is_always_on_top() const {
		return glfwGetWindowAttrib(window_handler, GLFW_FLOATING);
	}

	bool wm_glfw_window_system::is_focus_on_show() const {
		return glfwGetWindowAttrib(window_handler, GLFW_FOCUS_ON_SHOW);
	}

	bool wm_glfw_window_system::is_fullscreen() const {
		return fullscreen;
	}

	void wm_glfw_window_system::set_fullscreen(const bool fullscreen) {
		this->fullscreen = fullscreen;
		if(fullscreen) {
			glfwSetWindowMonitor(window_handler, glfwGetPrimaryMonitor(), 0, 0, screen_size.x, screen_size.y, GLFW_DONT_CARE);
		} else {
			auto position = get_position();
			auto size = get_size();
			glfwSetWindowMonitor(window_handler, nullptr, position.x, position.y, size.x, size.y, GLFW_DONT_CARE);
		}
		WM_LOG_INFO_2("Window fullscreen state changed to " + std::to_string(fullscreen));
	}

	glm::ivec2 wm_glfw_window_system::get_screen_size() const {
		return screen_size;
	}

	int32_t wm_glfw_window_system::get_screen_refresh_rate() const {
		return screen_refresh_rate;
	}

	wm_glfw_window_system::~wm_glfw_window_system() {
		glfwDestroyWindow(window_handler);
		glfwTerminate();
		WM_LOG_INFO_1("GLFW window system destructed");
	}

}
