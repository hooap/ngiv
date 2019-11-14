#pragma once
#include <ngiv/IScreen.h>
#include <ngiv/OBJ.h>
#include <ngiv/Renderer_3D.h>
#include <ngiv/PhysicsWorld.h>
#include <ngiv/TerrainGenerator.h>
#include <ngiv/GameContainer.h>


class Screen : public ngiv::IScreen
{
public:
	Screen();
	~Screen();

	virtual void onEntry() override;
	virtual void onExit() override;
	virtual bool update(float deltatime) override;
	virtual void draw() override;
	virtual void render() override;

	ngiv::GameContainer* getContainer() { return &_container; }
	ngiv::Camera3D* getCam() { return &_cam3d; }

private:
	virtual void initui() override;
	virtual void init() override;
	virtual void dispose() override;




	void save() {
		ngiv::o("log: save");
		_container.savetofile(_savepath + "savefile.txt");
	}
	void load() {
		ngiv::o("log: load");
		_container.clear_delete();
		_world.clear();
		_container.loadfromfile(_savepath + "savefile.txt");
		_container.addalltoWorld(_world);
	}


	bool grav = false;
	void checkInput();
	void updateui();

	ngiv::Camera3D _cam3d;

//	ngiv::OBJ* nanosuit;
//	ngiv::OBJ* box;

	std::string _savefilename = "savefile.txt";
	std::string _savepath = "Saves/";

	ngiv::Renderer_3D _3drenderer;
	ngiv::PhysicsWorld _world;
	ngiv::TerrainGenerator _terrain;
	ngiv::TerrainGenerator _terrain2;


	ngiv::NGUI_TEXT* text;

	ngiv::GameContainer _container;
};

