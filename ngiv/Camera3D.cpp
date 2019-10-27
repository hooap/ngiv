#include "Camera3D.h"
#include "Misc.h"
#include <SDL\SDL_keycode.h>
#include <SDL\SDL_events.h>

namespace ngiv {

	Camera3D::Camera3D()
	{
		_pos = glm::vec3(0.0f, 0.0f, 0.0f);
		_front = glm::vec3(0.0f, 0.0f, 1.0f);
		_up = glm::vec3(0.0f, 1.0f, 0.0f);		
	}


	Camera3D::~Camera3D()
	{
	}

	void Camera3D::init(glm::vec3 pos, float camSpeed, float sensitivity, float fov, int width, int height, float zNear, float zFar) {
		
		_sensivity = sensitivity;
		_fov = fov;
		_speed = camSpeed;


		_pos = pos;		
		_width = (float)width;
		_height = (float)height;
		
		_zNear = zNear;
		_zfar = zFar;
		_view = glm::lookAt(_pos, _pos + _front, _up);
		_projection = glm::perspective(glm::radians(fov), (float)_width / (float)_height, _zNear, _zfar);
		needUpdate = true;	

	}

	void Camera3D::updateCam(InputManager& i) {	

		static bool first = true;
		static bool second = false;
		if (first) {
			first = false;
			second = true;
			return;
		}	
		if (second) {
			second = false;
			return;
		}

		glm::vec2 mcoord = i.getMouseCoord();	
		glm::vec2 offset = glm::vec2(mcoord.x - (_width / 2), (_height / 2) - mcoord.y);
		
			

		if (!offset.x && !offset.y) {
			return;
		}	


		offset.x *= _sensivity;
		offset.y *= _sensivity;

		_yaw += offset.x;
		_pitch += offset.y;

		if (_pitch > 89.0f)
			_pitch = 89.0f;
		if (_pitch < -89.0f)
			_pitch = -89.0f;

		glm::vec3 front;

	
		front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
		front.y = sin(glm::radians(_pitch));
		front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
		_front = glm::normalize(front);

		needUpdate = true;		

		

	}

	void Camera3D::updateMatrix() {
		if (needUpdate) {
			_view = glm::lookAt(_pos, _pos + _front, _up);
			_projection = glm::perspective(glm::radians(_fov), (float)_width / (float)_height, _zNear, _zfar);
			
			needUpdate = false;
		}	
	}

	void Camera3D::do_basic_cam_movement(InputManager & i)
	{
		if (i.isKeyPressed(SDLK_ASTERISK)) {
			std::cout << _front.x << " " << _front.y << " " << _front.z << "\n";
		}

		if (i.isKeyDown(SDLK_w)) {
			setPos(_pos + _speed * _front);
		}
		if (i.isKeyDown(SDLK_s)) {
			setPos(_pos - _speed * _front);
		}
		if (i.isKeyDown(SDLK_a)) {
			setPos(_pos - glm::normalize(glm::cross(_front, _up)) * _speed);
		}
		if (i.isKeyDown(SDLK_d)) {
			setPos(_pos + glm::normalize(glm::cross(_front, _up)) * _speed);
		}
		if (i.isKeyDown(SDL_BUTTON_RIGHT)) {
			ngiv::o(_pos);
		}

	}

}