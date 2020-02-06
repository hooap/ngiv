#include "BasicLevelEditor.h"
#include "TextureLoader.h"
#include "IOManager.h"

namespace ngiv {

	void BasicLevelEditor::init() {


	}
	void BasicLevelEditor::lateinit() {
		_3drenderer.init(_cam3d, _width, _height);
		_3drenderer.loadSkybox("001");
	}

	void BasicLevelEditor::loadmodelpaths() {
		std::vector<DirEntry> _dir;

		bool preload = false;
		getDirectoryEntries("Models//", _dir);
		for (int i = 0; i < _dir.size(); i++) {
			std::string p = _dir[i].path;

			std::vector<DirEntry> in_dir;
			getDirectoryEntries(p.c_str(), in_dir);
			std::vector<std::string> fs;
			for (int j = 0; j < in_dir.size(); j++) {
				std::string innerpath = in_dir[j].path;
				if (innerpath.substr(innerpath.size() - 4) == ".obj") {
					if (preload) {
						OBJ* to = ModelLoader::loadModel(innerpath, glm::vec3(0), innerpath, false);
						if (to != nullptr) {
							_model_list->elements.push_back(p.substr(8));
							_model_paths.push_back(innerpath);
						}
					}
					else {
						_model_list->elements.push_back(p.substr(8));
						_model_paths.push_back(innerpath);
					}


				}
			}

		}

	}

	void BasicLevelEditor::initui() {

		//left panel
		{
			//panel
			ngiv::NGUI_PANEL* lpanel = _gui.addpanel(glm::vec4(0.5, 0.5, 20, 99), "");



			_model_list = _gui.getdropdownlist(glm::vec4(5, 90, 90, 5), 1.0f, ngiv::ColorRGBA8(0, 0, 0, 255));
			lpanel->dropdowns.push_back(_model_list);
			loadmodelpaths();

			ngiv::NGUI_PANEL_BUTTON* badd = _gui.getpanelbutton("add", 1.0f, glm::vec4(5, 82, 25, 6), "", ngiv::ColorRGBA8(0, 0, 0, 255), std::bind(&BasicLevelEditor::checkaddbutton, this, std::placeholders::_1, std::placeholders::_2));
			lpanel->buttons.push_back(badd);


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


			//savepanelopenbutton
			ngiv::NGUI_PANEL_BUTTON* bsave = _gui.getpanelbutton("Save", 0.9f, glm::vec4(42, 2, 26, 6), "", ngiv::ColorRGBA8(0, 255, 0, 255), std::bind(&BasicLevelEditor::checkpanelsavebutton, this, std::placeholders::_1, std::placeholders::_2));
			lpanel->buttons.push_back(bsave);

			//loadpanelopenbutton
			ngiv::NGUI_PANEL_BUTTON* bload = _gui.getpanelbutton("Load", 0.9f, glm::vec4(72, 2, 26, 6), "", ngiv::ColorRGBA8(0, 255, 0, 255), std::bind(&BasicLevelEditor::checkpanelloadbutton, this, std::placeholders::_1, std::placeholders::_2));
			lpanel->buttons.push_back(bload);


		}

		//save panel
		{
			_savepanel = _gui.addpanel(glm::vec4(30, 30, 40, 40), "", "", true, true, true, "Save map");
			_savepanel->active = false;


			ngiv::NGUI_PANEL_EDITBOX* e1 = _gui.getpaneleditbox("savefilename", glm::vec4(5, 62, 85, 15), 0.8f, "", ngiv::ColorRGBA8(0, 0, 255, 255), "");
			_savepanel->editboxs.push_back(e1);

			//save button
			ngiv::NGUI_PANEL_BUTTON* pb1 = _gui.getpanelbutton("Save", 0.7f, glm::vec4(86, 5, 10, 10), "", ngiv::ColorRGBA8(255, 0, 0, 255), std::bind(&BasicLevelEditor::checksaveactionbutton, this, std::placeholders::_1, std::placeholders::_2));
			_savepanel->buttons.push_back(pb1);
		}
		//load panel
		{
			_loadpanel = _gui.addpanel(glm::vec4(30, 30, 40, 40), "", "", true, true, true, "Load map");
			_loadpanel->active = false;

			ngiv::NGUI_PANEL_EDITBOX* e1 = _gui.getpaneleditbox("loadfilename", glm::vec4(5, 62, 85, 15), 0.8f, "", ngiv::ColorRGBA8(0, 0, 255, 255), "");
			_loadpanel->editboxs.push_back(e1);

			//save button
			ngiv::NGUI_PANEL_BUTTON* pb1 = _gui.getpanelbutton("Load", 0.7f, glm::vec4(86, 5, 10, 10), "", ngiv::ColorRGBA8(255, 0, 0, 255), std::bind(&BasicLevelEditor::checkloadactionbutton, this, std::placeholders::_1, std::placeholders::_2));
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

		if (!_gui.shouldcamerastop() && !_enablemouse) {
			_cam3d->do_basic_cam_movement(_inputmanager);
			_cam3d->updateCam(_inputmanager);
		}

		checkInput();

		if (_window->getState() == ngiv::Window_State::FocusOn && !_enablemouse) {
			SDL_WarpMouseInWindow(_window->getwindow(), _width / 2, _height / 2);
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
			_container->draw_setcollisionalways(_3drenderer);
		}else {
			_container->draw_setalways(_3drenderer);
		}


		_gui.draw();
	}

	void BasicLevelEditor::checkInput() {

		if (_inputmanager.isKeyPressed(SDLK_QUOTEDBL)) {
			if (_enablemouse) {
				SDL_ShowCursor(false);
			}
			else {
				SDL_ShowCursor(true);
			}
			_enablemouse = !_enablemouse;
		}

		if (_inputmanager.isKeyPressed(SDLK_F6)) {
			setSwitchScreen("game");
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

	//left panel

	void BasicLevelEditor::checkaddbutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p) {
		if (_inputmanager.isKeyPressed(SDL_BUTTON_LEFT)) {
			ngiv::o("Pressed checkaddbutton button");
			_inputmanager.releasekey(SDL_BUTTON_LEFT);
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
