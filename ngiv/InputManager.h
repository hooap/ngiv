#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <vector>
#include "Window.h"
#include "Misc.h"

namespace ngiv {
	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void update();
		void presskey(unsigned int keyID);
		void releasekey(unsigned int keyID);
		bool isKeyDown(unsigned int keyID);
		bool isKeyPressed(unsigned int keyID);

		glm::vec2 getMouseCoord() { return mousecoord; }
		//glm::vec2 getMouseOffset() { return mouseoffset; }

		//void setmouseoffset(glm::vec2 offset) { mouseoffset = offset; }
		void setmousecoord(glm::vec2 pos) { mousecoord = pos; }
		void setmousewheel(float mwheel) { mousewheel = mwheel; }

		void putMouseMiddleofScreen(ngiv::Window* win) {

			glm::vec2 p = glm::vec2(win->getWidth() / 2, win->getHeight() / 2);
			SDL_WarpMouseInWindow(win->getwindow(), (int)p.x, (int)p.y);
		}

		float getMouseWheel() { return mousewheel; }
		char getPressedKeys() {
			if (pressed.size() != 0) { return (char)pressed[0]; }
			else { return 0; }

		}

	private:
		bool waskeydown(unsigned int keyID);
		std::unordered_map<unsigned int, bool> keymap;
		std::unordered_map<unsigned int, bool> lastkeymap;

		glm::vec2 mousecoord;
		glm::vec2 mouseoffset;

		bool firsttime = true;
		float mousewheel;
		std::vector<unsigned int> pressed;
	};

}
