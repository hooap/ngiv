#include "IScreen.h"
#include "Misc.h"

namespace ngiv{

	IScreen::IScreen(){
		
				
	}
	IScreen::~IScreen(){		
		disposeScreen();
	}

	void IScreen::initScreen(int width, int height, Window* window, std::string guifont, int guiscale){
		_width = width;
		_height = height;
		_window = window;		
						
		_gui.init(&_inputmanager, width, height, guifont, guiscale);

		init();
		initui();
		initialized = true;
	}

		
	bool IScreen::getInput(InputManager& i, Window* w){
			i.update();

			SDL_Event evnt;			
									
			while (SDL_PollEvent(&evnt)){
				switch (evnt.type)
				{
				case SDL_WINDOWEVENT:
					switch (evnt.window.event)
					{
					case SDL_WINDOWEVENT_FOCUS_GAINED:
						w->setState(Window_State::FocusOn);
						break;
					case SDL_WINDOWEVENT_FOCUS_LOST:
						w->setState(Window_State::FocusOff);
						break;
						
					default:
						break;
					}
				
					break;
				case SDL_QUIT:
					return true;
					break;
				case SDL_KEYDOWN:
					i.presskey(evnt.key.keysym.sym);
					break;
				case SDL_KEYUP:
					i.releasekey(evnt.key.keysym.sym);
					break;
				case SDL_MOUSEMOTION:
					i.setmousecoord(glm::vec2(evnt.motion.x, evnt.motion.y));
					break;
				case SDL_MOUSEBUTTONDOWN:
					i.presskey(evnt.button.button);
					break;
				case SDL_MOUSEBUTTONUP:
					i.releasekey(evnt.button.button);
					break;
				case SDL_MOUSEWHEEL:
					i.setmousewheel((float)evnt.wheel.y);
					break;				
				}
			}
			
			return false;

		}

	void IScreen::disposeScreen(){		
		if (initialized) {
			initialized = false;
			_gui.destroy();
			dispose();
		}

	}
	
	bool IScreen::internalupdate(){
		bool exit = getInput(_inputmanager, _window);
		_gui.update();

		if (exit) {
			return true; 
		}

		return false;
	}

	
}