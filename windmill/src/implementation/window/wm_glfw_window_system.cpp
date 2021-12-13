#include "core/engine.h"
#include "window/event/gamepad_connection_event.h"
#include "defines/log_defines.h"

#include "wm_glfw_window_system.h"

namespace wm {

	ptr<wm_glfw_window_system> wm_glfw_window_system::get_instance() {
		static auto instance = ptr<wm_glfw_window_system>(new wm_glfw_window_system());
		if(!instance.is_valid()) {
			instance = ptr<wm_glfw_window_system>(new wm_glfw_window_system());
		}
		return instance;
	}

	wm_glfw_window_system::wm_glfw_window_system() {
	#ifdef WM_BUILD_DEBUG
		set_error_callback();
	#endif
		initialize_glfw();
		WM_LOG_INFO_1("GLFW window system constructed");
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
	}

	ptr<window> wm_glfw_window_system::create_window(const glm::ivec2& size, const std::string& title, const bool fullscreen, const bool visible) {
		const auto window = ptr<wm_glfw_window>(new wm_glfw_window(size, title, fullscreen, visible));
		windows.push_back(window);
		return window.convert<wm::window>();
	}

	int32_t wm_glfw_window_system::get_window_count() const {
		return static_cast<int32_t>(windows.size());
	}

	ptr<window> wm_glfw_window_system::get_window(const int32_t index) const {
		return windows.at(index).convert<window>();
	}

	void wm_glfw_window_system::destroy_window(ptr<window> window) {
		window.destroy();
	}

	void wm_glfw_window_system::remove_window(const GLFWwindow* id) {
		for(int32_t i = 0; i < windows.size(); i++) {
			if(id == windows.at(i)->get_handler()) {
				windows.erase(windows.begin() + i);
				return;
			}
		}
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

	void wm_glfw_window_system::update() {
		glfwPollEvents();
		for(int32_t i = get_window_count() - 1; i >= 0; i--) {
			auto window = windows.at(i);
			if(window->is_closing()) {
				window.destroy();
			} else {
				window->update();
			}
		}
		WM_LOG_INFO_3("GLFW window system updated");
	}

	wm_glfw_window_system::~wm_glfw_window_system() {
		for(auto window : windows) {
			window.destroy();
		}
		glfwTerminate();
		WM_LOG_INFO_1("GLFW window system destructed");
	}

}
