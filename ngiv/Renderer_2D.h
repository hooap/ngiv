#pragma once
#include <glm\glm.hpp>
#include <GLAD\glad.h>

#include <vector>

#include "Camera2D.h"
#include "GLSLProgram.h"
#include "ColorRGBA8.h"

namespace ngiv {

	
	struct Vertex2D {
		glm::vec2 pos;
		glm::vec2 uv;
	};
	struct Vertex2DRGBA {
		glm::vec2 pos;
		glm::vec4 col;
		glm::vec2 uv;
	};

	class Glyph {
	public:
		Glyph(){}

		std::vector<Vertex2D> vertexArray;
		std::vector<Vertex2DRGBA> vertexArrayrgba;
		GLuint texture;
		float angle;
	};

	class Renderer_2D
	{
	public:
		Renderer_2D();
		~Renderer_2D();

		void init(Camera2D* cam, bool rgbaonshader,ngiv::GLSLProgram* glsl = nullptr);
		void dispose();

		void draw(const glm::vec4& destrect,const glm::vec4& uv, GLuint texture, float angle);
		void draw(const glm::vec4& destrect, const glm::vec4& uv, GLuint texture, float angle, ngiv::ColorRGBA8 col);

		void render();

	private:
		GLuint _vbo = 0;
		GLuint _vao = 0;

		bool _rgbaonshader;
		bool ownglsl = false;
		std::vector<Glyph> sprites;

		Camera2D* _cam;

		ngiv::GLSLProgram* _glsl = nullptr;
		std::vector<Vertex2D> createVertexArray(const glm::vec4& destRect, const glm::vec4& uv);
		std::vector<Vertex2DRGBA> createVertexArray(const glm::vec4& destRect, const glm::vec4& uv, ngiv::ColorRGBA8 col);

	};


}