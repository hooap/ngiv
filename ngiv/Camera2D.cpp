#include "Camera2D.h"


namespace ngiv {

	Camera2D::Camera2D()
	{
		need = true;
		_scale = 1.0f;
		
	}

	Camera2D::~Camera2D()
	{
	}

	void Camera2D::init(int screenwidth, int screenheight) {
		_orthomatrix = glm::ortho(0.0f, (float)screenwidth, 0.0f, (float)screenheight);
		_screenheight = screenheight;
		_screenwidth = screenwidth;
		_position = glm::vec2(0.0f);
		need = true;		
	}

	void Camera2D::update() {
		if (need == true)
		{
			glm::vec3 translate = glm::vec3(-_position.x + _screenwidth / 2, -_position.y + _screenheight / 2, 0.0f);
			_cameramatrix = glm::translate(_orthomatrix, translate);
			//Camera Scale
			glm::vec3 scale(_scale, _scale, 0.0f);
			_cameramatrix = glm::scale(glm::mat4(1.0f), scale) * _cameramatrix;
			need = false;
		}
	}

	glm::vec2 Camera2D::translatescreentoworld(glm::vec2 screencoord) {
		screencoord.y = _screenheight - screencoord.y;
		screencoord -= glm::vec2(_screenwidth / 2.0f, _screenheight / 2.0f);
		screencoord /= _scale;
		screencoord += _position;
		return screencoord;
	}

}