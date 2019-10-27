#pragma once
#include <SDL/SDL.h>
#include <string>
#include <GLAD\glad.h>


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
	
	enum class Window_State{FocusOn,FocusOff};
	
	class Window 
	{
	public:
		enum class WindowType {FULLSCREEN,WINDOWED};

		Window();
		~Window();
		
		bool create(const std::string tabname, int& width, int& height, Uint32 comingflags , bool is_vsync_on);
		void dispose();
				
		void setFullscreen();
		void setWindowed();

		void setStateWindowed();
		void setStateFullscreen();

		void getwindowsize(int &width, int &height);
		WindowType getwindowtype() { return _windowtype; }

		SDL_Window* getwindow() { return _window; }

		int getWidth() { return _width; }
		int getHeight() { return _height; }

		void setState(Window_State s) {	_s = s;	}
		Window_State getState() { return _s; }

		void swapWindow();

	private:
		Window_State _s = Window_State::FocusOn;
		SDL_Window* _window = nullptr;
		int _baslangicwidth;
		int _baslangicheight;
		int _width;
		int _height;
		WindowType _windowtype;
	};

}