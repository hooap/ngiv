#include "Screen.h"
#include <ngiv/ModelLoader.h>
#include <time.h>
#include <ngiv/Collision_Box.h>
#include <ngiv/Misc.h>
#include <ngiv/Renderer_Text.h>

Screen::Screen()
{
}
Screen::~Screen()
{
}



void Screen::init()
{

	_cam3d.init(glm::vec3(13.0f, 13.0f, -5.0f), 0.4f, 0.2f, 60.0f, _width, _height,0.1f ,2000.0f);
	_cam3d.lookat(glm::vec3(5.0f, 0.0f, 0.0f));

	_3drenderer.init(&_cam3d, _width, _height);
	_3drenderer.loadSkybox("001");




	_world.init(0.001f);

	//exampleobj = ngiv::ModelLoader::loadModel(glm::vec3(2, 0, 0), "Models//metalboxv2obj//untitled.obj", false, false, 1.0f);

	//100,10,100

//	ngiv::OBJ* floorbox;
	ngiv::OBJ* sphere1;
	ngiv::OBJ* sphere2;

//	floorbox = ngiv::ModelLoader::loadModel("floorbox",glm::vec3(0, -5, 0), "Models//metalboxv2obj//untitled.obj", true, glm::vec3(100, 10, 100));
	sphere1 = ngiv::ModelLoader::loadModel("sphere1",glm::vec3(0, 5, 0), "Models//lowpolysphereobj//sphere.obj", true);
	sphere2 = ngiv::ModelLoader::loadModel("sphere2",glm::vec3(3, 5, 2), "Models//lowpolysphereobj//sphere.obj", true);

	ngiv::Collision_Sphere sp1(glm::vec3(0), 1);
	ngiv::Collision_Sphere sp2(glm::vec3(0), 1);

	//ngiv::Collision_Box fb(floorbox->getCenterPosRelative(), floorbox->getCenterPosRelative(), ngiv::Collision_Object::makeCollisionBox(floorbox->getscale()),floorbox->getscale());

	_world.add(sphere1->creatensetCollisionObject(sp1, true));
	_world.add(sphere2->creatensetCollisionObject(sp2, true));
//	_world.add(floorbox->creatensetCollisionObject(fb, false));

//	_container.addObj(floorbox);
	_container.addObj(sphere1);
	_container.addObj(sphere2);


	//_terrain.init((int)time(NULL),200.0f, 8.0f,512);
	_terrain.init(1234125,32.0f, 1.0f, 32, glm::vec3(0));



	//draw
	_container.draw_setalways(_3drenderer);
//	_container.drawallcollision(_3drenderer);
//	_terrain.draw(&_3drenderer);
//	_3drenderer.redraw_static();
}


void Screen::initui()
{
	text = _gui.addtext("123456789", glm::vec2(0, 0), ngiv::ColorRGBA8(255, 255, 255, 255), 1, false, false);
}

void Screen::onEntry()
{
	SDL_ShowCursor(false);

}

void Screen::onExit()
{
	SDL_ShowCursor(true);
}

void Screen::checkInput() {

	if (_inputmanager.isKeyPressed(SDLK_LSHIFT)) {
		_cam3d.setSpeed(_cam3d.getSpeed() + 0.1f);
	}
	if (_inputmanager.isKeyPressed(SDLK_LCTRL)) {
		_cam3d.setSpeed(_cam3d.getSpeed() - 0.1f);
	}


	if (_inputmanager.isKeyPressed(SDLK_KP_MULTIPLY)) {
		grav = !grav;
	}

	if (_inputmanager.isKeyPressed(SDLK_KP_PLUS)) {
		save();
	}
	if (_inputmanager.isKeyPressed(SDLK_KP_MINUS)) {
		load();
	}
	if (_inputmanager.isKeyPressed(SDLK_F6)) {
		setSwitchScreen("leveleditor");
	}

	return;
	// DEBUG BALL MOVEMENT
	ngiv::Collision_Object* b1 = _container.getObjbyName("sphere1")->getCollision_Object();
	ngiv::Collision_Object* b2 = _container.getObjbyName("sphere2")->getCollision_Object();

	float power = 0.001f;
	//1
	if (_inputmanager.isKeyDown(SDLK_KP_8)) {
		b1->addVelocity(glm::vec3(0, power, 0));
	}
	if (_inputmanager.isKeyDown(SDLK_KP_2)) {
		b1->addVelocity(glm::vec3(0, -power, 0));
	}
	if (_inputmanager.isKeyDown(SDLK_KP_4)) {
		b1->addVelocity(glm::vec3(-power, 0, 0));
	}
	if (_inputmanager.isKeyDown(SDLK_KP_6)) {
		b1->addVelocity(glm::vec3(power, 0, 0));
	}
	if (_inputmanager.isKeyDown(SDLK_KP_1)) {
		b1->addVelocity(glm::vec3(0, 0, power));
	}
	if (_inputmanager.isKeyDown(SDLK_KP_7)) {
		b1->addVelocity(glm::vec3(0, 0, -power));
	}
	//2
	if (_inputmanager.isKeyDown(SDLK_i)) {
		b2->addVelocity(glm::vec3(0, power, 0));
	}
	if (_inputmanager.isKeyDown(SDLK_m)) {
		b2->addVelocity(glm::vec3(0, -power, 0));
	}
	if (_inputmanager.isKeyDown(SDLK_j)) {
		b2->addVelocity(glm::vec3(-power, 0, 0));
	}
	if (_inputmanager.isKeyDown(SDLK_l)) {
		b2->addVelocity(glm::vec3(power, 0, 0));
	}
	if (_inputmanager.isKeyDown(SDLK_u)) {
		b2->addVelocity(glm::vec3(0, 0, power));
	}
	if (_inputmanager.isKeyDown(SDLK_n)) {
		b2->addVelocity(glm::vec3(0, 0, -power));
	}
	if (_inputmanager.isKeyDown(SDLK_o)) {

	}




}

void Screen::updateui() {
	std::string t1 = std::to_string(_cam3d.getPos().x);
	int t1d = t1.find(".");
	std::string t2 = std::to_string(_cam3d.getPos().y);
	int t2d = t2.find(".");
	std::string t3 = std::to_string(_cam3d.getPos().z);
	int t3d = t3.find(".");

	text->text = "" + t1.substr(0, t1d + 2) + " " + t2.substr(0, t2d + 2) + " " + t3.substr(0, t3d + 2);


}

bool Screen::update(float deltatime)
{
	if (_inputmanager.isKeyDown(SDLK_ESCAPE)) {
		return true;
	}






	updateui();

	_world.update(grav);

	_cam3d.do_basic_cam_movement(_inputmanager);
	_cam3d.updateCam(_inputmanager);


	if (_window->getState() == ngiv::Window_State::FocusOn) {
		_inputmanager.putMouseMiddleofScreen(_window);
	}


	_container.updateall();



	checkInput();



	return false;
}

void Screen::draw()
{

    _terrain.draw(&_3drenderer,6,_cam3d.getPos());
	_gui.draw();
}

void Screen::render()
{

	_3drenderer.render();

	_gui.render();

}

void Screen::dispose()
{


}
