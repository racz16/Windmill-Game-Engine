#include <memory>
#include <string>

#include <windmill/windmill.h>

class example_1_application: public wm::application {

	using wm::application::application;

	void add_log_system() override {
		const auto ls = wm::log_system::create();
		const auto clw = wm::log_writer::create_console_log_writer(wm::log_level::Info_1);
		ls->add_log_writer(clw);
		wm::engine::set_system(wm::log_system::get_key(), ls);
	}

	void initialize() override {
		wm::application::initialize();

		WM_LOG_DEBUG("test log messages");
		const auto translationVector = glm::vec3(1.0f, 2.0f, 3.0f);
		WM_LOG_DEBUG(wm::utility::to_string(glm::vec2(translationVector), "vec2:"));
		WM_LOG_DEBUG(wm::utility::to_string(translationVector, "translation vector:"));
		WM_LOG_DEBUG(wm::utility::to_string(glm::vec4(translationVector, 1.0f), "vec4:"));
		const auto translationMatrix = glm::translate(glm::mat4(1), translationVector);
		WM_LOG_DEBUG(wm::utility::to_string(translationMatrix, "translation matrix:"));
		WM_LOG_DEBUG(wm::utility::to_string(glm::quat(), "quaternion:"));
	}

};

int main() {
	example_1_application app("example 1");
	app.run();
}
