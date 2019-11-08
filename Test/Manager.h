#pragma once

#include "Screen.h"
#include <ngiv/IScreenManager.h>
#include <ngiv/Window.h>

class Manager
{
public:
	Manager();
	~Manager();

	void run();
private:
	void init();
	void dispose();
	ngiv::IScreenManager _screenManager;
	ngiv::Window _window;


private:
	int width = 1024;
	int height = 768;
	bool fullscreen = true;
	bool _vsync = false;
};

