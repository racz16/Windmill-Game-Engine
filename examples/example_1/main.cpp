#include <memory>
#include <string>

#include <windmill/windmill.h>

int main() {
	WM_LOG_INFO_1("application started");
	auto engine = wm::engine::create();
	engine->initialize();
	engine->run();
	auto translationVector = glm::vec3(1.0f, 2.0f, 3.0f);
	WM_LOG_DEBUG(wm::to_string(glm::vec2(translationVector), "vec2:"));
	WM_LOG_DEBUG(wm::to_string(translationVector, "translation vector:"));
	WM_LOG_DEBUG(wm::to_string(glm::vec4(translationVector, 1.0f), "vec4:"));
	auto translationMatrix = glm::translate(glm::mat4(1), translationVector);
	WM_LOG_DEBUG(wm::to_string(translationMatrix, "translation matrix:"));
	WM_LOG_INFO_1("application stopped");
}
