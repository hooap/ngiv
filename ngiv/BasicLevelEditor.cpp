#include "BasicLevelEditor.h"
#include "TextureLoader.h"
#include "Misc.h"

namespace ngiv {

	void BasicLevelEditor::init() {
		_cam3d.init(glm::vec3(13, 13, -5), 0.4, 0.2, 60, _width, _height, 0.1f, 400.0f);
		_cam3d.lookat(glm::vec3(5, 0, 0));

		_3drenderer.init(&_cam3d, _width, _height);
		_3drenderer.loadSkybox("001");

	}
	void BasicLevelEditor::initui() {
		//screen buttons
		{
			//return button
			unsigned int returntexture = ngiv::TextureLoader::LoadTexture("UI//return.png");
			_gui.addbutton("S", 3, glm::vec4(0, 95, 5, 5), "", ngiv::ColorRGBA8(255, 0, 0, 255), std::bind(&BasicLevelEditor::checkmenubutton, this, std::placeholders::_1));
			//

		}

		//left panel
		{
			//panel
			ngiv::NGUI_PANEL* lpanel = _gui.addpanel(glm::vec4(0.5, 0.5, 20, 94), "");

			//Free button	
			ngiv::NGUI_PANEL_BUTTON* b1 = _gui.getpanelbutton("Free", 1.0f, glm::vec4(5, 80, 42, 15), "", ngiv::ColorRGBA8(255, 255, 255, 255), std::bind(&BasicLevelEditor::checkfreebutton, this, std::placeholders::_1, std::placeholders::_2));
			lpanel->buttons.push_back(b1);

			//Move button	
			ngiv::NGUI_PANEL_BUTTON* b2 = _gui.getpanelbutton("Move", 1.0f, glm::vec4(53, 80, 42, 15), "", ngiv::ColorRGBA8(255, 255, 255, 255), std::bind(&BasicLevelEditor::checkselectbutton, this, std::placeholders::_1, std::placeholders::_2));
			lpanel->buttons.push_back(b2);

			//create box	
			ngiv::NGUI_PANEL_BUTTON* b3 = _gui.getpanelbutton("Box", 1.0f, glm::vec4(5, 60, 90, 15), "", ngiv::ColorRGBA8(255, 255, 255, 255), std::bind(&BasicLevelEditor::checkcreateboxbutton, this, std::placeholders::_1, std::placeholders::_2));
			lpanel->buttons.push_back(b3);

			//create spawnpoint
			ngiv::NGUI_PANEL_BUTTON* b4 = _gui.getpanelbutton("SpawnPoint", 1.0f, glm::vec4(5, 40, 90, 15), "", ngiv::ColorRGBA8(255, 255, 255, 255), std::bind(&BasicLevelEditor::checkcreatespawnpointbutton, this, std::placeholders::_1, std::placeholders::_2));
			lpanel->buttons.push_back(b4);

			//cam reset
			ngiv::NGUI_PANEL_BUTTON* b5 = _gui.getpanelbutton("CamReset", 1.0f, glm::vec4(4, 26, 42, 10), "", ngiv::ColorRGBA8(255, 255, 255, 255), std::bind(&BasicLevelEditor::checkcamresetbutton, this, std::placeholders::_1, std::placeholders::_2));
			lpanel->buttons.push_back(b5);

			//debug checkbox
			_debugcheckbox = _gui.getpanelcheckbox("debugcheckbox", glm::vec4(1, 0.4, 10, 3), "", "", true, std::bind(&BasicLevelEditor::checkdebugcheckbox, this, std::placeholders::_1, std::placeholders::_2));
			lpanel->checkboxs.push_back(_debugcheckbox);
			//debug checkbox text
			ngiv::NGUI_TEXT* t1 = _gui.gettext("debug", glm::vec2(25, 1.6), ngiv::ColorRGBA8(0, 0, 255, 255), 0.5f, true, true);
			lpanel->texts.push_back(t1);

			//forced grid checkbox
			_forcedgridcheckbox = _gui.getpanelcheckbox("forcedgridcheckbox", glm::vec4(1, 5, 10, 3), "", "", true, std::bind(&BasicLevelEditor::checkforcedgridcheckbox, this, std::placeholders::_1, std::placeholders::_2));
			lpanel->checkboxs.push_back(_forcedgridcheckbox);
			//forced grid text
			ngiv::NGUI_TEXT* t2 = _gui.gettext("forcedgrid", glm::vec2(25, 6.2), ngiv::ColorRGBA8(0, 0, 255, 255), 0.5f, true, true);
			lpanel->texts.push_back(t2);

			//clearbutton
			ngiv::NGUI_PANEL_BUTTON* bclear = _gui.getpanelbutton("Clear", 1.0f, glm::vec4(54, 26, 42, 10), "", ngiv::ColorRGBA8(255, 255, 255, 255), std::bind(&BasicLevelEditor::checkclearbutton, this, std::placeholders::_1, std::placeholders::_2));
			lpanel->buttons.push_back(bclear);

			//savepanelopenbutton
			ngiv::NGUI_PANEL_BUTTON* bsave = _gui.getpanelbutton("Save", 1.0f, glm::vec4(54, 2, 42, 10), "", ngiv::ColorRGBA8(0, 255, 0, 255), std::bind(&BasicLevelEditor::checkpanelsavebutton, this, std::placeholders::_1, std::placeholders::_2));
			lpanel->buttons.push_back(bsave);

			//loadpanelopenbutton
			ngiv::NGUI_PANEL_BUTTON* bload = _gui.getpanelbutton("Load", 1.0f, glm::vec4(54, 14, 42, 10), "", ngiv::ColorRGBA8(0, 255, 0, 255), std::bind(&BasicLevelEditor::checkpanelloadbutton, this, std::placeholders::_1, std::placeholders::_2));
			lpanel->buttons.push_back(bload);


		}

		//save panel
		{
			_savepanel = _gui.addpanel(glm::vec4(30, 30, 40, 40), "", "", true, true, true);
			_savepanel->active = false;

			ngiv::NGUI_PANEL_EDITBOX* e1 = _gui.getpaneleditbox("savefilename", glm::vec4(5, 50, 85, 15), 0.8f, "", ngiv::ColorRGBA8(0, 0, 255, 255), "");
			_savepanel->editboxs.push_back(e1);

			//save button
			ngiv::NGUI_PANEL_BUTTON* pb1 = _gui.getpanelbutton("Save", 0.7, glm::vec4(85, 5, 10, 10), "", ngiv::ColorRGBA8(255, 0, 0, 255), std::bind(&BasicLevelEditor::checksaveactionbutton, this, std::placeholders::_1, std::placeholders::_2));
			_savepanel->buttons.push_back(pb1);
		}

		//load panel
		{
			_loadpanel = _gui.addpanel(glm::vec4(30, 30, 40, 40), "", "", true, true, true);
			_loadpanel->active = false;

			ngiv::NGUI_PANEL_EDITBOX* e1 = _gui.getpaneleditbox("loadfilename", glm::vec4(5, 50, 85, 15), 0.8f, "", ngiv::ColorRGBA8(0, 0, 255, 255), "");
			_loadpanel->editboxs.push_back(e1);

			//save button
			ngiv::NGUI_PANEL_BUTTON* pb1 = _gui.getpanelbutton("Load", 0.7, glm::vec4(85, 5, 10, 10), "", ngiv::ColorRGBA8(255, 0, 0, 255), std::bind(&BasicLevelEditor::checkloadactionbutton, this, std::placeholders::_1, std::placeholders::_2));
			_loadpanel->buttons.push_back(pb1);
		}

		//resetpanel	
		_resetpanel = _gui.addPREMADEyesno(glm::vec4(30, 30, 20, 20), "Are you sure", std::bind(&BasicLevelEditor::checkresetpageyes, this, std::placeholders::_1, std::placeholders::_2));

		//loadeven
		_loadeven = _gui.addPREMADEyesno(glm::vec4(30, 30, 20, 20), "Are you sure", std::bind(&BasicLevelEditor::checkloadevenyes, this, std::placeholders::_1, std::placeholders::_2));

	}
	void BasicLevelEditor::onEntry() {
		SDL_ShowCursor(false);
	}
	void BasicLevelEditor::onExit() {
		SDL_ShowCursor(true);
	}
	bool BasicLevelEditor::update(float deltatime) {

		if (_inputmanager.isKeyPressed(SDLK_ESCAPE)) {
			return true;
		}

		checkInput();

		if (_window->getState() == ngiv::Window_State::FocusOn && !_pressingshift) {
			SDL_WarpMouseInWindow(_window->getwindow(), _width / 2, _height / 2);
		}

		if (!_gui.shouldcamerastop()) {
			_cam3d.do_basic_cam_movement(_inputmanager);
			_cam3d.updateCam(_inputmanager);
		}

		/*

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

		*/


		// true if player wants to exit
		return false;
	}
	void BasicLevelEditor::draw() {

	
		if (_selected != nullptr) {

			switch (_tool)
			{
			case Tool::MoveObject:
				break;
			case Tool::Selected:
				break;
			default:
				break;
			}
		}


		if (_debugcheckbox->value) {
			_container.drawallcollision(_3drenderer);
		}else {
			_container.drawall(_3drenderer);
		}

		_gui.draw();

	}

	void BasicLevelEditor::checkInput() {

		if (_inputmanager.isKeyPressed(SDLK_QUOTEDBL)) {
			if (_pressingshift) {
				SDL_ShowCursor(false);
			}
			else {
				SDL_ShowCursor(true);
			}
			_pressingshift = !_pressingshift;
		}
		

	}

	void BasicLevelEditor::render() {

		_3drenderer.render();
		_gui.render();
	}

	void BasicLevelEditor::save() {
		ngiv::o("save");
	}
	void BasicLevelEditor::load() {
		ngiv::o("load");
	}

	//main
	void BasicLevelEditor::checkmenubutton(ngiv::NGUI_BUTTON* b) {

		if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
			ngiv::o("pressed menu button");
			/*
			setSwitchScreen("menubutton");
			*/
			_inputmanager.releasekey(SDL_BUTTON_LEFT);

		}

	}

	//left panel
	void BasicLevelEditor::checkcreateboxbutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p) {

		if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {

			if (_selected != nullptr) {
				delete _selected;
			}

			ngiv::o("Pressed checkcreateboxbutton button");
			/*
			_selected = new Box();
			((Box*)_selected)->createbox(_mousepos, glm::vec2(50, 50), 0, "Textures/Box.png", ngiv::ColorRGBA8(255, 0, 0, 255));
			_inputmanager.releasekey(SDL_BUTTON_LEFT);
			_tool = Tool::SelectedBox;

			*/
		}

	}
	void BasicLevelEditor::checkselectbutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p) {

		if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
			ngiv::o("Pressed checkselectbutton button");

			if (_selected != nullptr) {
				delete _selected;
			}
			_tool = Tool::MoveObject;
			_inputmanager.releasekey(SDL_BUTTON_LEFT);
		}


	}
	void BasicLevelEditor::checkfreebutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p) {
		if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
			ngiv::o("Pressed checkfreebutton button");

			if (_selected != nullptr) {
				delete _selected;
			}
			_tool = Tool::Free;
			_inputmanager.releasekey(SDL_BUTTON_LEFT);
		}
	}
	void BasicLevelEditor::checkcreatespawnpointbutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p) {

		if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
			ngiv::o("Pressed checkcreatespawnpointbutton button");

			if (_selected != nullptr) {
				delete _selected;
			}
			/*
			_selected = new Dot();
			((Dot*)_selected)->createDot(_mousepos, ngiv::ColorRGBA8(0, 0, 255, 255), "Textures/Ball.png", 10.0f, 0);
			_inputmanager.releasekey(SDL_BUTTON_LEFT);
			_tool = Tool::SelectedDot;

			*/

		}

	}
	void BasicLevelEditor::checkclearbutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p) {

		if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
			ngiv::o("Pressed clear button");

			_resetpanel->active = true;
		}

	}
	void BasicLevelEditor::checkdebugcheckbox(ngiv::NGUI_PANEL_CHECKBOX* b, ngiv::NGUI_PANEL* p) {

		if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
			ngiv::o("Pressed checkdebugcheckbox button");

			if (b->value) { b->value = false; }
			else { b->value = true; }
			_inputmanager.releasekey(SDL_BUTTON_LEFT);
		}


	}
	void BasicLevelEditor::checkforcedgridcheckbox(ngiv::NGUI_PANEL_CHECKBOX* b, ngiv::NGUI_PANEL* p) {
		if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
			ngiv::o("Pressed checkforcedgridcheckbox button");
			if (b->value) { b->value = false; }
			else { b->value = true; }
			_inputmanager.releasekey(SDL_BUTTON_LEFT);

		}
	}
	void BasicLevelEditor::checkpanelsavebutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p) {
		if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
			ngiv::o("Pressed checkpanelsavebutton button");

			_inputmanager.releasekey(SDL_BUTTON_LEFT);
			_savepanel->active = 1;

		}
	}
	void BasicLevelEditor::checkpanelloadbutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p) {
		if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
			ngiv::o("Pressed checkpanelloadbutton button");

			_inputmanager.releasekey(SDL_BUTTON_LEFT);
			_loadpanel->active = 1;
		}
	}
	void BasicLevelEditor::checkcamresetbutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p) {
		if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
			ngiv::o("Pressed CamReset button");
		}
	}

	//save load
	void BasicLevelEditor::checksaveactionbutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p) {
		if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
			ngiv::o("Pressed checksaveactionbutton button");

			path = p->editboxs[0]->text;
			save();
			p->active = false;
		}

	}
	void BasicLevelEditor::checkloadactionbutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p) {
		if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {


			ngiv::o("Pressed checkloadactionbutton button");

			p->active = false;
		}
	}

	//areyousurepanel
	void BasicLevelEditor::checkresetpageyes(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p) {
		if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
			ngiv::o("Pressed checkresetpageyes button");
			/*
			_con_box.clear();
			_con_spawnpoint.clear();
			*/

			p->active = false;
		}
	}
	void BasicLevelEditor::checkloadevenyes(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p) {
		if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
			ngiv::o("Pressed checkloadevenyes button");

			load();
			p->active = false;
		}
	}
	



	void BasicLevelEditor::dispose() {

		delete _savepanel;
		delete _loadpanel;
		delete _resetpanel;
		delete _loadeven;
		delete _debugcheckbox;
		delete _forcedgridcheckbox;

	}
	//END

}