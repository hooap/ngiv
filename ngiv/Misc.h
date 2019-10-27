#pragma once
#include <conio.h>
#include <iostream>
#include <glm\glm.hpp>
#include <string>

namespace ngiv {

	inline void stop() {
		_getch();
	}
	inline void o(std::string t) {
		std::cout << t.c_str();
	}
	inline void o(glm::vec4 p) {
		std::cout << "|" << p.x << "|" << p.y << "|" << p.z << "|" << p.w <<  "|\n";
	}
	inline void o(glm::vec3 p) {
		std::cout << "|" << p.x << "|" << p.y << "|" << p.z << "|\n";
	}
	inline void o(glm::vec2 p) {
		std::cout << "|" << p.x << "|" << p.y << "|\n";
	}
	inline void o(int d) {
		std::cout << d << "\n";
	}
	inline void o(std::string t, glm::vec3 p) {
		std::cout << t << "		|" << p.x << "|" << p.y << "|" << p.z <<  "|\n";
	}


	inline void error(std::string error, bool critical) {
		std::cout << error << std::endl;
		if (!critical) {
			return;
		}
		throw(error.c_str());
	}

}