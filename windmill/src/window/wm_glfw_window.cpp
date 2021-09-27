#include "wm_glfw_window.h"
#include "debug/log/log_level.h"
#include "core/engine.h"
#include "core/utility.h"
#include "core/defines.h"
#include "wm_glfw_window_system.h"

namespace wm {

	int32_t wm_glfw_window::last_id = 0;

	wm_glfw_window::wm_glfw_window(const glm::ivec2& size, const std::string& title, const bool fullscreen, const bool visible): title(title), fullscreen(fullscreen), id(++last_id) {
		set_window_hints(visible);
		create_window(size);
		WM_LOG_INFO_1("GLFW window constructed");
	}

	void wm_glfw_window::set_window_hints(const bool visible) {
		glfwWindowHint(GLFW_VISIBLE, visible);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	}

	void wm_glfw_window::create_window(const glm::ivec2& size) {
		const auto monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
		window_handler = glfwCreateWindow(size.x, size.y, title.c_str(), monitor, nullptr);
		WM_ASSERT(window_handler != nullptr);
	}

	int32_t wm_glfw_window::get_id() const {
		return id;
	}

	bool wm_glfw_window::is_closing() const {
		return glfwWindowShouldClose(window_handler);
	}

	void wm_glfw_window::set_closing(const bool closing) {
		glfwSetWindowShouldClose(window_handler, closing);
		WM_LOG_INFO_2("GLFW window set to closing");
	}

	bool wm_glfw_window::is_visible() const {
		return glfwGetWindowAttrib(window_handler, GLFW_VISIBLE);
	}

	void wm_glfw_window::set_visibility(const bool visibility) {
		if(visibility) {
			glfwShowWindow(window_handler);
		} else {
			glfwHideWindow(window_handler);
		}
		WM_LOG_INFO_2("GLFW window visibility state changed to " + std::to_string(visibility));
	}

	glm::ivec2 wm_glfw_window::get_position() const {
		int32_t x_pos, y_pos;
		glfwGetWindowPos(window_handler, &x_pos, &y_pos);
		return glm::ivec2(x_pos, y_pos);
	}

	void wm_glfw_window::set_position(const glm::ivec2& position) {
		glfwSetWindowPos(window_handler, position.x, position.y);
		WM_LOG_INFO_2("GLFW window position changed to " + wm::utility::to_string(position));
	}

	std::string wm_glfw_window::get_title() const {
		return this->title;
	}

	void wm_glfw_window::set_title(const std::string& title) {
		this->title = title;
		glfwSetWindowTitle(window_handler, title.c_str());
		WM_LOG_INFO_2("GLFW window title changed to " + title);
	}

	bool wm_glfw_window::is_minimized() const {
		return glfwGetWindowAttrib(window_handler, GLFW_ICONIFIED);
	}

	void wm_glfw_window::set_minimization(const bool minimization) {
		if(minimization) {
			glfwIconifyWindow(window_handler);
		} else {
			glfwRestoreWindow(window_handler);
		}
		WM_LOG_INFO_2("GLFW window minimization state changed to " + std::to_string(minimization));
	}

	bool wm_glfw_window::is_maximized() const {
		return glfwGetWindowAttrib(window_handler, GLFW_MAXIMIZED);
	}

	void wm_glfw_window::set_maximization(const bool maximization) {
		if(maximization) {
			glfwMaximizeWindow(window_handler);
		} else {
			glfwRestoreWindow(window_handler);
		}
		WM_LOG_INFO_2("GLFW window maximization state changed to " + std::to_string(maximization));
	}

	glm::vec2 wm_glfw_window::get_aspect_ratio() const {
		return aspect_ratio;
	}

	void wm_glfw_window::set_aspect_ratio(const glm::vec2 aspect_ratio) {
		this->aspect_ratio = aspect_ratio;
		glfwSetWindowAspectRatio(window_handler, aspect_ratio.x, aspect_ratio.y);
		WM_LOG_INFO_2("GLFW window aspect ratio changed to " + std::to_string(aspect_ratio.x) + ":" + std::to_string(aspect_ratio.y));
	}

	glm::ivec2 wm_glfw_window::get_minimum_size() const {
		return minimum_size;
	}

	void wm_glfw_window::set_minimum_size(const glm::ivec2& minimum_size) {
		this->minimum_size = minimum_size;
		glfwSetWindowSizeLimits(window_handler, minimum_size.x, minimum_size.y, maximum_size.x, maximum_size.y);
		WM_LOG_INFO_2("GLFW window min size changed to " + wm::utility::to_string(minimum_size));
	}

	glm::ivec2 wm_glfw_window::get_maximum_size() const {
		return maximum_size;
	}

	void wm_glfw_window::set_maximum_size(const glm::ivec2& maximum_size) {
		this->maximum_size = maximum_size;
		glfwSetWindowSizeLimits(window_handler, minimum_size.x, minimum_size.y, maximum_size.x, maximum_size.y);
		WM_LOG_INFO_2("GLFW window max size changed to " + wm::utility::to_string(maximum_size));
	}

	glm::ivec2 wm_glfw_window::get_size() const {
		int32_t width, height;
		glfwGetWindowSize(window_handler, &width, &height);
		return glm::ivec2(width, height);
	}

	void wm_glfw_window::set_size(const glm::ivec2& size) {
		glfwSetWindowSize(window_handler, size.x, size.y);
		WM_LOG_INFO_2("GLFW window size changed to " + wm::utility::to_string(size));
	}

	glm::ivec2 wm_glfw_window::get_framebuffer_size() const {
		int32_t width, height;
		glfwGetFramebufferSize(window_handler, &width, &height);
		return glm::ivec2(width, height);
	}

	glm::ivec4 wm_glfw_window::get_frame_size() const {
		int32_t left, top, right, bottom;
		glfwGetWindowFrameSize(window_handler, &left, &top, &right, &bottom);
		return glm::ivec4(left, top, right, bottom);
	}

	glm::vec2 wm_glfw_window::get_content_scale() const {
		float x_scale, y_scale;
		glfwGetWindowContentScale(window_handler, &x_scale, &y_scale);
		return glm::vec2(x_scale, y_scale);
	}

	bool wm_glfw_window::is_fullscreen() const {
		return fullscreen;
	}

	void wm_glfw_window::set_fullscreen(const bool fullscreen) {
		this->fullscreen = fullscreen;
		set_window_monitor();
		WM_LOG_INFO_2("GLFW window fullscreen state changed to " + std::to_string(fullscreen));
	}

	int32_t wm_glfw_window::get_refresh_rate() const {
		return refresh_rate;
	}

	void wm_glfw_window::set_refresh_rate(const int32_t refresh_rate) {
		this->refresh_rate = refresh_rate;
		set_window_monitor();
		WM_LOG_INFO_2("GLFW window refresh rate changed to " + std::to_string(refresh_rate));
	}

	void wm_glfw_window::set_window_monitor() {
		const auto size = get_size();
		const auto position = get_position();
		const auto monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
		glfwSetWindowMonitor(window_handler, monitor, position.x, position.y, size.x, size.y, refresh_rate);
	}

	bool wm_glfw_window::is_resizable() const {
		return glfwGetWindowAttrib(window_handler, GLFW_RESIZABLE);
	}

	void wm_glfw_window::set_resizable(const bool resizable) {
		glfwSetWindowAttrib(window_handler, GLFW_RESIZABLE, resizable);
		WM_LOG_INFO_2("GLFW window resizable state changed to " + std::to_string(resizable));
	}

	bool wm_glfw_window::is_decorated() const {
		return glfwGetWindowAttrib(window_handler, GLFW_DECORATED);
	}

	void wm_glfw_window::set_decorated(const bool decorated) {
		glfwSetWindowAttrib(window_handler, GLFW_DECORATED, decorated);
		WM_LOG_INFO_2("GLFW window decorated state changed to " + std::to_string(decorated));
	}

	bool wm_glfw_window::is_always_on_top() const {
		return glfwGetWindowAttrib(window_handler, GLFW_FLOATING);
	}

	void wm_glfw_window::set_always_on_top(const bool always_on_top) {
		glfwSetWindowAttrib(window_handler, GLFW_FLOATING, always_on_top);
		WM_LOG_INFO_2("GLFW window always on top state changed to " + std::to_string(always_on_top));
	}

	bool wm_glfw_window::is_focus_on_show() const {
		return glfwGetWindowAttrib(window_handler, GLFW_FOCUS_ON_SHOW);
	}

	void wm_glfw_window::set_focus_on_show(const bool focus_on_show) {
		glfwSetWindowAttrib(window_handler, GLFW_FOCUS_ON_SHOW, focus_on_show);
		WM_LOG_INFO_2("GLFW window focus on show state changed to " + std::to_string(focus_on_show));
	}

	bool wm_glfw_window::is_focused() const {
		return glfwGetWindowAttrib(window_handler, GLFW_FOCUSED);
	}

	void wm_glfw_window::focus() {
		glfwFocusWindow(window_handler);
		WM_LOG_INFO_2("GLFW window requested focus");
	}

	void wm_glfw_window::request_attention() {
		glfwRequestWindowAttention(window_handler);
		WM_LOG_INFO_2("GLFW window requested attention");
	}

	bool wm_glfw_window::is_hovered() const {
		return glfwGetWindowAttrib(window_handler, GLFW_HOVERED);
	}

	wm_glfw_window::~wm_glfw_window() {
		wm_glfw_window_system::get_instance()->remove_window(id);
		glfwDestroyWindow(window_handler);
		window_handler = nullptr;
		WM_LOG_INFO_1("GLFW window destructed");
	}
}
