#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ngiv {

	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();
		void init(int screenwidth, int screenheight);
		void update();
		glm::vec2 translatescreentoworld(glm::vec2 screencoord);

		void movetoPos(const glm::vec2& pos) { _position += pos; need = true; }

		glm::vec2 getpos() { return _position; }
		float getscale() { return _scale; }
		glm::mat4 getMatrix() { return _cameramatrix; }
		glm::vec2 getwh() { return glm::vec2(_screenwidth, _screenheight); }

		void setscale(float scale) { _scale = scale; need = true; }
		void setposition(glm::vec2 newpos) {
			need = true;
			_position = newpos;
		}
		void setscreensize(int width, int heigth) {
			_orthomatrix = glm::ortho(0.0f, (float)width, 0.0f, (float)heigth);
			_screenwidth = width;
			_screenheight = heigth;
			need = true;
		}

	private:
		int _screenwidth = 0, _screenheight = 0;
		float _scale;
		bool need = true;
		glm::mat4 _orthomatrix;
		glm::mat4 _cameramatrix;
		glm::vec2 _position;
	};


}
