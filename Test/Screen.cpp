#include "Screen.h"
#include <ngiv\ModelLoader.h>
#include <time.h>
#include <ngiv\Collision_Box.h>

Screen::Screen()
{
}
Screen::~Screen()
{
}


void Screen::init()
{
	_cam3d.init(glm::vec3(13, 13, -5), 0.4, 0.2, 60, _width, _height,0.1f ,400.0f);
	_cam3d.lookat(glm::vec3(5, 0, 0));

	_cam3d.updateMatrix();
	_3drenderer.init(&_cam3d, _width, _height);
	_3drenderer.loadSkybox("001");
	_world.init(0.001);

	//exampleobj = ngiv::ModelLoader::loadModel(glm::vec3(2, 0, 0), "Models//metalboxv2obj//untitled.obj", false, false, 1.0f);
	
	floorbox = ngiv::ModelLoader::loadModel(glm::vec3(0, -5, 0), "Models//metalboxv2obj//untitled.obj", true, glm::vec3(100,10,100));
	sphere1 = ngiv::ModelLoader::loadModel(glm::vec3(0, 5, 0), "Models//lowpolysphereobj//sphere.obj", true);
	sphere2 = ngiv::ModelLoader::loadModel(glm::vec3(3, 5, 2), "Models//lowpolysphereobj//sphere.obj", true);
	
	ngiv::Collision_Sphere sp1(glm::vec3(0), 1);
	ngiv::Collision_Sphere sp2(glm::vec3(0), 1);
		
	ngiv::Collision_Box fb(glm::vec3(0), floorbox->getCenterPos(), ngiv::Collision_Object::makeCollisionBox(floorbox->getscale()),floorbox->getscale());

	_world.add(sphere1->setCollisionObject(sp1));
	_world.add(sphere2->setCollisionObject(sp2));
	_world.add(floorbox->setCollisionObject(fb));

		
	//nanosuit = ngiv::ModelLoader::loadModel(glm::vec3(0,0,0), "Models//nanosuit//nanosuit.obj", false, true, 1.0f);
	//box = ngiv::ModelLoader::loadModel(glm::vec3(5, 12, 5), "Models//metalboxv2obj//untitled.obj", false, true, 1.0f);
	


//	_terrain.init(time(NULL),10.0f, 32);
	//_terrain.init(1234125, 10.0f, 32);
//	_terrain.create();
		

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

	// DEBUG BALL MOVEMENT
	ngiv::Collision_Object* b1 = sphere1->getCollision_Object();
	ngiv::Collision_Object* b2 = sphere2->getCollision_Object();

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

	if (_window->getState() == ngiv::Window_State::FocusOn) {
		SDL_WarpMouseInWindow(_window->getwindow(), _width / 2, _height / 2);
	}	
	
	updateui();
	
	_world.update();
	
	_cam3d.do_basic_cam_movement(_inputmanager);
	_cam3d.updateCam(_inputmanager);	

	
	sphere1->updateObject();
	sphere2->updateObject();

	checkInput();

	_gui.update();
	return false;
}

void Screen::draw()
{
	_cam3d.updateMatrix();

	_3drenderer.draw(floorbox);
	_3drenderer.draw(sphere1);
	_3drenderer.draw(sphere2);

//	_3drenderer.drawCollisionBox(sphere1->getCollision_Object());
//	_3drenderer.drawCollisionBox(sphere2->getCollision_Object());
//	_3drenderer.drawCollisionBox(floorbox->getCollision_Object());


//	_terrain.draw(&_3drenderer);

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
