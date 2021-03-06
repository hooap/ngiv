#pragma once
#include <SDL/SDL_events.h>
#include <GLAD/glad.h>

#include "Renderer_Text.h"
#include "Renderer_2D.h"
#include "ColorRGBA8.h"
#include <vector>
#include <functional>
#include "InputManager.h"
#include <glm/glm.hpp>


//void draw(SpriteBatch& batch, const char* s, glm::vec2 position, glm::vec2 scaling,float depth, ColorRGBA8 tint, float maxx, Justification xjust = Justification::LEFTORBOTTOM, Justification yjust = Justification::LEFTORBOTTOM);


namespace ngiv {



	static glm::vec4 ratiotopixel(const glm::vec4& p, int w, int h) {
		return glm::vec4(p.x * w / 100.0f, p.y * h / 100.0f, p.z * w / 100.0f, p.w * h / 100.0f);
	}
	static glm::vec2 ratiotopixel(const glm::vec2& p, int w, int h) {
		return glm::vec2(p.x * w / 100.0f, p.y * h / 100.0f);
	}
	static float ratiotopixel(float p, int w) {
		return p * w / 100.0f;
	}
	static glm::vec4 pixeltoratio(const glm::vec4& p, int w, int h) {
		return glm::vec4(p.x / w * 100.0f, p.y / h * 100.0f, p.z / w * 100.0f, p.w / h * 100.0f);
	}
	static glm::vec2 pixeltoratio(const glm::vec2& p, int w, int h) {
		return glm::vec2(p.x / w * 100.0f, p.y / h * 100.0f);
	}
	static glm::vec4 getdestrectinpanel(glm::vec4& p, glm::vec4& b, int w, int h) {
		glm::vec4 op;
		op.x = p.x + p.z * b.x / 100;
		op.y = p.y + p.w * b.y / 100;
		op.z = p.z * b.z / 100;
		op.w = p.w * b.w / 100;
		return glm::vec4(op.x * w / 100.0f, op.y * h / 100.0f, op.z * w / 100.0f, op.w * h / 100.0f);
	}
	static glm::vec2 getdestrectinpanel(glm::vec4& p, glm::vec2& b, int w, int h) {
		glm::vec2 op;
		op.x = p.x + p.z * b.x / 100;
		op.y = p.y + p.w * b.y / 100;
		return glm::vec2(op.x * w / 100.0f, op.y * h / 100.0f);
	}

	static glm::vec4 getdestrectinpanelpanel(glm::vec4& mp, glm::vec4& p, glm::vec4& t, int w, int h) {
		glm::vec4 op;
		op.x = mp.x + mp.z * p.x / 100;
		op.y = mp.y + mp.w * p.y / 100;
		op.z = mp.z * p.z / 100;
		op.w = mp.w * p.w / 100;
		glm::vec4 lop;
		lop.x = op.x + op.z * t.x / 100;
		lop.y = op.y + op.w * t.y / 100;
		lop.z = op.z * t.z / 100;
		lop.w = op.w * t.w / 100;
		return glm::vec4(lop.x * w / 100.0f, lop.y * h / 100.0f, lop.z * w / 100.0f, lop.w * h / 100.0f);
	}
	static glm::vec2 getdestrectinpanelpanel(glm::vec4& mp, glm::vec4& p, glm::vec2& t, int w, int h) {
		glm::vec4 op;
		op.x = mp.x + mp.z * p.x / 100;
		op.y = mp.y + mp.w * p.y / 100;
		op.z = mp.z * p.z / 100;
		op.w = mp.w * p.w / 100;
		glm::vec4 lop;
		lop.x = op.x + op.z * t.x / 100;
		lop.y = op.y + op.w * t.y / 100;
		return glm::vec2(lop.x * w / 100.0f, lop.y * h / 100.0f);
	}


	ALIGNED_(16) struct NGUI_ELEMENT_BASE {
		void* operator new(size_t i)
		{
			return _mm_malloc(i, 16);
		}

		void operator delete(void* p)
		{
			_mm_free(p);
		}

	};

	class IScreen;
	struct NGUI_PANEL_BUTTON;
	struct NGUI_PANEL_CHECKBOX;
	struct NGUI_PANEL_EDITBOX;


	struct NGUI_EDITBOX : NGUI_ELEMENT_BASE {
		bool active;
		std::string text;
		float tsize;
		glm::vec4 destrect;
		GLuint texture_body;
		ngiv::ColorRGBA8 color_text;
		std::function<void(NGUI_EDITBOX*)> func;
	};
	struct NGUI_CHECKBOX : NGUI_ELEMENT_BASE {
		bool value;
		glm::vec4 destrect;
		GLuint texture_on;
		GLuint texture_off;
		std::function<void(NGUI_CHECKBOX*)> func;
	};
	struct NGUI_BUTTON : NGUI_ELEMENT_BASE {
		std::string text;
		float size;
		glm::vec4 destrect;
		GLuint texture_body;
		ngiv::ColorRGBA8 color_text;
		std::function<void(NGUI_BUTTON*)> func;
	};
	struct NGUI_TEXT : NGUI_ELEMENT_BASE {
		std::string text;
		float size;
		glm::vec2 pos;
		ngiv::ColorRGBA8 color_text;
		bool isxcenter;
		bool isycenter;
	};
	struct NGUI_DROPDOWN_LIST : NGUI_ELEMENT_BASE	{
		bool open;
		int selected_index;
		glm::vec4 destrect;
		GLuint texture_background;
		GLuint texture_listbackground;
		ngiv::ColorRGBA8 color_text;
		float tsize;

		std::vector<std::string> elements;
	};


	//panel stuff
	struct NGUI_PANEL : NGUI_ELEMENT_BASE {
		std::vector<NGUI_PANEL*> panels;
		std::vector<NGUI_TEXT*> texts;
		std::vector<NGUI_PANEL_BUTTON*> buttons;
		std::vector<NGUI_PANEL_CHECKBOX*> checkboxs;
		std::vector<NGUI_PANEL_EDITBOX*> editboxs;
		std::function<void(NGUI_PANEL*, NGUI_PANEL*)> cfunc;
		std::vector<NGUI_DROPDOWN_LIST*> dropdowns;

		glm::vec4 destrect;
		bool dynamic;
		GLuint texture_body;
		bool active;
		int value;
	};
	struct NGUI_PANEL_BUTTON : NGUI_ELEMENT_BASE	{
		bool gotomainpanel;
		std::string text;
		float size;
		glm::vec4 destrect;
		std::function<void(NGUI_PANEL_BUTTON*,NGUI_PANEL*)> func;
		GLuint texture_body;
		ngiv::ColorRGBA8 color_text;
	};
	struct NGUI_PANEL_CHECKBOX : NGUI_ELEMENT_BASE	{
		std::string name;
		bool value;
		glm::vec4 destrect;
		std::function<void(NGUI_PANEL_CHECKBOX*,NGUI_PANEL*)> func;
		GLuint texture_off;
		GLuint texture_on;
	};
	struct NGUI_PANEL_EDITBOX : NGUI_ELEMENT_BASE {
		bool active;
		std::string text;
		float tsize;
		glm::vec4 destrect;
		std::function<void(NGUI_PANEL_EDITBOX*, NGUI_PANEL*)> func;
		GLuint texture_body;
		ngiv::ColorRGBA8 color_text;
	};


	class NGUI
	{
	public:
		NGUI();
		~NGUI();
		void init(InputManager* im, int width, int heigth, std::string textfont, int size);
		void destroy();

		void draw();
		void render();
		void update();

		bool shouldcamerastop() { return _shouldcamerastop; }

		//get
		NGUI_PANEL*			 getpanel(const glm::vec4& pos, std::string bodypostfix = "", std::string titlebarpostfix = "", bool isdynamic = false, bool canclose = false, bool createtitlebar = false, std::string titlebar_text = "",  std::function<void(NGUI_PANEL*, NGUI_PANEL*)> cfunc = nullptr);
		NGUI_PANEL*			 getpanel(const glm::vec4& pos, GLuint bodycolor, GLuint titlebarcolor, bool isdynamic = false, bool canclose = false, bool createtitlebar = false, std::string titlebar_text = "", std::function<void(NGUI_PANEL*, NGUI_PANEL*)> cfunc = nullptr);


		NGUI_TEXT*			 gettext(std::string t,const glm::vec2& pos, ngiv::ColorRGBA8 color_text, float size, bool isxcenter, bool isycenter);

		NGUI_PANEL_BUTTON*	 getpanelbutton(std::string t, float tsize,const glm::vec4& bdestrect, std::string bodypostfix , ngiv::ColorRGBA8 color_text, std::function<void(NGUI_PANEL_BUTTON*, NGUI_PANEL*)> func);
		NGUI_PANEL_BUTTON*	 getpanelbutton(std::string t, float tsize,const glm::vec4& bdestrect, GLuint bodycolor, ngiv::ColorRGBA8 color_text, std::function<void(NGUI_PANEL_BUTTON*, NGUI_PANEL*)> func);

		NGUI_PANEL_CHECKBOX* getpanelcheckbox(std::string name,const glm::vec4& pos, GLuint texture_off, GLuint texture_on, bool dvalue, std::function<void(NGUI_PANEL_CHECKBOX*, NGUI_PANEL*)> func);
		NGUI_PANEL_CHECKBOX* getpanelcheckbox(std::string name,const glm::vec4& pos, std::string texture_off_postfix, std::string texture_on_postfix, bool dvalue, std::function<void(NGUI_PANEL_CHECKBOX*, NGUI_PANEL*)> func);

		NGUI_PANEL_EDITBOX*  getpaneleditbox(std::string name,const glm::vec4& pos, float tsize, GLuint texture_body, ngiv::ColorRGBA8 color_text, std::string text, std::function<void(NGUI_PANEL_EDITBOX*, NGUI_PANEL*)> func = nullptr);
		NGUI_PANEL_EDITBOX*  getpaneleditbox(std::string name,const glm::vec4& pos, float tsize, std::string bodypostfix, ngiv::ColorRGBA8 color_text, std::string text, std::function<void(NGUI_PANEL_EDITBOX*, NGUI_PANEL*)> func = nullptr);

		NGUI_DROPDOWN_LIST*  getdropdownlist(const glm::vec4& pos, float tsize, ColorRGBA8 tcol, GLuint texture_background, GLuint texture_listbackground, std::string defaultelement = "");
		NGUI_DROPDOWN_LIST*  getdropdownlist(const glm::vec4& pos, float tsize, ColorRGBA8 tcol, std::string postfix_texture_background = "", std::string postfix_texture_listbackground = "", std::string defaultelement = "");




		//add
		NGUI_PANEL*			 addpanel(const glm::vec4& pos, std::string bodypostfix = "", std::string titlebarpostfix = "", bool isdynamic = false, bool canclose = false, bool createtitlebar = false, std::string titlebar_text = "", std::function<void(NGUI_PANEL*, NGUI_PANEL*)> cfunc = nullptr);
		NGUI_PANEL*			 addpanel(const glm::vec4& pos, GLuint bodycolor, GLuint titlebarcolor, bool isdynamic = false, bool canclose = false, bool createtitlebar = false, std::string titlebar_text = "", std::function<void(NGUI_PANEL*, NGUI_PANEL*)> cfunc = nullptr);



		NGUI_TEXT*			 addtext(std::string t,const glm::vec2& pos, ngiv::ColorRGBA8 color_text, float size, bool isxcenter,bool isycenter);

		NGUI_BUTTON*		 addbutton(std::string t, float tsize,const glm::vec4& bdestrect, std::string bodypostfix, ngiv::ColorRGBA8 color_text, std::function<void(NGUI_BUTTON*)> func);
		NGUI_BUTTON*		 addbutton(std::string t, float tsize,const glm::vec4& bdestrect, GLuint bodycolor, ngiv::ColorRGBA8 color_text, std::function<void(NGUI_BUTTON*)> func);

		NGUI_CHECKBOX*		 addcheckbox(const glm::vec4& pos, GLuint texture_off, GLuint texture_on, bool dvalue, std::function<void(NGUI_CHECKBOX*)> func);
		NGUI_CHECKBOX*		 addcheckbox(const glm::vec4& pos, std::string texture_off_postfix, std::string texture_on_postfix, bool dvalue, std::function<void(NGUI_CHECKBOX*)> func);



		NGUI_EDITBOX*		 addeditbox(const glm::vec4& pos, float tsize, GLuint texture_body, ngiv::ColorRGBA8 color_text, std::string text, std::function<void(NGUI_EDITBOX*)> func = nullptr);
		NGUI_EDITBOX*		 addeditbox(const glm::vec4& pos, float tsize, std::string bodypostfix, ngiv::ColorRGBA8 color_text, std::string text, std::function<void(NGUI_EDITBOX*)> func = nullptr);

		NGUI_DROPDOWN_LIST*  adddropdownlist(const glm::vec4& pos, float tsize, ColorRGBA8 tcol, GLuint texture_background, GLuint texture_listbackground, std::string defaultelement = "");
		NGUI_DROPDOWN_LIST*  adddropdownlist(const glm::vec4& pos, float tsize, ColorRGBA8 tcol, std::string postfix_texture_background = "", std::string postfix_texture_listbackground = "", std::string defaultelement = "");



		//premeade

		void addPREMADEok(const glm::vec4& pos, std::string text);
		NGUI_PANEL* addPREMADEyesno(const glm::vec4& pos, std::string text, std::function<void(NGUI_PANEL_BUTTON*,NGUI_PANEL*)> func = nullptr);



		GLuint _texture_red;
		GLuint _texture_green;
		GLuint _texture_blue;
		GLuint _texture_grey;
		GLuint _texture_dark_grey;
		GLuint _texture_darker_grey;
	private:



		bool _shouldcamerastop = false;
		// defaulteditboxfunction

		void default_panelfunc(NGUI_PANEL* p, NGUI_PANEL* mp);
		void default_dynamicborderfunc(NGUI_PANEL* p, NGUI_PANEL* mp);
		void default_editboxfunc(NGUI_EDITBOX* e);
		void default_paneleditboxfunc(NGUI_PANEL_EDITBOX* e, NGUI_PANEL* p);
		void default_closebuttonfunc(NGUI_PANEL_BUTTON* b, NGUI_PANEL* p);
		void default_okfunc(NGUI_PANEL_BUTTON* b, NGUI_PANEL* p);
		void default_yesno_yes(NGUI_PANEL_BUTTON* b, NGUI_PANEL* p);
		void default_yesno_no(NGUI_PANEL_BUTTON* b, NGUI_PANEL* p);

		bool INITIALIZED;

		std::vector<NGUI_BUTTON*> _button;
		std::vector<NGUI_TEXT*> _text_static;
		std::vector<NGUI_PANEL*> _panel;
		std::vector<NGUI_CHECKBOX*> _checkbox;
		std::vector<NGUI_EDITBOX*> _editbox;
		std::vector<NGUI_DROPDOWN_LIST*> _droplist;

		Camera2D _camera;
		InputManager* _inputmanager = nullptr;
		Renderer_2D _rend;
		Renderer_Text _trend;
	};

}
