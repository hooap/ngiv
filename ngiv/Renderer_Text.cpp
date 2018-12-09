#include "Renderer_Text.h"
#include "Misc.h"


namespace ngiv {

	Renderer_Text::Renderer_Text()
	{
		
	}


	Renderer_Text::~Renderer_Text()
	{
		dispose();
	}


	void Renderer_Text::init(Camera2D* cam, std::string font, int size, ngiv::GLSLProgram* glsl) {
		if (glsl == nullptr) {
			//Create a default glslprogram
			_glsl = new ngiv::GLSLProgram();
			_glsl->compileShaders("Shaders//default_text.vs", "Shaders//default_text.fs");
			_glsl->addAttribute("vertex");
			_glsl->linkShaders();
			ownglsl = true;
		}
		else {
			_glsl = glsl;
			ownglsl = false;
		}

		_cam = cam;
		if (FT_Init_FreeType(&ft))
			ngiv::error("NGIV::FREETYPE::COULDNTINITIALIZED",true);

		FT_Face face;
		if (FT_New_Face(ft, font.c_str(), 0, &face))
			ngiv::error("NGIV::FREETYPE::FAILEDTOLOADTTF", true);


		FT_Set_Pixel_Sizes(face, 0, size);


		// load characters

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
		//128 ASCII
		//155 türkçe karakterler
		for (GLubyte c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE::CANTFINDCHARACTER" << std::endl;
				continue;
			}
			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				(GLuint)face->glyph->advance.x
			};
			Characters.insert(std::pair<GLchar, Character>(c, character));
		}
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
				
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexText), (void*)offsetof(VertexText, VertexText::pos));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexText), (void*)offsetof(VertexText, VertexText::texcoord));


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	void Renderer_Text::draw(std::string text,const glm::vec2& pos, const glm::vec2& scale, ngiv::ColorRGBA8 text_color, float depth, float max_x,  Justification xjust, Justification yjust) {
		
		_groups.emplace_back();

		depth = -depth / 100.0f;

		glm::vec2 cpos = pos;

		// Iterate through all characters
		std::string::const_iterator c;

		//iterate and learn dims
		float lenght = 0;
		float heigth = 0;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = Characters[*c];			
			lenght += (ch.Advance >> 6) * scale.x;
			if (ch.Size.y * scale.y > heigth) heigth = ch.Size.y * scale.y;
		}

		cpos = dojustification(cpos, glm::vec2(lenght,heigth), scale.y,xjust, yjust);

		//iterate again and 
		for (c = text.begin(); c != text.end(); c++)
		{
			_groups.back()._characters.emplace_back();
			size++;
			Character ch = Characters[*c];

			GLfloat xpos = cpos.x + ch.Bearing.x * scale.x;
			GLfloat ypos = cpos.y - (ch.Size.y - ch.Bearing.y) * scale.y;

			if (max_x != 0) {
				if (xpos > max_x) {
					return;
				}
			}

			GLfloat w = ch.Size.x * scale.x;
			GLfloat h = ch.Size.y * scale.y;
			// create vbo for each character
			GLfloat vertices[24] = {
				 xpos,     ypos + h,   0.0, 0.0 ,
				 xpos,     ypos,       0.0, 1.0 ,
				 xpos + w, ypos,       1.0, 1.0 ,

				 xpos + w, ypos,	   1.0, 1.0 ,
				 xpos + w, ypos + h,   1.0, 0.0 ,
				 xpos,     ypos + h,   0.0, 0.0 };

			_groups.back()._textures.push_back(ch.textureID);

			VertexText v;
			v.pos.x = xpos;
			v.pos.y = ypos + h;
			v.pos.z = depth;
			v.texcoord.x = 0.0f;
			v.texcoord.y = 0.0f;

			_groups.back()._characters.back().push_back(v);

			v.pos.x = xpos;
			v.pos.y = ypos;
			v.texcoord.x = 0.0f;
			v.texcoord.y = 1.0f;
			_groups.back()._characters.back().push_back(v);

			v.pos.x = xpos + w;
			v.pos.y = ypos;
			v.texcoord.x = 1.0f;
			v.texcoord.y = 1.0f;
			_groups.back()._characters.back().push_back(v);

			v.pos.x = xpos + w;
			v.pos.y = ypos;
			v.texcoord.x = 1.0f;
			v.texcoord.y = 1.0f;
			_groups.back()._characters.back().push_back(v);

			v.pos.x = xpos + w;
			v.pos.y = ypos + h;
			v.texcoord.x = 1.0f;
			v.texcoord.y = 0.0f;
			_groups.back()._characters.back().push_back(v);

			v.pos.x = xpos;
			v.pos.y = ypos + h;
			v.texcoord.x = 0.0f;
			v.texcoord.y = 0.0f;
			_groups.back()._characters.back().push_back(v);

			// take the glyph					

			_groups.back()._col = text_color;
						
			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			cpos.x += (ch.Advance >> 6) * scale.x; // Bitshift by 6 to get value in pixels (2^6 = 64)
		}
		
	}

	void Renderer_Text::draw(std::string text, const glm::vec2& pos, const glm::vec2& scale, ngiv::ColorRGBA8 text_color, float depth, Justification xjust, Justification yjust) {
		draw(text, pos, scale, text_color, depth, 0, xjust, yjust);
	}


	void Renderer_Text::render() {

		_glsl->use();

		//load data to gpu
		std::vector<VertexText> verticies;
	//	verticies.reserve(size);
		for (size_t i = 0; i < _groups.size(); i++) {
			for (size_t j = 0; j < _groups[i]._characters.size(); j++) {
				verticies.insert(verticies.end(), _groups[i]._characters[j].begin(), _groups[i]._characters[j].end());
			}
		}		

		glUniformMatrix4fv(_glsl->getUniformLocation("projection"), 1, GL_FALSE, &(_cam->getMatrix()[0][0]));
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexText) *  verticies.size(), verticies.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		
		glBindVertexArray(VAO);
		int tc = 0;
		glActiveTexture(GL_TEXTURE0);

		int counter = 0;
		for (size_t i = 0; i < _groups.size(); i++) {
			for (size_t j = 0; j < _groups[i]._characters.size(); j++) {
				glUniform3f(_glsl->getUniformLocation("textColor"), _groups[i]._col.r, _groups[i]._col.g, _groups[i]._col.b);
				glBindTexture(GL_TEXTURE_2D, _groups[i]._textures[j]);
				glDrawArrays(GL_TRIANGLES, counter, 6);
				counter += 6;
			}								
		}	
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE0, 0);

		
		//clear memory
		_glsl->unuse();
		_groups.clear();		
		size = 0;
		
	}

	void Renderer_Text::dispose() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		if (ownglsl) {
			_glsl->dispose();
		}


		
		for (auto it : Characters) {
			glDeleteTextures(1,&it.second.textureID);
		}
		Characters.clear();
		_groups.clear();
		

	}

	glm::vec2 Renderer_Text::dojustification(glm::vec2 pos, glm::vec2 dims, float yscale,Justification xjust, Justification yjust) {
		glm::vec2 output = pos;

		switch (yjust)
		{
		case ngiv::Justification::LEFTORBOTTOM:
			break;
		case ngiv::Justification::MIDDLE:
			output.y -= dims.y / 2.0f;
			break;
		case ngiv::Justification::RIGHTORUP:
			output.y -= dims.y;
			break;		
		}

		switch (xjust)
		{
		case ngiv::Justification::LEFTORBOTTOM:
			break;
		case ngiv::Justification::MIDDLE:
			output.x -= dims.x / 2.0f;
			break;
		case ngiv::Justification::RIGHTORUP:
			output.x -= dims.x;
			break;		
		}

		return output;
	}

}