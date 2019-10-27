#include "Window.h"
#include "Misc.h"
#include <iostream>

namespace ngiv {

	/*
	SDL_WINDOW_FULLSCREEN; = 1
	SDL_WINDOW_OPENGL; = 2
	SDL_WINDOW_SHOWN; = 4
	SDL_WINDOW_HIDDEN; = 8
	SDL_WINDOW_BORDERLESS; = 16
	SDL_WINDOW_RESIZABLE; = 32
	SDL_WINDOW_FULLSCREEN_DESKTOP = 4097
	*/
	

	Window::Window()
	{
	}


	Window::~Window()
	{
		
	}

	bool Window::create(const std::string tabname, int& width, int& height, Uint32 comingflags, bool is_vsync_on)
	{
		
		
		if ((int)comingflags - 6 > 0) {
			_windowtype = WindowType::FULLSCREEN;
		}
		else {
			_windowtype = WindowType::WINDOWED;
		}
		if (_window == nullptr){
			_window = SDL_CreateWindow(tabname.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, comingflags);

			if (_window == nullptr)
			{
				error("SDL Window olusturulamadi", true); 1;
			}


			

			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

			SDL_GLContext glcontext = SDL_GL_CreateContext(_window);
			//SDL_GLContext glcontext = SDL_GL_CreateContext(_window);
			
			

			
			if (glcontext == nullptr)
			{
				error("GLContext olusturulamadi", true); 1;
			}

			if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
			{
				error("GLContext olusturulamadi", true);
			}
			getwindowsize(width, height);			
			_width = width;
			_height = height;
			glViewport(0, 0, _width, _height);

			std::cout << "OpenGL Version:" << glGetString(GL_VERSION) << std::endl;
			
			//VSYNC
			SDL_GL_SetSwapInterval(is_vsync_on);

			//transparancy
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // NORMALÝ BU // en üstteki rengi gösterir
			// glBlendFunc(GL_SRC_ALPHA, GL_ONE); // bu ise renkleri toplar
		}

		return 0;
	 }
	
	void Window::dispose(){
		
		SDL_GL_DeleteContext(SDL_GL_GetCurrentContext());
		SDL_DestroyWindow(_window);
		
	}

	void Window::swapWindow()
	{
		SDL_GL_SwapWindow(_window);
	}
	
	void Window::setFullscreen(){
		SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		_windowtype = WindowType::FULLSCREEN;
	}
	void Window::setWindowed(){
		SDL_SetWindowFullscreen(_window, 0);
		_windowtype = WindowType::WINDOWED;
	}

	void Window::getwindowsize(int &width, int &height){
		SDL_GetWindowSize(_window, &width, &height);
	}

	void Window::setStateWindowed(){
		_windowtype = WindowType::WINDOWED;
	}
	void Window::setStateFullscreen(){
		_windowtype = WindowType::FULLSCREEN;
	}


}