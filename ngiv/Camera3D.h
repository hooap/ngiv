#pragma once

#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "InputManager.h"

namespace ngiv {

	class Camera3D
	{
	public:
		Camera3D();
		~Camera3D();

		void init(glm::vec3 pos, float camspeed, float sensitivity, float fov, float width, float height, float zNear, float zFar);

		void updateCam(InputManager& i);
		void updateMatrix();


		void do_basic_cam_movement(InputManager& i);
		void lookat(glm::vec3 pos) { 
			_front = pos - _pos;
			_front = glm::normalize(_front);	
			_pitch = glm::degrees(asin(_front.y));
			_yaw = glm::degrees(atan2(-_front.x, _front.z));
			_yaw += 90;

			needUpdate = true; 
		}


		glm::mat4 getView() { return _view; };
		glm::mat4 getProjection() { return _projection; };		

		glm::vec3 getPos() { return _pos; }
		glm::vec3 getFront() { return _front; }
		glm::vec3 getUp() { return _up; }
		float getSpeed() { return _speed; }


		void setPos(glm::vec3 pos) { _pos = pos; needUpdate = true; }		
		void setFront(glm::vec3 pos) { _front = pos; needUpdate = true; }
		float setSpeed(float speed) { _speed = speed; }
		

		

	

		
		bool needUpdate = false;
		
	private:
		float _zNear;
		float _zfar;
		float _yaw = -90.0f;
		float _pitch = 0.0f;


		float _fov = 45.0f;
		float _speed = 1.0f;
		float _sensivity = 0.05f;


		glm::vec3 _pos;
		glm::vec3 _front;
		glm::vec3 _up;
		float _width;
		float _height;

	private:
		glm::mat4 _view;
		glm::mat4 _projection;
	};


}