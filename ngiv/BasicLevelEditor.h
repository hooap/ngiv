#pragma once
#include "NGUI.h"
#include "IScreen.h"
#include <vector>
#include "Camera3D.h"
#include "Renderer_3D.h"
#include "GameContainer.h"

namespace ngiv {

	enum class Tool { Free, MoveObject, Selected, MultipleSelection};

	class BasicLevelEditor : public ngiv::IScreen {
	public:
		BasicLevelEditor() {}
		~BasicLevelEditor() {}
		void onEntry() override;
		void onExit() override;
		bool update(float deltatime) override;
		void draw() override;

		void render() override;

		void lateinit();

		
		void save();
		void load();


		void setContainer(GameContainer* cont) {_container = cont;	}
		void setCam(Camera3D* cam) { _cam3d = cam; }

	private:
		void init() override;
		void initui() override;
		void dispose() override;

		void checkInput();


		std::string path;

		ngiv::NGUI_PANEL* _savepanel;
		ngiv::NGUI_PANEL* _loadpanel;

		ngiv::NGUI_PANEL* _resetpanel;
		ngiv::NGUI_PANEL* _loadeven;

		ngiv::NGUI_PANEL_CHECKBOX* _debugcheckbox;
		ngiv::NGUI_PANEL_CHECKBOX* _forcedgridcheckbox;


		Tool _tool = Tool::Free;
		void* _selected = nullptr;
	public:

		bool _enablemouse = false;

		//SCREEN

		GameContainer *_container = nullptr;
		ngiv::Camera3D *_cam3d = nullptr;
		ngiv::Renderer_3D _3drenderer;






		//GUI FUNCTIONS
		//main buttons

		//left panel	


		void checkdebugcheckbox(ngiv::NGUI_PANEL_CHECKBOX* b, ngiv::NGUI_PANEL* p);
		void checkforcedgridcheckbox(ngiv::NGUI_PANEL_CHECKBOX* b, ngiv::NGUI_PANEL* p);
		void checkpanelsavebutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p);
		void checkpanelloadbutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p);


		//save load
		void checksaveactionbutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p);
		void checkloadactionbutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p);

		//areyousurepanel
		void checkresetpageyes(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p);
		void checkloadevenyes(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p);


	};


}