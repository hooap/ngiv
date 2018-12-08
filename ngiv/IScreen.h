#pragma once

//#define NOMINMAX


#include "GLSLProgram.h"
#include "InputManager.h"
#include "NGUI.h"
//#include "DebugRenderer.h"
#include "Window.h"

namespace ngiv{

	class IScreen
	{
	public:
		IScreen();
		~IScreen();

		void initScreen(int width, int height, Window* window, std::string guifont, int guiscale);
		void disposeScreen();
		bool internalupdate();
		

		virtual void onEntry() = 0;		
		virtual void onExit() = 0;
		virtual bool update(float deltatime) = 0;
		virtual void draw() = 0;
		virtual void render() = 0;
		
		


		IScreen* switchscreen = nullptr;

		virtual void setscreenPointera(IScreen* a) {	}
		virtual void setscreenPointerb(IScreen* b) {	}


		virtual void initui() = 0;
		virtual void init() = 0;
		virtual void dispose() = 0;
		
	protected:
		bool initialized = false;
		static bool getInput(InputManager& i, Window* w);	
			
		InputManager* getinputmanager() { return &_inputmanager; }		
		
	protected:			
		
		int _width, _height;
		
								
		InputManager _inputmanager;								
				
		NGUI _gui;
		Window* _window;
	};

}