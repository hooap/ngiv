#pragma once
#include <GLAD\glad.h>
#include <string>
#include <glm\glm.hpp>
#include <SDL\SDL.h>
#include "Window.h"

#define NGIV_VERSION "0.2"

namespace ngiv {
	
	void init(bool printInfo) {
		static bool isalreadyinitialized = false;
		if (isalreadyinitialized == false) {
			SDL_Init(SDL_INIT_EVERYTHING);
			isalreadyinitialized = true;
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		}

		if (printInfo) {
			std::cout << "NGIV VERSION:" << NGIV_VERSION << std::endl;			
		}

	}
	void dispose() {

		SDL_Quit();


	}

	

}