#pragma once


#include <iostream>
#include <glm/glm.hpp>
#include <string>



#ifdef _WIN32
#include <conio.h>
#else

#include <unistd.h>
#include <termios.h>


inline char _getch(void)
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if(tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if(tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if(read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if(tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    printf("%c\n", buf);
    return buf;
 }


#endif // WIN32


namespace ngiv {


#if defined(_MSC_VER)
#define ALIGNED_(x) __declspec(align(x))
#else
#if defined(__GNUC__)
#define ALIGNED_(x) __attribute__ ((aligned(x)))
#endif
#endif

#define ALIGNED_TYPE_(t,x) typedef t ALIGNED_(x)


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
