#include <iostream>
#include "windmill/windmill.h"

int main() {
	std::cout << "START" << std::endl;
	std::shared_ptr<wm::engine> e = wm::engine::create();
	e->run();
	std::cout << "STOP" << std::endl;
}