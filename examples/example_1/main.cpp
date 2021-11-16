#include <memory>
#include <string>
#include <functional>

#include <windmill/windmill.h>

class example_1_application: public wm::application {

	using wm::application::application;

	void add_log_system() override {
		const auto ls = wm::log_system::create();
		const auto clw = wm::log_writer::create_console_log_writer(wm::log_level::info_1);
		ls->add_log_writer(clw);
		wm::engine::set_system(wm::log_system::get_key(), ls);
	}

	template<class T>
	wm::ptr<wm::event_listener<T>> add_event_listener(const wm::key<T> key, const std::function<void(const T)> callback) {
		auto el = wm::ptr<wm::event_listener<T>>(new wm::event_listener<T>(callback));
		wm::engine::get_event_system()->add_event_listener<T>(key, el);
		return el;
	}

	void initialize() override {
		wm::application::initialize();

		add_event_listener<wm::mouse_button_event>(wm::mouse_button_event::get_key(), [](const wm::mouse_button_event event) {
			WM_LOG_DEBUG(event.get_button_name() + " " + std::to_string(event.is_press()));
		});

		WM_LOG_DEBUG("test log messages");
		const auto translationVector = glm::vec3(1000.0f, 200.0f, 3.0f);
		WM_LOG_DEBUG(wm::utility::to_string(glm::vec2(translationVector), "vec2:"));
		WM_LOG_DEBUG(wm::utility::to_string(translationVector, "translation vector:"));
		WM_LOG_DEBUG(wm::utility::to_string(glm::vec4(translationVector, 1.0f), "vec4:"));
		const auto translationMatrix = glm::translate(glm::mat4(1), translationVector);
		WM_LOG_DEBUG(wm::utility::to_string(translationMatrix, "translation matrix:"));
		WM_LOG_DEBUG(wm::utility::to_string(glm::quat(100, 10, 1, 0), "quaternion:"));
	}

};

int main() {
	example_1_application app("example 1");
	app.run();
}
