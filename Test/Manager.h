#pragma once

#include "Screen.h"
#include <ngiv\IScreenManager.h>
#include <ngiv\Window.h>

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
	Screen _screen;
	ngiv::Window _window;


private:
	int width = 800;
	int height = 600;
	int fullscreen = 2;
};

