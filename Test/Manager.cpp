#include "Manager.h"

#include <ngiv/NGIV.h>
#include <ngiv/BasicLevelEditor.h>

Manager::Manager()
{
}


Manager::~Manager()
{
}


void Manager::init() {
	ngiv::init(true);

	Uint32 flags = 6;
	
	if (fullscreen) {
		flags += 4097;
	}
	_window.create("Tech Demo", width, height, flags, _vsync);

	Screen *_screen = new Screen();

	_screenManager.init(&_window, ngiv::ColorRGBA8(255, 255, 255, 255));
	//create screen

	ngiv::BasicLevelEditor* _leveleditor = new ngiv::BasicLevelEditor();


	_leveleditor->initScreen(width,height,&_window, "Fonts//calibril.ttf", 32);

	_screen->initScreen(width,height,&_window,"Fonts//calibril.ttf",64);
	_screen->setscreenPointer("leveleditor", _leveleditor);



	_leveleditor->setContainer(_screen->getContainer());
	_leveleditor->setCam(_screen->getCam());
	_leveleditor->setscreenPointer("game", _screen);
	_leveleditor->lateinit();


	_screenManager.addScreen(_screen);
	_screenManager.addScreen(_leveleditor);
	_screenManager.setActiveScreen(0);
}
void Manager::run() {

	init();

	while (true) {

		bool exit = _screenManager.do_screen_loop();

		if (exit) {
			dispose();
			return;
		}
	}

}
void Manager::dispose() {
	_window.dispose();
	ngiv::dispose();
}
