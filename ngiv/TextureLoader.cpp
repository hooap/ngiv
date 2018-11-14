#include "TextureLoader.h"

#include "Misc.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


namespace ngiv {

	GLuint TextureLoader::LoadTexture(std::string fpath) {
		static std::map<std::string, GLuint> cache;
		std::map<std::string, GLuint>::iterator it;
		
		it = cache.find(fpath);
		if (it != cache.end()) {
			return it->second;
		}		
		//load texture

	//	stbi_set_flip_vertically_on_load(true);

		GLuint texture;
		glGenTextures(1, &texture);
		
		int twidth, theight, nrChannels;
		unsigned char *data = stbi_load(fpath.c_str(), &twidth, &theight, &nrChannels, 0);
		if (data)
		{
			glBindTexture(GL_TEXTURE_2D, texture);
			if (fpath.find(".png") != std::string::npos) {			
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, twidth, theight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else if (fpath.find(".jpg") != std::string::npos || fpath.find(".jpeg") != std::string::npos) {
				
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else {
				ngiv::error("NGIV::TEXTURELOADER::" + fpath + "::UNKNOWN_FILE_EXTENSION", true);
			}
		}
		else {
			ngiv::error("NGIV::TEXTURELOADER::" + fpath + "::NOT_FOUND_OR_READABLE", true);
		}

		stbi_image_free(data);
		cache.insert(std::pair<std::string, GLuint>(fpath, texture));
		return texture;
	}
}