#include <iostream>
#include <memory>

#include <windmill/windmill.h>

int main() {
	std::cout << "START" << std::endl;
	std::shared_ptr<wm::engine> e = wm::engine::create();
	e->run();
	std::cout << "STOP" << std::endl;
	glm::vec4 vector = glm::vec4(1.0);
	std::cout << "vector x: " << vector.x << ", y: " << vector.y << ", z: " << vector.z << std::endl;
}
