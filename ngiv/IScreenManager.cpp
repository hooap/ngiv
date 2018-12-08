#include "IScreenManager.h"

namespace ngiv {

	IScreenManager::IScreenManager()
	{
	}


	IScreenManager::~IScreenManager()
	{
	}

	bool IScreenManager::do_screen_loop() {

		bool exit = active_screen->internalupdate();
		if (exit) {
			quitlogic();
			return true;
		}

		exit = active_screen->update(0);
		if (exit) {
			quitlogic();
			return true;
		}
		



		active_screen->draw();
		active_screen->render();

		_window->swapWindow();
		return false;
	}

	void IScreenManager::quitlogic() {
		for (int i = 0; i < _screens.size(); i++) {
			_screens[i]->disposeScreen();
		}
		_screens.clear();

	}

}