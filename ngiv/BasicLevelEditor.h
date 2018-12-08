#pragma once
#include "NGUI.h"
#include "IScreen.h"
#include <vector>
#include "Camera3D.h"
#include "Renderer_3D.h"


enum class Tool { Free,MoveObject ,SelectedBox, SelectedDot};

class BasicLevelEditor : public ngiv::IScreen{
public:
	BasicLevelEditor(){}
	~BasicLevelEditor(){}
	void init() override;
	void initui() override;
	void onEntry() override;
	void onExit() override;
	bool update(float deltatime) override;		
	void draw() override;
	void setscreenPointera(ngiv::IScreen* menupointer) override {
		_menupointer = menupointer;
	}

	void save();
	void load();

private:
	std::string path;

	ngiv::NGUI_PANEL* _savepanel;
	ngiv::NGUI_PANEL* _loadpanel;

	ngiv::NGUI_PANEL* _resetpanel;
	ngiv::NGUI_PANEL* _loadeven;

	ngiv::NGUI_PANEL_CHECKBOX* _debugcheckbox;
	ngiv::NGUI_PANEL_CHECKBOX* _forcedgridcheckbox;
	
	ngiv::IScreen* _menupointer;
	
	Tool _tool = Tool::Free;
	void* _selected = nullptr;
	

public:		
	//SCREEN

	ngiv::Camera3D _cam3d;
	ngiv::Renderer_3D _3drenderer;

	//GUI FUNCTIONS
	
	//main buttons
	void checkmenubutton(ngiv::NGUI_BUTTON* b);

	//left panel	
	void checkcreateboxbutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p);
	void checkfreebutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p);
	void checkselectbutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p);
	void checkcreatespawnpointbutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p);
	void checkclearbutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p);
	void checkdebugcheckbox(ngiv::NGUI_PANEL_CHECKBOX* b, ngiv::NGUI_PANEL* p);
	void checkforcedgridcheckbox(ngiv::NGUI_PANEL_CHECKBOX* b, ngiv::NGUI_PANEL* p);
	void checkpanelsavebutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p);
	void checkpanelloadbutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p);
	void checkcamresetbutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p);


	//save load
	void checksaveactionbutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p);
	void checkloadactionbutton(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p);

	//areyousurepanel
	void checkresetpageyes(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p);	

	void checkloadevenyes(ngiv::NGUI_PANEL_BUTTON* b, ngiv::NGUI_PANEL* p);


};

