#pragma once

//#define NOMINMAX

#include "GLSLProgram.h"
#include "InputManager.h"
#include "NGUI.h"
#include "Window.h"
#include "Misc.h"

#include <map>

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
		
		



		virtual void setscreenPointer(std::string name,IScreen* a) {
			screenpointers.insert(std::make_pair(name, a));
		}
		


		IScreen* switchscreen = nullptr;

	protected:
		virtual void initui() = 0;
		virtual void init() = 0;
		virtual void dispose() = 0;



		bool initialized = false;
		static bool getInput(InputManager& i, Window* w);	
		
		void setSwitchScreen(std::string name) {
			auto it = screenpointers.find(name);
			if (it != screenpointers.end()) {
				switchscreen = it->second;
			}
			else {
				ngiv::error("switchscreen:" + name + "not found", false);
			}
		}

		InputManager* getinputmanager() { return &_inputmanager; }		
		
	protected:	
		std::map<std::string, IScreen*> screenpointers;


		int _width, _height;
		
								
		InputManager _inputmanager;								
				
		NGUI _gui;
		Window* _window;
	};

}