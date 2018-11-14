#pragma once
#include "Camera2D.h"
#include "ColorRGBA8.h"
#include <string>
#include "GLSLProgram.h"

#include <vector>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H


namespace ngiv {

	enum class Justification {
		LEFTORBOTTOM, MIDDLE, RIGHTORUP
	};

	struct Character {
		GLuint     textureID;  // ID handle of the glyph texture
		glm::ivec2 Size;       // Size of glyph
		glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
		GLuint     Advance;    // Offset to advance to next glyph
	};

	struct Text_Group {
		ngiv::ColorRGBA8 _col;
		std::vector<GLuint> _textures;
		std::vector<std::vector<GLfloat>> _characters;
	};

	


	class Renderer_Text
	{
	public:
		Renderer_Text();
		~Renderer_Text();


		void init(Camera2D* cam, std::string font, int size,ngiv::GLSLProgram* glsl = nullptr);

		void dispose();

		void draw(std::string text,const glm::vec2& pos,const glm::vec2& scale, ngiv::ColorRGBA8 text_color, float max_x = 0, Justification xjust = Justification::LEFTORBOTTOM, Justification yjust = Justification::LEFTORBOTTOM);
		
		void draw(std::string text, const glm::vec2& pos, const glm::vec2& scale, ngiv::ColorRGBA8 text_color, Justification xjust = Justification::LEFTORBOTTOM, Justification yjust = Justification::LEFTORBOTTOM);
		
		void render();

		glm::vec2 dojustification(glm::vec2 pos, glm::vec2 dims, float yscale, Justification xjust, Justification yjust);


	private:
		bool ownglsl = false;
		int size = 0;
		GLuint VAO = 0;
		GLuint VBO = 0;
		ngiv::GLSLProgram* _glsl;
		Camera2D* _cam;

		FT_Library ft;
		
		std::vector<Text_Group> _groups;
		std::map<GLchar, Character> Characters;
						
	};

}