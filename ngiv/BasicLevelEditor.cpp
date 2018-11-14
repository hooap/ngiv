#include "Screen_Level_Editor.h"
#include <NGI\ResourceManager.h>
#include "Renderer.h"
#include <NGI\StopFunction.h>
#include <NGI\IOManager.h>
#include "SaveLoad.h"

void Screen_Level_Editor::init() {
	_camera.setposition(glm::vec2(0));
	
}
void Screen_Level_Editor::initui() {
	//screen buttons
	{
		//return button
		ngi::GLTexture returntexture = ngi::ResourceManager::getTexture("Textures/return.png");
		_gui.addbutton("", 1, glm::vec4(0, 95, 5, 5), ngi::ColorRGBA8(255, 255, 255, 255), ngi::ColorRGBA8(255, 0, 0, 255), std::bind(&Screen_Level_Editor::checkmenubutton, this, std::placeholders::_1), returntexture.id);
		//

	}
				
	//left panel
	{
		//panel
		ngi::NGUI_PANEL* lpanel = _gui.addpanel(glm::vec4(0.5, 0.5, 20, 94), ngi::ColorRGBA8(100, 100, 100, 255));

		//Free button	
		ngi::NGUI_PANEL_BUTTON* b1 = _gui.getpanelbutton("Free", 1.0f, glm::vec4(5, 80, 42, 15), ngi::ColorRGBA8(130, 150, 130, 255), ngi::ColorRGBA8(255, 255, 255, 255), std::bind(&Screen_Level_Editor::checkfreebutton, this, std::placeholders::_1, std::placeholders::_2));
		lpanel->buttons.push_back(b1);

		//Move button	
		ngi::NGUI_PANEL_BUTTON* b2 = _gui.getpanelbutton("Move", 1.0f, glm::vec4(53, 80, 42, 15), ngi::ColorRGBA8(130, 150, 130, 255), ngi::ColorRGBA8(255, 255, 255, 255), std::bind(&Screen_Level_Editor::checkselectbutton, this, std::placeholders::_1, std::placeholders::_2));
		lpanel->buttons.push_back(b2);

		//create box	
		ngi::NGUI_PANEL_BUTTON* b3 = _gui.getpanelbutton("Box", 1.0f, glm::vec4(5, 60, 90, 15), ngi::ColorRGBA8(130, 150, 130, 255), ngi::ColorRGBA8(255, 255, 255, 255), std::bind(&Screen_Level_Editor::checkcreateboxbutton, this, std::placeholders::_1, std::placeholders::_2));
		lpanel->buttons.push_back(b3);

		//create spawnpoint
		ngi::NGUI_PANEL_BUTTON* b4 = _gui.getpanelbutton("SpawnPoint", 1.0f, glm::vec4(5, 40, 90, 15), ngi::ColorRGBA8(130, 150, 130, 255), ngi::ColorRGBA8(255, 255, 255, 255), std::bind(&Screen_Level_Editor::checkcreatespawnpointbutton, this, std::placeholders::_1, std::placeholders::_2));
		lpanel->buttons.push_back(b4);
		
		//cam reset
		ngi::NGUI_PANEL_BUTTON* b5 = _gui.getpanelbutton("CamReset", 1.0f, glm::vec4(4, 26, 42, 10), ngi::ColorRGBA8(130, 150, 130, 255), ngi::ColorRGBA8(255, 255, 255, 255), std::bind(&Screen_Level_Editor::checkcamresetbutton, this, std::placeholders::_1, std::placeholders::_2));
		lpanel->buttons.push_back(b5);

		//debug checkbox
		_debugcheckbox = _gui.getpanelcheckbox("debugcheckbox", glm::vec4(1, 0.4, 10, 3), ngi::ColorRGBA8(70, 70, 70, 255), ngi::ColorRGBA8(0, 255, 0, 255), true, std::bind(&Screen_Level_Editor::checkdebugcheckbox, this, std::placeholders::_1, std::placeholders::_2));
		lpanel->checkboxs.push_back(_debugcheckbox);
		//debug checkbox text
		ngi::NGUI_TEXT* t1 = _gui.gettext("debug", glm::vec2(25, 1.6), ngi::ColorRGBA8(0, 0, 255, 255), 0.5f, true, true);
		lpanel->texts.push_back(t1);

		//forced grid checkbox
		_forcedgridcheckbox = _gui.getpanelcheckbox("forcedgridcheckbox", glm::vec4(1, 5, 10, 3), ngi::ColorRGBA8(70, 70, 70, 255), ngi::ColorRGBA8(0, 255, 0, 255), true, std::bind(&Screen_Level_Editor::checkforcedgridcheckbox, this, std::placeholders::_1, std::placeholders::_2));
		lpanel->checkboxs.push_back(_forcedgridcheckbox);
		//forced grid text
		ngi::NGUI_TEXT* t2 = _gui.gettext("forcedgrid", glm::vec2(25, 6.2), ngi::ColorRGBA8(0, 0, 255, 255), 0.5f, true, true);
		lpanel->texts.push_back(t2);

		//clearbutton
		ngi::NGUI_PANEL_BUTTON* bclear = _gui.getpanelbutton("Clear", 1.0f, glm::vec4(54, 26, 42, 10), ngi::ColorRGBA8(125, 125, 125, 255), ngi::ColorRGBA8(255, 255, 255, 255), std::bind(&Screen_Level_Editor::checkclearbutton, this, std::placeholders::_1, std::placeholders::_2));
		lpanel->buttons.push_back(bclear);
		
		//savepanelopenbutton
		ngi::NGUI_PANEL_BUTTON* bsave = _gui.getpanelbutton("Save", 1.0f, glm::vec4(54, 2, 42, 10), ngi::ColorRGBA8(125, 125, 125, 255), ngi::ColorRGBA8(0, 255, 0, 255), std::bind(&Screen_Level_Editor::checkpanelsavebutton, this, std::placeholders::_1, std::placeholders::_2));
		lpanel->buttons.push_back(bsave);

		//loadpanelopenbutton
		ngi::NGUI_PANEL_BUTTON* bload = _gui.getpanelbutton("Load", 1.0f, glm::vec4(54, 14, 42, 10), ngi::ColorRGBA8(125, 125, 125, 255), ngi::ColorRGBA8(0, 255, 0, 255), std::bind(&Screen_Level_Editor::checkpanelloadbutton, this, std::placeholders::_1, std::placeholders::_2));
		lpanel->buttons.push_back(bload);
				

	}

	//save panel
	{
		_savepanel = _gui.addpanel(glm::vec4(30, 30, 40, 40), ngi::ColorRGBA8(200, 200, 200, 255),true,ngi::ColorRGBA8(120,120,120,255),true);
		_savepanel->active = false;

		ngi::NGUI_PANEL_EDITBOX* e1 = _gui.getpaneleditbox("savefilename", glm::vec4(5, 50, 85, 15), 0.8f, ngi::ColorRGBA8(100, 100, 100, 255), ngi::ColorRGBA8(0, 0, 255, 255), "");
		_savepanel->editboxs.push_back(e1);

		//save button
		ngi::NGUI_PANEL_BUTTON* pb1 = _gui.getpanelbutton("Save", 0.7, glm::vec4(85, 5, 10, 10), ngi::ColorRGBA8(100, 100, 100, 255), ngi::ColorRGBA8(255, 0, 0, 255), std::bind(&Screen_Level_Editor::checksaveactionbutton, this, std::placeholders::_1, std::placeholders::_2));
		_savepanel->buttons.push_back(pb1);
	}
	
	//load panel
	{
		_loadpanel = _gui.addpanel(glm::vec4(30, 30, 40, 40), ngi::ColorRGBA8(200, 200, 200, 255), true, ngi::ColorRGBA8(120, 120, 120, 255), true);
		_loadpanel->active = false;

		ngi::NGUI_PANEL_EDITBOX* e1 = _gui.getpaneleditbox("loadfilename", glm::vec4(5, 50, 85, 15), 0.8f, ngi::ColorRGBA8(100, 100, 100, 255), ngi::ColorRGBA8(0, 0, 255, 255), "");
		_loadpanel->editboxs.push_back(e1);

		//save button
		ngi::NGUI_PANEL_BUTTON* pb1 = _gui.getpanelbutton("Load", 0.7, glm::vec4(85, 5, 10, 10), ngi::ColorRGBA8(100, 100, 100, 255), ngi::ColorRGBA8(255, 0, 0, 255), std::bind(&Screen_Level_Editor::checkloadactionbutton, this, std::placeholders::_1, std::placeholders::_2));
		_loadpanel->buttons.push_back(pb1);
	}

	//resetpanel	
	_resetpanel = _gui.addPREMADEyesno(glm::vec4(30, 30, 20, 20), "Are you sure", std::bind(&Screen_Level_Editor::checkresetpageyes, this, std::placeholders::_1, std::placeholders::_2));
		
	//loadeven
	_loadeven = _gui.addPREMADEyesno(glm::vec4(30, 30, 20, 20), "Are you sure", std::bind(&Screen_Level_Editor::checkloadevenyes, this, std::placeholders::_1, std::placeholders::_2));
	
	
}
void Screen_Level_Editor::onEntry() {
}
void Screen_Level_Editor::onExit() {

}
bool Screen_Level_Editor::update(float deltatime) {
	if (getInput()) return true;
	
	_uicamera.update();
	_camera.update();
	_gui.update();	
	
	if (!_gui.shouldcamerastop()) {
		//resize
		if (_inputmanager.getMouseWheel() > 0) {
			float f = _camera.getscale();
			_camera.setscale(f += 0.1f);
		}
		if (_inputmanager.getMouseWheel() < 0) {
			float f = _camera.getscale();
			_camera.setscale(f -= 0.1f);
		}

		if (_inputmanager.isKeyDown(SDLK_w)) {
			_camera.movetoPos(glm::vec2(0, 2));
		}
		if (_inputmanager.isKeyDown(SDLK_s)) {
			_camera.movetoPos(glm::vec2(0, -2));
		}
		if (_inputmanager.isKeyDown(SDLK_a)) {
			_camera.movetoPos(glm::vec2(-2, 0));
		}
		if (_inputmanager.isKeyDown(SDLK_d)) {
			_camera.movetoPos(glm::vec2(2, 0));
		}
	}
	
	//check for delete
	if (_inputmanager.isKeyDown(SDL_BUTTON_RIGHT)) {
		for (int i = 0; i < _con_box.size(); i++){
			glm::vec2 p = _con_box[i]->get_pos();
			glm::vec2 d = _con_box[i]->get_dims();
			if (_mousepos.x > p.x) {
				if (_mousepos.x < p.x + d.x) {
					if (_mousepos.y > p.y) {
						if (_mousepos.y < p.y + d.y) {
							delete _con_box[i];
							_con_box[i] = _con_box.back();
							_con_box.pop_back();
							
						}
					}
				}
			}
		}
		for (int i = 0; i < _con_spawnpoint.size(); i++) {
			glm::vec2 p = _con_spawnpoint[i]->get_pos();
			float r = _con_spawnpoint[i]->get_radius();
			if (_mousepos.x > p.x) {
				if (_mousepos.x < p.x + r) {
					if (_mousepos.y > p.y) {
						if (_mousepos.y < p.y + r) {
							delete _con_spawnpoint[i];
							_con_spawnpoint[i] = _con_spawnpoint.back();
							_con_spawnpoint.pop_back();
						}
					}
				}
			}

		}

	}
	
	switch (_tool) {
	case Tool::SelectedBox:
		//update pos
		if (_forcedgridcheckbox->value) {
			glm::vec2 destrect = glm::vec2(_mousepos / ((Box*)_selected)->get_dims());
			if(destrect.x < 0) destrect.x--;
			if(destrect.y < 0) destrect.y--;
			destrect.x = (int)destrect.x;
			destrect.y = (int)destrect.y;
						
			((Box*)_selected)->set_pos(destrect * ((Box*)_selected)->get_dims());
		}
		else {
			((Box*)_selected)->set_pos(_mousepos);
		}
		//cancel
		if (_inputmanager.isKeyPressed(SDLK_ESCAPE)) {
			delete _selected;
			_selected = nullptr;
			_tool = Tool::Free;			
		};		
		//drop
		if (_inputmanager.isKeyDown(SDL_BUTTON_LEFT)) {
			_con_box.push_back(((Box*)_selected));
			_selected = nullptr;
			_tool = Tool::Free;
		}
		//rotate
		{
			const float toradian = 0.0174532925;
			if (_inputmanager.isKeyDown(SDLK_q)) {
				((Box*)_selected)->set_angle(((Box*)_selected)->get_angle() + 0.05f);
			}
			if (_inputmanager.isKeyDown(SDLK_e)) {
				((Box*)_selected)->set_angle(((Box*)_selected)->get_angle() - 0.05f);
			}

			if (_inputmanager.isKeyPressed(SDLK_KP_6)) {
				((Box*)_selected)->set_angle(0);
			}
			if (_inputmanager.isKeyPressed(SDLK_KP_8)) {
				((Box*)_selected)->set_angle(90 * toradian);
			}
			if (_inputmanager.isKeyPressed(SDLK_KP_4)) {
				((Box*)_selected)->set_angle(180 * toradian);
			}
			if (_inputmanager.isKeyPressed(SDLK_KP_2)) {
				((Box*)_selected)->set_angle(270 * toradian);
			}
		}
		//resize
		{
			if (_inputmanager.isKeyDown(SDLK_f)) {
				((Box*)_selected)->set_dims(((Box*)_selected)->get_dims() - glm::vec2(1.0f, 0.0f));
			}
			if (_inputmanager.isKeyDown(SDLK_g)) {
				((Box*)_selected)->set_dims(((Box*)_selected)->get_dims() + glm::vec2(1.0f, 0.0f));
			}
			if (_inputmanager.isKeyDown(SDLK_h)) {
				((Box*)_selected)->set_dims(((Box*)_selected)->get_dims() - glm::vec2(0.0f, 1.0f));
			}
			if (_inputmanager.isKeyDown(SDLK_y)) {
				((Box*)_selected)->set_dims(((Box*)_selected)->get_dims() + glm::vec2(0.0f, 1.0f));
			}			
		}
		break;
	case Tool::SelectedDot:
		//update pos
		if (_forcedgridcheckbox->value) {
			glm::vec2 destrect = glm::vec2(_mousepos / (((Dot*)_selected)->get_radius()));
			if (destrect.x < 0) destrect.x--;
			if (destrect.y < 0) destrect.y--;
			destrect.x = (int)destrect.x;
			destrect.y = (int)destrect.y;
			glm::vec2 pos = glm::vec2(destrect * ((Dot*)_selected)->get_radius());
			pos += ((Dot*)_selected)->get_radius();

			((Dot*)_selected)->set_pos(pos);
		}
		else {
			((Dot*)_selected)->set_pos(_mousepos);
		}
		//cancel
		if (_inputmanager.isKeyPressed(SDLK_ESCAPE)) {
			delete _selected;
			_selected = nullptr;
			_tool = Tool::Free;
		};
		//drop
		if (_inputmanager.isKeyDown(SDL_BUTTON_LEFT)) {
			switch (((Dot*)_selected)->get_type()) {
			case 0:
				_con_spawnpoint.push_back(((Dot*)_selected));
				break;
			default:
				break;
			}						
			_selected = nullptr;
			_tool = Tool::Free;
		}
		break;
	case Tool::MoveObject:
		
		if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
			_inputmanager.releasekey(SDL_BUTTON_LEFT);
			
			for (int i = 0; i < _con_spawnpoint.size(); i++) {
				glm::vec2 p = _con_spawnpoint[i]->get_pos();
				float r = _con_spawnpoint[i]->get_radius();
				if (_mousepos.x > p.x) {
					if (_mousepos.x < p.x + r) {
						if (_mousepos.y > p.y) {
							if (_mousepos.y < p.y + r) {
								//found 
								_tool = Tool::SelectedDot;
								_selected = _con_spawnpoint[i];
								_con_spawnpoint[i] = _con_spawnpoint.back();
								_con_spawnpoint.pop_back();
								break;
							}
						}
					}
				}

			}

			for (int i = 0; i < _con_box.size(); i++) {
				glm::vec2 p = _con_box[i]->get_pos();
				glm::vec2 d = _con_box[i]->get_dims();
				if (_mousepos.x > p.x) {
					if (_mousepos.x < p.x + d.x) {
						if (_mousepos.y > p.y) {
							if (_mousepos.y < p.y + d.y) {
								//found 
								_tool = Tool::SelectedBox;
								_selected = _con_box[i];
								_con_box[i] = _con_box.back();
								_con_box.pop_back();
								break;
							}
						}
					}
				}
			}
			

		};
		
	default:
		break;
	}
	
	// true if player wants to exit
	return false;
}
void Screen_Level_Editor::draw() {
	

	if (_selected != nullptr) {
		
		switch (_tool)
		{
		case Tool::SelectedBox:
			_debugrenderer.drawbox(glm::vec4(((Box*)_selected)->get_pos(), ((Box*)_selected)->get_dims()), ngi::ColorRGBA8(0, 255, 0, 255), ((Box*)_selected)->get_angle());
			break;
		case Tool::SelectedDot:
			_debugrenderer.drawCircle(((Dot*)_selected)->get_pos(), ngi::ColorRGBA8(0,100,255,255), ((Dot*)_selected)->get_radius());
		default:
			break;
		}
	
	}

	if (_debugcheckbox->value) {
		_debugrenderer.drawbox(glm::vec4(0,-10000,0.1f,20000), ngi::ColorRGBA8(0, 255, 0, 255), 0);
		_debugrenderer.drawbox(glm::vec4(-10000, 0, 20000, 0.1f), ngi::ColorRGBA8(255, 0, 0, 255), 0);

		//render boxes with debug lines
		for (int i = 0; i < _con_box.size(); i++) {					
			Renderer::render_box_normal(_spritebatch, _con_box[i]);
			_debugrenderer.drawbox(glm::vec4(_con_box[i]->get_pos(), _con_box[i]->get_dims()), ngi::ColorRGBA8(0, 255, 0, 255), _con_box[i]->get_angle());
		}

		//render dots 
		for (int i = 0; i < _con_spawnpoint.size(); i++) {			
			Renderer::render_dot_normal(_spritebatch, _con_spawnpoint[i]);			
		}

	}

	else {


		for (int i = 0; i < _con_box.size(); i++) {
			Renderer::render_box_normal(_spritebatch, _con_box[i]);
		}

		//render dots 
		for (int i = 0; i < _con_spawnpoint.size(); i++) {
			Renderer::render_dot_normal(_spritebatch, _con_spawnpoint[i]);
		}

	}

	
	
	
}
void Screen_Level_Editor::save() {
	int r = g_save("Saves\\" + path, _con_box, _con_spawnpoint);
	
}
void Screen_Level_Editor::load() {
	int r = g_load("Saves\\" + path, _con_box, _con_spawnpoint);
	switch (r)
	{
	case 0:
		break;
	case 1:
		_gui.addPREMADEok(glm::vec4(30, 40, 30, 30), "Corrupted File");
		break;
	case 2:
		_gui.addPREMADEok(glm::vec4(30, 40, 30, 30), "Version Mismatch");
		break;
	case 3:
		_gui.addPREMADEok(glm::vec4(30, 40, 30, 30), "File Reading Error");
		break;
	}

}

//main
void Screen_Level_Editor::checkmenubutton(ngi::NGUI_BUTTON* b) {

	if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
		switchscreen = _menupointer;
		_inputmanager.releasekey(SDL_BUTTON_LEFT);

	}

}

//left panel
void Screen_Level_Editor::checkcreateboxbutton(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p) {

	if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {

		if (_selected != nullptr) {
			delete _selected;
		}

		_selected = new Box();
		((Box*)_selected)->createbox(_mousepos, glm::vec2(50, 50), 0, "Textures/Box.png", ngi::ColorRGBA8(255, 0, 0, 255));
		_inputmanager.releasekey(SDL_BUTTON_LEFT);
		_tool = Tool::SelectedBox;

	}

}
void Screen_Level_Editor::checkselectbutton(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p) {

	if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {

		if (_selected != nullptr) {
			delete _selected;
		}
		_tool = Tool::MoveObject;
		_inputmanager.releasekey(SDL_BUTTON_LEFT);
	}


}
void Screen_Level_Editor::checkfreebutton(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p) {
	if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {

		if (_selected != nullptr) {
			delete _selected;
		}
		_tool = Tool::Free;
		_inputmanager.releasekey(SDL_BUTTON_LEFT);
	}
}
void Screen_Level_Editor::checkcreatespawnpointbutton(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p) {

	if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {

		if (_selected != nullptr) {
			delete _selected;
		}

		_selected = new Dot();
		((Dot*)_selected)->createDot(_mousepos, ngi::ColorRGBA8(0, 0, 255, 255), "Textures/Ball.png", 10.0f, 0);
		_inputmanager.releasekey(SDL_BUTTON_LEFT);
		_tool = Tool::SelectedDot;

	}

}
void Screen_Level_Editor::checkclearbutton(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p) {

	if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
		_resetpanel->active = true;
	}

}
void Screen_Level_Editor::checkdebugcheckbox(ngi::NGUI_PANEL_CHECKBOX* b, ngi::NGUI_PANEL* p) {

	if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
		if (b->value) { b->value = false; }
		else { b->value = true; }
		_inputmanager.releasekey(SDL_BUTTON_LEFT);
	}


}
void Screen_Level_Editor::checkforcedgridcheckbox(ngi::NGUI_PANEL_CHECKBOX* b, ngi::NGUI_PANEL* p) {
	if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
		if (b->value) { b->value = false; }
		else { b->value = true; }
		_inputmanager.releasekey(SDL_BUTTON_LEFT);

	}
}
void Screen_Level_Editor::checkpanelsavebutton(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p) {
	if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
		_inputmanager.releasekey(SDL_BUTTON_LEFT);
		_savepanel->active = 1;

	}
}
void Screen_Level_Editor::checkpanelloadbutton(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p) {
	if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
		_inputmanager.releasekey(SDL_BUTTON_LEFT);
		_loadpanel->active = 1;
	}
}
void Screen_Level_Editor::checkcamresetbutton(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p) {
	if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
		_camera.setposition(glm::vec2(0));
		_camera.setscale(1);
	}
}

//save load
void Screen_Level_Editor::checksaveactionbutton(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p) {
	if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
		path = p->editboxs[0]->text;
		save();
		p->active = false;
	}

}
void Screen_Level_Editor::checkloadactionbutton(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p) {
	if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {

		path = p->editboxs[0]->text;
		if (!_con_box.empty() || !_con_spawnpoint.empty()) {
			_loadeven->active = true;
		}
		else {
			load();
		}

		p->active = false;
	}
}

//areyousurepanel
void Screen_Level_Editor::checkresetpageyes(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p) {
	if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
		_con_box.clear();
		_con_spawnpoint.clear();
		p->active = false;
	}
}
void Screen_Level_Editor::checkloadevenyes(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p) {
	if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
		load();
		p->active = false;
	}
}


//END