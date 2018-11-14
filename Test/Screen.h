#pragma once
#include <ngiv\IScreen.h>
#include <ngiv\OBJ.h>
#include <ngiv\Renderer_3D.h>
#include <ngiv\PhysicsWorld.h>
#include <ngiv\TerrainGenerator.h>

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

	
	virtual void initui() override;
	virtual void init() override;
	virtual void dispose() override;
private:


	void checkInput();
	void updateui();

	ngiv::Camera3D _cam3d;

//	ngiv::OBJ* nanosuit;
	ngiv::OBJ* floorbox;
//	ngiv::OBJ* box;

	ngiv::OBJ* sphere1;
	ngiv::OBJ* sphere2;


	ngiv::Renderer_3D _3drenderer;
	ngiv::PhysicsWorld _world;
	ngiv::TerrainGenerator _terrain;

	ngiv::NGUI_TEXT* text;
};

