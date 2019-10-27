#include "NGUI.h"
#include "Misc.h"
#include "TextureLoader.h"
#include <string>
#include "IScreen.h"
#include "Misc.h"

namespace ngiv {

	NGUI::NGUI()
	{
		INITIALIZED = false;
	}
	NGUI::~NGUI()
	{
		destroy();
	}

	void NGUI::init(InputManager* im,int width, int heigth,std::string textfont, int size) {
		if (!INITIALIZED) {
			INITIALIZED = true;

			_texture_red = ngiv::TextureLoader::LoadTexture("UI//red.jpg");
			_texture_green = ngiv::TextureLoader::LoadTexture("UI//green.jpg");
			_texture_blue = ngiv::TextureLoader::LoadTexture("UI//blue.jpg");
			_texture_grey = ngiv::TextureLoader::LoadTexture("UI//grey.jpg");
			_texture_dark_grey = ngiv::TextureLoader::LoadTexture("UI//dark_grey.jpg");		
			_texture_darker_grey = ngiv::TextureLoader::LoadTexture("UI//darker_grey.jpg");

			
			
			_inputmanager = im;
			_camera.init(width, heigth);
			_camera.setposition(glm::vec2(width / 2, heigth / 2));
			_rend.init(&_camera, false);
			_trend.init(&_camera, textfont,size);
		}
		else {
			ngiv::o("NGIV::UI::init::already");
		}
	}		
	void NGUI::destroy() {
		
		_button.clear();
		_text_static.clear();
		_panel.clear();
		_checkbox.clear();
		_editbox.clear();
		_rend.dispose();
		_trend.dispose();
		INITIALIZED = false;
		
	}

	void NGUI::draw() {

	//NE KADAR KUCUK O KADAR ON	
		glm::vec2 wh = _camera.getwh();

		//checkbox
		for (size_t i = 0; i < _checkbox.size(); i++) {
			if (_checkbox[i]->value) {
				_rend.draw(_checkbox[i]->destrect, glm::vec4(0, 0, 1, 1), _checkbox[i]->texture_on, 0 , 1);
			}
			else {
				_rend.draw(_checkbox[i]->destrect, glm::vec4(0, 0, 1, 1), _checkbox[i]->texture_off, 0 , 1);
			}			
		}
	
		//static text
		for (size_t i = 0; i < _text_static.size(); i++) {
			ngiv::Justification jx, jy;

			if (_text_static[i]->isxcenter) {
				jx = ngiv::Justification::MIDDLE;
			}					
			else {
				jx = ngiv::Justification::LEFTORBOTTOM;				
			}
			if (_text_static[i]->isycenter) {
				jy = ngiv::Justification::MIDDLE;
			}
			else {
				jy = ngiv::Justification::LEFTORBOTTOM;
			}

			
			_trend.draw(_text_static[i]->text, ratiotopixel(_text_static[i]->pos,(int)wh.x,(int)wh.y), glm::vec2(_text_static[i]->size), _text_static[i]->color_text, 1, jx, jy);
			//
		}
				
		//buttons
		for (size_t i = 0; i < _button.size(); i++) {
			_rend.draw(ratiotopixel(_button[i]->destrect, (int)wh.x, (int)wh.y), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), _button[i]->texture_body, 0.0f, 2);
			glm::vec2 pos = glm::vec2(_button[i]->destrect.x + _button[i]->destrect.z / 2.0f, _button[i]->destrect.y + _button[i]->destrect.w / 2.0f);
			if (_button[i]->text != "") {
				_trend.draw(_button[i]->text, ratiotopixel(pos, (int)wh.x, (int)wh.y), glm::vec2(_button[i]->size), _button[i]->color_text, 5, ngiv::Justification::MIDDLE, ngiv::Justification::MIDDLE);
			}
		}		

		//editbox
		for (size_t i = 0; i < _editbox.size(); i++) {
			_rend.draw(ratiotopixel(_editbox[i]->destrect, (int)wh.x, (int)wh.y), glm::vec4(0, 0, 1, 1), _editbox[i]->texture_body,0 , 1);
			//glm::vec2 pos = glm::vec2(_editbox[i]->destrect.x + _editbox[i]->destrect.z / 2.0f, _editbox[i]->destrect.y + _editbox[i]->destrect.w / 2.0f);
			_trend.draw(_editbox[i]->text, ratiotopixel(glm::vec2(_editbox[i]->destrect.x, _editbox[i]->destrect.y + _editbox[i]->destrect.w / 2.0f), (int)wh.x, (int)wh.y), glm::vec2(_editbox[i]->tsize), _editbox[i]->color_text, ratiotopixel(_editbox[i]->destrect.x + _editbox[i]->destrect.z, (int)wh.x), 1, ngiv::Justification::LEFTORBOTTOM, ngiv::Justification::MIDDLE);
		}

		//droplist
		for (size_t i = 0; i < _droplist.size(); i++) {
			//draw main box
			_rend.draw(ratiotopixel(_droplist[i]->destrect, (int)wh.x, (int)wh.y), glm::vec4(0, 0, 1, 1), _droplist[i]->texture_background, 0, 1);
			if (_droplist[i]->selected_index != -1) {
				glm::vec4 p = _droplist[i]->destrect;
				_trend.draw(_droplist[i]->elements[_droplist[i]->selected_index], ratiotopixel(glm::vec2(p.x, p.y + (p.w / 2.0f) ), wh.x, wh.y), glm::vec2(_droplist[i]->tsize), _droplist[i]->color_text, 5, ratiotopixel(_droplist[i]->destrect.x + _droplist[i]->destrect.z, (int)wh.x), ngiv::Justification::LEFTORBOTTOM, ngiv::Justification::MIDDLE);
			}
			if (_droplist[i]->open) {
				//todo scrolling
				int num = _droplist[i]->elements.size();

				for (size_t j = 0; j < num; j++) {
					glm::vec4 dropdestrect = _droplist[i]->destrect;
					dropdestrect = glm::vec4(dropdestrect.x, dropdestrect.y - (dropdestrect.w * (j + 1)) , dropdestrect.z, dropdestrect.w);
					_rend.draw(ratiotopixel(dropdestrect,wh.x,wh.y), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), _droplist[i]->texture_listbackground, 0.0f, 1.0f);
					_trend.draw(_droplist[i]->elements[j], ratiotopixel(glm::vec2(dropdestrect.x, dropdestrect.y + (dropdestrect.w / 2.0f) ), wh.x, wh.y), glm::vec2(_droplist[i]->tsize), _droplist[i]->color_text, 5, ratiotopixel(_droplist[i]->destrect.x + _droplist[i]->destrect.z, (int)wh.x), ngiv::Justification::LEFTORBOTTOM, ngiv::Justification::MIDDLE);
				}



			}		


		}

		//panel
		for (size_t i = 0; i < _panel.size(); i++) {
			if (!_panel[i]->active) continue;

			//panelbox					
			glm::vec4 pp = ratiotopixel(_panel[i]->destrect, (int)wh.x, (int)wh.y);
			_rend.draw(pp, glm::vec4(0, 0, 1, 1), _panel[i]->texture_body, 0, 10);


			//buttons
			for (size_t j = 0; j < _panel[i]->buttons.size(); j++) {
				//button				
				glm::vec4 bpos = getdestrectinpanel(_panel[i]->destrect, _panel[i]->buttons[j]->destrect, (int)wh.x, (int)wh.y);
				_rend.draw(bpos, glm::vec4(0, 0, 1, 1), _panel[i]->buttons[j]->texture_body, 0, 15);
				//text
				glm::vec2 pos = glm::vec2(bpos.x + bpos.z / 2.0f, bpos.y + bpos.w / 2.0f);
				_trend.draw(_panel[i]->buttons[j]->text, pos, glm::vec2(_panel[i]->buttons[j]->size), _panel[i]->buttons[j]->color_text, 25, ngiv::Justification::MIDDLE, ngiv::Justification::MIDDLE);

			}

			//checkbox			
			for (size_t j = 0; j < _panel[i]->checkboxs.size(); j++) {
				glm::vec4 bpos = getdestrectinpanel(_panel[i]->destrect, _panel[i]->checkboxs[j]->destrect, (int)wh.x, (int)wh.y);
				if (_panel[i]->checkboxs[j]->value) {
					_rend.draw(bpos, glm::vec4(0, 0, 1, 1), _panel[i]->checkboxs[j]->texture_on, 0, 15);
				}
				else {
					_rend.draw(bpos, glm::vec4(0, 0, 1, 1), _panel[i]->checkboxs[j]->texture_off, 0, 15);
				}


			}

			//editbox			
			for (size_t j = 0; j < _panel[i]->editboxs.size(); j++) {
				glm::vec4 bpos = getdestrectinpanel(_panel[i]->destrect, _panel[i]->editboxs[j]->destrect, (int)wh.x, (int)wh.y);
				_rend.draw(bpos, glm::vec4(0, 0, 1, 1), _panel[i]->editboxs[j]->texture_body, 0, 15);
				glm::vec2 pos = glm::vec2(bpos.x, bpos.y + bpos.w / 2.0f);
				_trend.draw(_panel[i]->editboxs[j]->text, pos, glm::vec2(_panel[i]->editboxs[j]->tsize), _panel[i]->editboxs[j]->color_text, 20, bpos.x + bpos.z, ngiv::Justification::LEFTORBOTTOM, ngiv::Justification::MIDDLE);
			}

			//text
			for (size_t j = 0; j < _panel[i]->texts.size(); j++) {
				ngiv::Justification jx, jy;
				if (_panel[i]->texts[j]->isxcenter) {
					jx = ngiv::Justification::MIDDLE;
				}
				else {
					jx = ngiv::Justification::LEFTORBOTTOM;
				}
				if (_panel[i]->texts[j]->isycenter) {
					jy = ngiv::Justification::MIDDLE;
				}
				else {
					jy = ngiv::Justification::LEFTORBOTTOM;
				}


				_trend.draw(_panel[i]->texts[j]->text, getdestrectinpanel(_panel[i]->destrect, _panel[i]->texts[j]->pos, (int)wh.x, (int)wh.y), glm::vec2(_panel[i]->texts[j]->size), _panel[i]->texts[j]->color_text, 15, jx, jy);
				//
			}

			//droplist
			for (size_t j = 0; j < _panel[i]->dropdowns.size(); j++) {
				//draw main box

				glm::vec4 drdest = getdestrectinpanel(_panel[i]->destrect, _panel[i]->dropdowns[j]->destrect, (int)wh.x, (int)wh.y);
				_rend.draw(drdest, glm::vec4(0, 0, 1, 1), _panel[i]->dropdowns[j]->texture_background, 0, 15);

				float max_x = drdest.x + drdest.z;
				glm::vec2 p = glm::vec2(drdest.x, drdest.y + (drdest.w / 2.0f));

				if (_panel[i]->dropdowns[j]->selected_index != -1) {
					_trend.draw(_panel[i]->dropdowns[j]->elements[_panel[i]->dropdowns[j]->selected_index], p, glm::vec2(_panel[i]->dropdowns[j]->tsize), _panel[i]->dropdowns[j]->color_text, 20, max_x, ngiv::Justification::LEFTORBOTTOM, ngiv::Justification::MIDDLE);
				}
				
				if (_panel[i]->dropdowns[j]->open) {
					//todo scrolling
					int num = _panel[i]->dropdowns[j]->elements.size();

					for (size_t ff = 0; ff < num; ff++) {
						glm::vec4 dropdestrect = drdest;
						dropdestrect = glm::vec4(dropdestrect.x, dropdestrect.y - (dropdestrect.w * (ff + 1)), dropdestrect.z, dropdestrect.w);
						_rend.draw(dropdestrect, glm::vec4(0, 0, 1, 1), _panel[i]->dropdowns[j]->texture_listbackground, 0, 20);
						_trend.draw(_panel[i]->dropdowns[j]->elements[ff], glm::vec2(dropdestrect.x, dropdestrect.y + (dropdestrect.w / 2.0f)), glm::vec2(_panel[i]->dropdowns[j]->tsize), _panel[i]->dropdowns[j]->color_text, 25, max_x, ngiv::Justification::LEFTORBOTTOM, ngiv::Justification::MIDDLE);
					}

				}
				

			}


			//panel
			for (size_t j = 0; j < _panel[i]->panels.size(); j++) {
				//panelbox					
				glm::vec4 pp = getdestrectinpanel(_panel[i]->destrect, _panel[i]->panels[j]->destrect, (int)wh.x, (int)wh.y);
				_rend.draw(pp, glm::vec4(0, 0, 1, 1), _panel[i]->panels[j]->texture_body, 0, 30);


				std::vector<NGUI_PANEL_BUTTON*>* s = &_panel[i]->panels[j]->buttons;
				//button
				for (size_t m = 0; m < s->size(); m++) {
					//button				
					glm::vec4 bpos = getdestrectinpanelpanel(_panel[i]->destrect, _panel[i]->panels[j]->destrect, (*s)[m]->destrect, (int)wh.x, (int)wh.y);
					_rend.draw(bpos, glm::vec4(0, 0, 1, 1), (*s)[m]->texture_body, 0,35);
					//text
					glm::vec2 pos = glm::vec2(bpos.x + bpos.z / 2.0f, bpos.y + bpos.w / 2.0f);
					_trend.draw((*s)[m]->text, pos, glm::vec2((*s)[m]->size), (*s)[m]->color_text, 40, ngiv::Justification::MIDDLE, ngiv::Justification::MIDDLE);
				}

				std::vector<NGUI_TEXT*>* ts = &_panel[i]->panels[j]->texts;
				for (size_t m = 0; m < ts->size(); m++) {
					
					glm::vec2 bpos = getdestrectinpanelpanel(_panel[i]->destrect, _panel[i]->panels[j]->destrect, (*ts)[m]->pos, (int)wh.x, (int)wh.y);
					_trend.draw((*ts)[m]->text, bpos, glm::vec2((*ts)[m]->size), (*ts)[m]->color_text, 40, ngiv::Justification::LEFTORBOTTOM, ngiv::Justification::MIDDLE);
				}

			}
		}		

		
	}

	void NGUI::render() {

		_rend.render();
		_trend.render();

	}

	void NGUI::update() {
		_camera.update();
		bool cam = false;
		glm::vec2 mpos = _inputmanager->getMouseCoord();
		mpos = _camera.translatescreentoworld(mpos);
		glm::vec2 wh = _camera.getwh();
		glm::vec4 p;
		bool clicked = false;
		if (_inputmanager->isKeyPressed(SDL_BUTTON_LEFT)) {
			clicked = true;
		}
		
		//panel update
		for (size_t i = 0; i < _panel.size(); i++) {
			if (!_panel[i]->active) continue;

			//panels in panel
			for (size_t j = 0; j < _panel[i]->panels.size(); j++) {
				if (!_panel[i]->panels[j]->active) continue;

				if (!_inputmanager->isKeyDown(SDL_BUTTON_LEFT)) {
					_panel[i]->panels[j]->value = false;
				}

				bool f = false;
				//check and call cfunc			
				p = getdestrectinpanel(_panel[i]->destrect, _panel[i]->panels[j]->destrect, (int)wh.x, (int)wh.y);
				if (mpos.x > p.x) {
					if (mpos.x < p.x + p.z) {
						if (mpos.y > p.y) {
							if (mpos.y < p.y + p.w) {
								_panel[i]->panels[j]->cfunc(_panel[i]->panels[j], _panel[i]);
								f = true;
							}
						}
					}
				}
				if (_panel[i]->panels[j]->value) {
					if (!f) {
						_panel[i]->panels[j]->cfunc(_panel[i]->panels[j], _panel[i]);
					}
				}

				//buttons
				std::vector<NGUI_PANEL_BUTTON*>* s = &_panel[i]->panels[j]->buttons;
				for (size_t m = 0; m < s->size(); m++) {
					p = getdestrectinpanelpanel(_panel[i]->destrect, _panel[i]->panels[j]->destrect, (*s)[m]->destrect, (int)wh.x, (int)wh.y);
					if (mpos.x > p.x) {
						if (mpos.x < p.x + p.z) {
							if (mpos.y > p.y) {
								if (mpos.y < p.y + p.w) {
									if ((*s)[m]->gotomainpanel) {
										(*s)[m]->func((*s)[m], _panel[i]);
									}
									else {
										(*s)[m]->func((*s)[m], _panel[i]->panels[j]);
									}
								}
							}
						}
					}


				}


			}
			
			//buttons
			for (size_t j = 0; j < _panel[i]->buttons.size(); j++) {
				p = getdestrectinpanel(_panel[i]->destrect, _panel[i]->buttons[j]->destrect, (int)wh.x, (int)wh.y);
				if (mpos.x > p.x) {
					if (mpos.x < p.x + p.z) {
						if (mpos.y > p.y) {
							if (mpos.y < p.y + p.w) {
								_panel[i]->buttons[j]->func(_panel[i]->buttons[j], _panel[i]);
							}
						}
					}
				}


			}

			//checkbox
			for (size_t j = 0; j < _panel[i]->checkboxs.size(); j++) {
				p = getdestrectinpanel(_panel[i]->destrect, _panel[i]->checkboxs[j]->destrect, (int)wh.x, (int)wh.y);
				if (mpos.x > p.x) {
					if (mpos.x < p.x + p.z) {
						if (mpos.y > p.y) {
							if (mpos.y < p.y + p.w) {
								_panel[i]->checkboxs[j]->func(_panel[i]->checkboxs[j], _panel[i]);
							}
						}
					}
				}
			}

			//editbox			
			for (size_t j = 0; j < _panel[i]->editboxs.size(); j++) {
				if (_inputmanager->isKeyPressed(SDL_BUTTON_LEFT)) {
					_panel[i]->editboxs[j]->active = false;
				}
				p = getdestrectinpanel(_panel[i]->destrect, _panel[i]->editboxs[j]->destrect, (int)wh.x, (int)wh.y);
				bool f = false;
				if (mpos.x > p.x) {
					if (mpos.x < p.x + p.z) {
						if (mpos.y > p.y) {
							if (mpos.y < p.y + p.w) {
								f = true;
								_panel[i]->editboxs[j]->func(_panel[i]->editboxs[j], _panel[i]);
							}
						}
					}
				}
				if (_panel[i]->editboxs[j]->active) {
					if (!f) {
						f = true;
						_panel[i]->editboxs[j]->func(_panel[i]->editboxs[j], _panel[i]);
					}
					cam = true;
				}
			}

			//dropdownlist update
			for (size_t j = 0; j < _panel[i]->dropdowns.size(); j++) {

				if (clicked) {
					glm::vec4 porig = getdestrectinpanel(_panel[i]->destrect,_panel[i]->dropdowns[j]->destrect, wh.x, wh.y);
					p = porig;
					bool open = _panel[i]->dropdowns[j]->open;
					int num = _panel[i]->dropdowns[j]->elements.size();

					if (open) {
						p = glm::vec4(p.x, p.y - (p.w * num), p.z, p.w * num);
					}
					else {
						p = porig;
					}

					bool onButton = false;
					if (mpos.x > p.x) {
						if (mpos.x < p.x + p.z) {
							if (mpos.y > p.y) {
								if (mpos.y < p.y + p.w) {
									onButton = true;
									if (open) {
										//find the element that was clicked
										for (int f = 0; f < num; f++) {
											if (mpos.y > porig.y - (porig.w* (f + 1)) && mpos.y < porig.y - porig.w * f) {
												_panel[i]->dropdowns[j]->selected_index = f;
												break;
											}
										}

										_panel[i]->dropdowns[j]->open = false;
									}
									else {
										//list was clicked lets open it
										_panel[i]->dropdowns[j]->open = true;
									}
								}
							}
						}
					}

					if (!onButton) {
						_panel[i]->dropdowns[j]->open = false;
					}



				}

			}


		}

		//checkbox
		for (size_t i = 0; i < _checkbox.size(); i++) {
			p = ratiotopixel(_checkbox[i]->destrect, (int)wh.x, (int)wh.y);
			if (mpos.x > p.x) {
				if (mpos.x < p.x + p.z) {
					if (mpos.y > p.y) {
						if (mpos.y < p.y + p.w) {
							_checkbox[i]->func(_checkbox[i]);
						}
					}
				}
			}
		}

		//button update
		for (size_t i = 0; i < _button.size(); i++) {
			p = ratiotopixel(_button[i]->destrect,(int)wh.x,(int)wh.y);
			if (mpos.x > p.x) {
				if (mpos.x < p.x + p.z) {
					if (mpos.y > p.y) {
						if (mpos.y < p.y + p.w) {
							_button[i]->func(_button[i]);
						}
					}
				}
			}
			
			
		}

		//editbox update		
		for (size_t i = 0; i < _editbox.size(); i++) {
			if (_inputmanager->isKeyPressed(SDL_BUTTON_LEFT)) {
				_editbox[i]->active = false;
			}
			p = ratiotopixel(_editbox[i]->destrect, (int)wh.x, (int)wh.y);
			bool f = false;
			if (mpos.x > p.x) {
				if (mpos.x < p.x + p.z) {
					if (mpos.y > p.y) {
						if (mpos.y < p.y + p.w) {
							f = true;
							_editbox[i]->func(_editbox[i]);
						}
					}
				}
			}
			if (_editbox[i]->active) {
				if (!f) {
					f = true;
					_editbox[i]->func(_editbox[i]);
				}
				cam = true;
			}
		}
		
		//dropdownlist update
		for (size_t i = 0; i < _droplist.size(); i++) {		

			if (clicked) {
				glm::vec4 porig = ratiotopixel(_droplist[i]->destrect,wh.x,wh.y);
				p = _droplist[i]->destrect;
				bool open = _droplist[i]->open;
				int num = _droplist[i]->elements.size();

				if (open) {
					p = glm::vec4(p.x, p.y - (p.w * num), p.z, p.w * num);
					p = ratiotopixel(p, wh.x, wh.y);
				}
				else {
					p = porig;
				}


				bool onButton = false;
				if (mpos.x > p.x) {
					if (mpos.x < p.x + p.z) {
						if (mpos.y > p.y) {
							if (mpos.y < p.y + p.w) {
								onButton = true;
								if (open) {
									//find the element that was clicked
									for (int f = 0; f < num; f++) {
										if (mpos.y > porig.y - (porig.w* (f + 1)) && mpos.y < porig.y - porig.w * f) {
											_droplist[i]->selected_index = f;
											break;
										}
									}

									_droplist[i]->open = false;
								}
								else {
									//list was clicked lets open it
									_droplist[i]->open = true;
								}
							}
						}
					}
				}				

				if (!onButton) {
					_droplist[i]->open = false;
				}



			}
					   			 
		}

	


		if (cam == false) {
			_shouldcamerastop = false;
		}
	}

	//gets
	NGUI_PANEL*				 NGUI::getpanel(glm::vec4& pos, std::string bodypostfix, std::string titlebarpostfix, bool isdynamic , bool canclose, bool createtitlebar, std::string titlebar_text, std::function<void(NGUI_PANEL*, NGUI_PANEL*)> cfunc) {
		if (bodypostfix != "" || titlebarpostfix != "") {
			assert("LOADING THEMES NOT SUPPORTED");
		}
		return getpanel(pos, _texture_dark_grey, _texture_darker_grey, isdynamic, canclose, createtitlebar, titlebar_text, cfunc);
	}
	NGUI_PANEL*				 NGUI::getpanel(glm::vec4& pos, GLuint body, GLuint titlebar, bool isdynamic, bool canclose, bool createtitlebar, std::string titlebar_text, std::function<void(NGUI_PANEL*, NGUI_PANEL*)> cfunc) {
		if (!INITIALIZED) {
			ngiv::o("NGUI not initialized");
			return nullptr;
		}
		NGUI_PANEL* p = new NGUI_PANEL();
		p->destrect = pos;
		p->dynamic = isdynamic;
		if (cfunc != nullptr) {
			p->cfunc = cfunc;
		}
		else {
			p->cfunc = std::bind(&NGUI::default_panelfunc, this, std::placeholders::_1, std::placeholders::_2);
		}


		p->texture_body = body;
		p->active = true;
		p->value = false;

		if (createtitlebar) {
			//create titlebar

			NGUI_PANEL* ubp = new NGUI_PANEL();
			glm::vec4 titl = glm::vec4(0, 85, 100, 15);
			ubp->destrect = titl;
			ubp->active = true;
			ubp->dynamic = false;
			ubp->value = false;

			if (titlebar_text != "") {
				NGUI_TEXT* t = gettext(titlebar_text, glm::vec2(2,50), ngiv::ColorRGBA8(0, 0, 0, 255), 1.0f, false, false);
				ubp->texts.push_back(t);
			}
			
			ubp->texture_body = titlebar;
			ubp->cfunc = std::bind(&NGUI::default_dynamicborderfunc, this, std::placeholders::_1, std::placeholders::_2);
			//create close button
			if (canclose) {
				NGUI_PANEL_BUTTON* cb = new NGUI_PANEL_BUTTON();
				cb->destrect = glm::vec4(89, 20, 8, 60);
				cb->texture_body = _texture_red;
				cb->size = 1;
				cb->color_text = ngiv::ColorRGBA8(255, 255, 255, 255);
				cb->text = "X";
				cb->gotomainpanel = true;
				cb->func = std::bind(&NGUI::default_closebuttonfunc, this, std::placeholders::_1, std::placeholders::_2);
				ubp->buttons.push_back(cb);
			}
			p->panels.push_back(ubp);

		}



		return p;
	}
	
	NGUI_TEXT*		         NGUI::gettext(std::string t, glm::vec2 pos, ngiv::ColorRGBA8 color_text, float size, bool isxcenter, bool isycenter) {
		if (!INITIALIZED) {
			ngiv::o("NGUI not initialized");
			return nullptr;
		}
		NGUI_TEXT* p = new NGUI_TEXT();

		p->text = t;
		p->pos = pos;
		p->color_text = color_text;
		p->size = size;
		p->isxcenter = isxcenter;
		p->isycenter = isycenter;
		return p;
	}
	
	NGUI_PANEL_BUTTON*		 NGUI::getpanelbutton(std::string t, float tsize, glm::vec4& bdestrect, std::string bodypostfix, ngiv::ColorRGBA8 color_text, std::function<void(NGUI_PANEL_BUTTON*, NGUI_PANEL*)> func) {
		if (bodypostfix != "") {
			assert("LOADING THEMES NOT SUPPORTED");
		}		
		return getpanelbutton(t, tsize, bdestrect, _texture_darker_grey, color_text, func);
	}
	NGUI_PANEL_BUTTON*		 NGUI::getpanelbutton(std::string t, float tsize, glm::vec4& bdestrect, GLuint bodycolor, ngiv::ColorRGBA8 color_text, std::function<void(NGUI_PANEL_BUTTON*, NGUI_PANEL*)> func) {
		if (!INITIALIZED) {
			ngiv::o("NGUI not initialized");
			return nullptr;
		}

		NGUI_PANEL_BUTTON* p = new NGUI_PANEL_BUTTON();

		p->text = t;
		p->size = tsize;
		p->destrect = bdestrect;

		
		p->texture_body = bodycolor;
		p->color_text = color_text;
		p->func = func;
		p->gotomainpanel = false;
		return p;
	}
	
	NGUI_PANEL_CHECKBOX*	 NGUI::getpanelcheckbox(std::string name, glm::vec4& pos, GLuint texture_off, GLuint texture_on, bool dvalue, std::function<void(NGUI_PANEL_CHECKBOX*, NGUI_PANEL*)> func) {
		if (!INITIALIZED) {
			ngiv::o("NGUI not initialized");
			return nullptr;
		}

		NGUI_PANEL_CHECKBOX* p = new NGUI_PANEL_CHECKBOX();

		p->name = name;
		p->destrect = pos;
		p->texture_off = texture_off;
		p->texture_on = texture_on;
		p->value = dvalue;
		p->func = func;		
		return p;
	}
	NGUI_PANEL_CHECKBOX*	 NGUI::getpanelcheckbox(std::string name, glm::vec4& pos, std::string texture_off_postfix, std::string texture_on_postfix, bool dvalue, std::function<void(NGUI_PANEL_CHECKBOX*, NGUI_PANEL*)> func) {
		if (texture_off_postfix != "" || texture_on_postfix != "") {
			assert("LOADING THEMES NOT SUPPORTED");
		}		
		return getpanelcheckbox(name, pos, _texture_red, _texture_green, dvalue, func);
	}
		
	NGUI_PANEL_EDITBOX*		 NGUI::getpaneleditbox(std::string name, glm::vec4& pos, float tsize, GLuint texture_body, ngiv::ColorRGBA8 color_text, std::string text, std::function<void(NGUI_PANEL_EDITBOX*, NGUI_PANEL*)> func) {
		if (!INITIALIZED) {
			ngiv::o("NGUI not initialized");
			return nullptr;
		}
		NGUI_PANEL_EDITBOX* p = new NGUI_PANEL_EDITBOX();

		p->active = false;
		p->texture_body = texture_body;
		p->color_text = color_text;
		p->tsize = tsize;
		p->destrect = pos;

		if (func == nullptr) {
			p->func = std::bind(&NGUI::default_paneleditboxfunc, this, std::placeholders::_1, std::placeholders::_2);
		}
		else {
			p->func = func;
		}

		p->text = text;
				
		return p;

	}
	NGUI_PANEL_EDITBOX*		 NGUI::getpaneleditbox(std::string name, glm::vec4& pos, float tsize, std::string bodypostfix, ngiv::ColorRGBA8 color_text, std::string text, std::function<void(NGUI_PANEL_EDITBOX*, NGUI_PANEL*)> func) {
		if (bodypostfix != "") {
			assert("LOADING THEMES NOT SUPPORTED");
		}
		return getpaneleditbox(name, pos, tsize, _texture_grey, color_text, text, func);
	}

	NGUI_DROPDOWN_LIST*		 NGUI::getdropdownlist(glm::vec4& pos, float tsize, ColorRGBA8 tcol, GLuint texture_background, GLuint texture_listbackground, std::string defaultelement) {
		if (!INITIALIZED) {
			ngiv::o("NGUI not initialized");
			return nullptr;
		}
		NGUI_DROPDOWN_LIST* list = new NGUI_DROPDOWN_LIST();

		list->color_text = tcol;
		list->destrect = pos;
		list->texture_background = texture_background;
		list->texture_listbackground = texture_listbackground;

		if (defaultelement != "") {
			list->elements.push_back(defaultelement);
			list->selected_index = 0;
		}
		else {
			list->selected_index = -1;
		}

		list->open = false;
		list->tsize = tsize;
		return list;
	}
	NGUI_DROPDOWN_LIST*		 NGUI::getdropdownlist(glm::vec4& pos, float tsize, ColorRGBA8 tcol, std::string postfix_texture_background, std::string postfix_texture_listbackground, std::string defaultelement) {
		if (postfix_texture_background != "" || postfix_texture_listbackground != "") {
			assert("LOADING THEMES NOT SUPPORTED");
		}
		return getdropdownlist(pos, tsize, tcol, _texture_darker_grey, _texture_dark_grey, defaultelement);
	}
		   	
	//adds
	NGUI_PANEL*				 NGUI::addpanel(glm::vec4& pos, std::string bodypostfix, std::string titlebarpostfix, bool isdynamic, bool canclose, bool createtitlebar, std::string titlebar_text, std::function<void(NGUI_PANEL*, NGUI_PANEL*)> cfunc) {
		NGUI_PANEL* p = getpanel(pos, bodypostfix, titlebarpostfix, isdynamic, canclose, createtitlebar,titlebar_text, cfunc);
		_panel.push_back(p);
		return p;

	}
	NGUI_PANEL*				 NGUI::addpanel(glm::vec4& pos, GLuint body, GLuint titlebar, bool isdynamic, bool canclose, bool createtitlebar, std::string titlebar_text, std::function<void(NGUI_PANEL*, NGUI_PANEL*)> cfunc) {
		NGUI_PANEL* p = getpanel(pos, body, titlebar, isdynamic, canclose, createtitlebar, titlebar_text, cfunc);
		_panel.push_back(p);
		return p;
	}
	
	NGUI_TEXT*				 NGUI::addtext(std::string t, glm::vec2 pos, ngiv::ColorRGBA8 color_text, float size, bool isxcenter, bool isycenter) {
		NGUI_TEXT* p = gettext(t, pos, color_text, size, isxcenter, isycenter);
		_text_static.push_back(p);
		return p;
	}
	
	NGUI_BUTTON*			 NGUI::addbutton(std::string t, float tsize,glm::vec4& bdestrect, std::string bodypostfix, ngiv::ColorRGBA8 color_text, std::function<void(NGUI_BUTTON*)> func) {
		if (bodypostfix != "") {
			assert("LOADING THEMES NOT SUPPORTED");
		}
		return addbutton(t, tsize, bdestrect, _texture_dark_grey, color_text, func);		
	}
	NGUI_BUTTON*			 NGUI::addbutton(std::string t, float tsize, glm::vec4& bdestrect, GLuint bodycolor, ngiv::ColorRGBA8 color_text, std::function<void(NGUI_BUTTON*)> func) {
		if (!INITIALIZED) {
			ngiv::o("NGUI not initialized");
			return nullptr;
		}

		NGUI_BUTTON* p = new NGUI_BUTTON();

		p->text = t;
		p->size = tsize;
		p->destrect = bdestrect;
		p->texture_body = bodycolor;
		p->color_text = color_text;
		p->func = func;
		_button.push_back(p);
		return p;
	}
	
	NGUI_CHECKBOX*			 NGUI::addcheckbox(glm::vec4& pos, GLuint texture_off, GLuint texture_on, bool dvalue, std::function<void(NGUI_CHECKBOX*)> func) {
		if (!INITIALIZED) {
			ngiv::o("NGUI not initialized");
			return nullptr;
		}

		NGUI_CHECKBOX* p = new NGUI_CHECKBOX();

		p->destrect = pos;
		p->texture_off = texture_off;
		p->texture_on = texture_on;
		p->value = dvalue;
		p->func = func;		
		_checkbox.push_back(p);
		return p;
	}
	NGUI_CHECKBOX*			 NGUI::addcheckbox(glm::vec4& pos, std::string texture_off_postfix, std::string texture_on_postfix, bool dvalue, std::function<void(NGUI_CHECKBOX*)> func) {
		if (texture_off_postfix != "" || texture_on_postfix != "") {
			assert("LOADING THEMES NOT SUPPORTED");
		}
		return addcheckbox(pos, _texture_red, _texture_green, dvalue, func);
	}
	
	NGUI_EDITBOX*			 NGUI::addeditbox(glm::vec4& pos, float tsize, GLuint texture_body, ngiv::ColorRGBA8 color_text, std::string text, std::function<void(NGUI_EDITBOX*)> func) {
		if (!INITIALIZED) {
			ngiv::o("NGUI not initialized");
			return nullptr;
		}
		NGUI_EDITBOX* p = new NGUI_EDITBOX();

		p->active = false;
		p->texture_body = texture_body;
		p->color_text = color_text;
		p->tsize = tsize;
		p->destrect = pos;
		if (func == nullptr) {
			p->func = std::bind(&NGUI::default_editboxfunc, this, std::placeholders::_1);
		}
		else {
			p->func = func;
		}
		p->text = text;		
		_editbox.push_back(p);
		return p;

	}
	NGUI_EDITBOX*			 NGUI::addeditbox(glm::vec4& pos, float tsize, std::string bodypostfix, ngiv::ColorRGBA8 color_text, std::string text, std::function<void(NGUI_EDITBOX*)> func) {
		if (bodypostfix != "") {
			assert("LOADING THEMES NOT SUPPORTED");
		}		
		return addeditbox(pos, tsize, _texture_dark_grey, color_text, text, func);
	}

	NGUI_DROPDOWN_LIST*		 NGUI::adddropdownlist(glm::vec4& pos, float tsize, ColorRGBA8 tcol, GLuint texture_background, GLuint texture_listbackground, std::string defaultelement) {
		NGUI_DROPDOWN_LIST* l = getdropdownlist(pos, tsize, tcol, _texture_darker_grey, _texture_dark_grey, defaultelement);
		_droplist.push_back(l);
		return l;
	}
	NGUI_DROPDOWN_LIST*		 NGUI::adddropdownlist(glm::vec4& pos, float tsize, ColorRGBA8 tcol, std::string postfix_texture_background, std::string postfix_texture_listbackground, std::string defaultelement) {
		if (postfix_texture_background != "" || postfix_texture_listbackground != "") {
			assert("LOADING THEMES NOT SUPPORTED");
		}
		return adddropdownlist(pos, tsize, tcol, _texture_darker_grey, _texture_dark_grey, defaultelement);
	}
	   	  	

	//premades
	void			NGUI::addPREMADEok(glm::vec4& pos, std::string text) {
		NGUI_PANEL* p = addpanel(pos, _texture_grey,_texture_red); // SECOND ONE IS NOT USED ITS THERE BECAUSE LAZINESS TO FIX IT
		NGUI_PANEL_BUTTON* b1 = getpanelbutton("Ok", 1, glm::vec4(30, 5, 40, 20), _texture_dark_grey, ngiv::ColorRGBA8(0, 0, 0, 255), std::bind(&NGUI::default_okfunc, this, std::placeholders::_1, std::placeholders::_2));
		p->buttons.push_back(b1);

		NGUI_TEXT* t1 = gettext(text, glm::vec2(50, 75), ngiv::ColorRGBA8(0, 0, 0, 255), 1, true, true);
		p->texts.push_back(t1);
	}
	NGUI_PANEL*		NGUI::addPREMADEyesno(glm::vec4& pos, std::string text, std::function<void(NGUI_PANEL_BUTTON*, NGUI_PANEL*)> func) {
		NGUI_PANEL* p = addpanel(pos, _texture_grey,_texture_red);// SECOND ONE IS NOT USED ITS THERE BECAUSE LAZINESS TO FIX IT

		if (func == nullptr) {
			func = std::bind(&NGUI::default_yesno_yes, this, std::placeholders::_1, std::placeholders::_2);
		}

		NGUI_PANEL_BUTTON* b1 = getpanelbutton("Yes", 1, glm::vec4(10, 5, 35, 20), _texture_dark_grey, ngiv::ColorRGBA8(0, 0, 0, 255), func);
		p->buttons.push_back(b1);
		NGUI_PANEL_BUTTON* b2 = getpanelbutton("No", 1, glm::vec4(55, 5, 35, 20), _texture_dark_grey, ngiv::ColorRGBA8(0, 0, 0, 255), std::bind(&NGUI::default_yesno_no, this, std::placeholders::_1, std::placeholders::_2));
		p->buttons.push_back(b2);

		NGUI_TEXT* t1 = gettext(text, glm::vec2(50, 75), ngiv::ColorRGBA8(0, 0, 0, 255), 1, true, true);
		p->texts.push_back(t1);
		p->active = false;

		return p;
	}
	
	void NGUI::default_editboxfunc(NGUI_EDITBOX* e) {		
		if(_inputmanager->isKeyPressed(SDL_BUTTON_LEFT)) {
			_inputmanager->releasekey(SDL_BUTTON_LEFT);
			e->active = true;
		}
		
		if (e->active) {
			_shouldcamerastop = true;
			char in = _inputmanager->getPressedKeys();

			//its backspace so delete last item
			if (_inputmanager->isKeyPressed(SDLK_BACKSPACE)) {
				if (e->text.size() > 0) {
					e->text.pop_back();
				}				
			}

			//its a key so add it to text
			if(in >= 33 && in <= 126){
				
				e->text.push_back(in);
			}


		}


	}
	void NGUI::default_paneleditboxfunc(NGUI_PANEL_EDITBOX* e, NGUI_PANEL* p) {
		if (_inputmanager->isKeyPressed(SDL_BUTTON_LEFT)) {
			_inputmanager->releasekey(SDL_BUTTON_LEFT);
			e->active = true;
		}

		if (e->active) {
			_shouldcamerastop = true;
			char in = _inputmanager->getPressedKeys();

			//its backspace so delete last item
			if (_inputmanager->isKeyPressed(SDLK_BACKSPACE)) {
				if (e->text.size() > 0) {
					e->text.pop_back();
				}
			}

			//its a key so add it to text
			if (in >= 33 && in <= 126) {

				e->text.push_back(in);
			}


		}
	}
	void NGUI::default_closebuttonfunc(NGUI_PANEL_BUTTON* b, NGUI_PANEL* p) {
		if (_inputmanager->isKeyPressed(SDL_BUTTON_LEFT)) {
			_inputmanager->releasekey(SDL_BUTTON_LEFT);
			//close
			p->active = false;
		}
	}
	void NGUI::default_panelfunc(NGUI_PANEL* p, NGUI_PANEL* mp) {
		
	}
	void NGUI::default_dynamicborderfunc(NGUI_PANEL* p, NGUI_PANEL* mp) {
	
		glm::vec2 f = pixeltoratio(_inputmanager->getMouseCoord(), (int)_camera.getwh().x, (int)_camera.getwh().y);
		static glm::vec2 lf = f;
		
		if (_inputmanager->isKeyPressed(SDL_BUTTON_LEFT)) {			
			p->value = true;
		}
		if (p->value) {			
			glm::vec2 fark = f - lf;
			mp->destrect = glm::vec4(mp->destrect.x + fark.x,mp->destrect.y - fark.y, mp->destrect.z, mp->destrect.w);

			lf = f;
		}
		else {
			lf = f;
		}

	}
	void NGUI::default_okfunc(NGUI_PANEL_BUTTON* b, NGUI_PANEL* p) {
		if (_inputmanager->isKeyPressed(SDL_BUTTON_LEFT)) {
			p->active = false;
			_inputmanager->releasekey(SDL_BUTTON_LEFT);
		}
	}
	void NGUI::default_yesno_yes(NGUI_PANEL_BUTTON* b, NGUI_PANEL* p) {
		if (_inputmanager->isKeyPressed(SDL_BUTTON_LEFT)) {
			p->value = 1;
			p->active = false;
			_inputmanager->releasekey(SDL_BUTTON_LEFT);
		}
	}
	void NGUI::default_yesno_no(NGUI_PANEL_BUTTON* b, NGUI_PANEL* p) {
		if (_inputmanager->isKeyPressed(SDL_BUTTON_LEFT)) {
			p->value = -1;
			p->active = false;
			_inputmanager->releasekey(SDL_BUTTON_LEFT);
		}
	}
	

}

