#pragma once
#include "IScreen.h"
#include <vector>

namespace ngiv {

	class IScreenManager
	{
	public:
		IScreenManager();
		~IScreenManager();

		void init(ngiv::Window* window, ColorRGBA8 backgroundColor) {
			_window = window;
			_backgroundColor = backgroundColor;
		}

		void addScreen(IScreen* screen) {
			_screens.push_back(screen);
			if (_screens.size() == 1) {
				active_screen = _screens[0];
			}
		}
		void setActiveScreen(IScreen* screenpointer) {
			static bool first = true;
			if (first) {
				active_screen = screenpointer;
				active_screen->onEntry();
				return;
			}
			active_screen->onExit();
			active_screen = screenpointer;
			active_screen->onEntry();
			return;
		}
		bool do_screen_loop();


	private:
		void quitlogic();


		ngiv::Window* _window;
		ColorRGBA8 _backgroundColor;
		std::vector<IScreen*> _screens;
		IScreen* active_screen = nullptr;

	};


}