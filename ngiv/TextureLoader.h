#pragma once
#include <GLAD/glad.h>
#include <string>
#include <map>


namespace ngiv {

	class TextureLoader
	{
	public:
		static GLuint LoadTexture(std::string fpath);

	};

}
