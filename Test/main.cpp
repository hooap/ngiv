#include "Manager.h"

#include <iostream>

#undef main
int main(int argc, char** argv) {

    std::cout << "hello";

	Manager m;
	m.run();
	return 0;
}
