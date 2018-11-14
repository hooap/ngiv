#include "Manager.h"
#include <iostream>

#undef main
int main(int argc, char** argv) {
	std::cout << "Starting\n";

	Manager m;
	m.run();

	std::cout << "End of program\n";	
	return 0;
}