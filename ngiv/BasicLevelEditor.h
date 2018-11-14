#pragma once
#include <NGI\IScreen.h>
#include <vector>
#include "Box.h"
#include "Dot.h"

enum class Tool { Free,MoveObject ,SelectedBox, SelectedDot};

class Screen_Level_Editor : public ngi::IScreen{
public:
	Screen_Level_Editor(){}
	~Screen_Level_Editor(){}
	void init() override;
	void initui() override;
	void onEntry() override;
	void onExit() override;
	bool update(float deltatime) override;		
	void draw() override;
	void setscreenPointera(ngi::IScreen* menupointer) override {
		_menupointer = menupointer;
	}

	void save();
	void load();

private:
	std::string path;

	ngi::NGUI_PANEL* _savepanel;
	ngi::NGUI_PANEL* _loadpanel;

	ngi::NGUI_PANEL* _resetpanel;
	ngi::NGUI_PANEL* _loadeven;

	ngi::NGUI_PANEL_CHECKBOX* _debugcheckbox;
	ngi::NGUI_PANEL_CHECKBOX* _forcedgridcheckbox;
	
	ngi::IScreen* _menupointer;
	
	Tool _tool = Tool::Free;
	void* _selected = nullptr;

	std::vector<Box*> _con_box;//
	std::vector<Dot*> _con_spawnpoint;

public:			//GUI FUNCTIONS
	
	//main buttons
	void checkmenubutton(ngi::NGUI_BUTTON* b);

	//left panel	
	void checkcreateboxbutton(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p);
	void checkfreebutton(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p);
	void checkselectbutton(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p);
	void checkcreatespawnpointbutton(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p);
	void checkclearbutton(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p);
	void checkdebugcheckbox(ngi::NGUI_PANEL_CHECKBOX* b, ngi::NGUI_PANEL* p);
	void checkforcedgridcheckbox(ngi::NGUI_PANEL_CHECKBOX* b, ngi::NGUI_PANEL* p);
	void checkpanelsavebutton(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p);
	void checkpanelloadbutton(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p);
	void checkcamresetbutton(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p);


	//save load
	void checksaveactionbutton(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p);
	void checkloadactionbutton(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p);

	//areyousurepanel
	void checkresetpageyes(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p);	

	void checkloadevenyes(ngi::NGUI_PANEL_BUTTON* b, ngi::NGUI_PANEL* p);


};

