#pragma once
#include <GLAD/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <SDL/SDL.h>
#include "Window.h"


#define NGIV_VERSION "0.4d"

namespace ngiv {

	void init(bool printInfo) {
		static bool isalreadyinitialized = false;
		if (isalreadyinitialized == false) {
			SDL_Init(SDL_INIT_EVERYTHING);
			isalreadyinitialized = true;
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);


            SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3); //OpenGL 3+
            SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 3); //OpenGL 3.3
            SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );


		}

		if (printInfo) {
			std::cout << "NGIV VERSION:" << NGIV_VERSION << std::endl;
		}

	}
	void dispose() {

		SDL_Quit();


	}



}
