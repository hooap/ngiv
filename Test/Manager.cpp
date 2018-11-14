#include "Manager.h"

#include <ngiv\NGIV.h>

Manager::Manager()
{
}


Manager::~Manager()
{
}


void Manager::init() {
	ngiv::init(true);

	Uint32 flags = 6;
	if (fullscreen == 1) {
		flags += 1;
	}
	else if (fullscreen == 2) {
		flags += 4097;
	}
	_window.create("Tech Demo", width, height, flags, false);

	_screenManager.init(&_window, ngiv::ColorRGBA8(255, 255, 255, 255));
	//create screen
	_screen.initScreen(width,height,&_window,"Fonts//calibril.ttf",64);	

	_screenManager.addScreen(&_screen);
	_screenManager.setActiveScreen(&_screen);
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