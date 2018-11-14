#include "InputManager.h"

#include "Misc.h"

namespace ngiv {
	InputManager::InputManager()
	{
		mousecoord = glm::vec2(0);				
		mouseoffset = glm::vec2(0);
	}
	InputManager::~InputManager()
	{
	}
	void InputManager::presskey(unsigned int keyID){
		keymap[keyID] = true;
	}
	void InputManager::releasekey(unsigned int keyID){
		keymap[keyID] = false;
	}
	bool InputManager::isKeyDown(unsigned int keyID){
		auto mit = keymap.find(keyID);
		if (mit != keymap.end())
		{
			return mit->second;
		}
		else {
			return false;
		}
	}
	
	bool InputManager::isKeyPressed(unsigned int keyID){
		bool isdown = isKeyDown(keyID);
		bool wasdown = waskeydown(keyID);
		if (isdown == true && wasdown == false){
			return true;
		}
		return false;
	}
	void InputManager::update(){
		pressed.clear();
		for (auto &it : keymap){
			if (it.second && !lastkeymap[it.first] && keymap[it.first]) {
				pressed.push_back(it.first);
			}
			lastkeymap[it.first] = it.second;			
		}
		mousewheel = 0.0f;		
		
	}

	bool InputManager::waskeydown(unsigned int keyID){
		auto mit = lastkeymap.find(keyID);
		if (mit != lastkeymap.end())
		{
			return mit->second;
		}
		else {
			return false;

		}

	}	

}