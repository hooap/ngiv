#include "Renderer_2D.h"
#include "Misc.h"


namespace ngiv {

	Renderer_2D::Renderer_2D()
	{
	}


	Renderer_2D::~Renderer_2D()
	{
		dispose();
	}


	void Renderer_2D::init(Camera2D* cam, bool rgbaonshader ,ngiv::GLSLProgram* glsl) {
		_rgbaonshader = rgbaonshader;
		if (glsl == nullptr) {
			//no glsl specified create a default glsl
			ownglsl = true;
			_glsl = new ngiv::GLSLProgram();
			if (_rgbaonshader) {
				assert("RGBA SHADER NOT IMPLEMENTED");
				_glsl->compileShaders("Shaders//default2DRGBA.vs", "Shaders//default2DRGBA.fs");				
			}
			else {
				_glsl->compileShaders("Shaders//default_2D.vs", "Shaders//default_2D.fs");
			}
			
			_glsl->addAttribute("VertexPos");
			if (_rgbaonshader) {
				_glsl->addAttribute("VertexColor");
			}
			_glsl->addAttribute("VertexUV");
			_glsl->linkShaders();
		}
		else {
			ownglsl = false;
			_glsl = glsl;
		}

		_cam = cam;
		if (_vao == 0) {
			glGenVertexArrays(1,&_vao);
		}
		if (_vbo == 0) {
			glGenBuffers(1,&_vbo);
		}

		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		
		if (_rgbaonshader) {
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DRGBA), (void*)offsetof(Vertex2DRGBA, Vertex2DRGBA::pos));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2DRGBA), (void*)offsetof(Vertex2DRGBA, Vertex2DRGBA::col));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DRGBA), (void*)offsetof(Vertex2DRGBA, Vertex2DRGBA::uv));
		}
		else {
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, Vertex2D::pos));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, Vertex2D::uv));
		}
		

		glBindVertexArray(0);		
	}

	void Renderer_2D::draw(const glm::vec4& destrect, const glm::vec4& uv, GLuint texture, float angle, float depth) {
		if (_vao == 0 || _vbo == 0) {
			ngiv::error("NGIV::RENDERER_2D::NOTINITALIZED::CANTWORK", true);
		}
		if (_rgbaonshader) {
			ngiv::error("NGIV_RENDERER_2D::VERTEX::RGBA::NONGIVEN", true);
		}
		sprites.emplace_back();
		sprites.back().angle = angle;
		sprites.back().texture = texture;
		sprites.back().vertexArray = createVertexArray(destrect, uv, -depth / 100.0f);
	}

	void Renderer_2D::draw(const glm::vec4& destrect, const glm::vec4& uv, GLuint texture, float angle, float depth ,ngiv::ColorRGBA8 col) {
		if (_vao == 0 || _vbo == 0) {
			ngiv::error("NGIV::RENDERER_2D::NOTINITALIZED::CANTWORK", true);
		}
		if (!_rgbaonshader) {
			ngiv::error("NGIV_RENDERER_2D::VERTEX::RGBA::NOTACCEPTABLE", true);
		}
		sprites.emplace_back();
		sprites.back().angle = angle;
		sprites.back().texture = texture;
		sprites.back().vertexArrayrgba = createVertexArray(destrect, uv,col);
	}

	void Renderer_2D::render() {
		if (_vao == 0 || _vbo == 0) {
			ngiv::error("NGIV::RENDERER_2D::NOTINITALIZED::CANTWORK",true);
		}

		if (sprites.empty()) {
			return;
		}

		_glsl->use();
		if (_rgbaonshader) {
			//upload all data 
			std::vector<Vertex2DRGBA> verts;
			verts.reserve(sprites.size());
			for (int i = 0; i < sprites.size(); i++) {
				verts.insert(verts.end(), sprites[i].vertexArrayrgba.begin(), sprites[i].vertexArrayrgba.end());
			}			
			

			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex2DRGBA), verts.data(), GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

		}
		else {
			//upload all data 
			std::vector<Vertex2D> verts;
			verts.reserve(sprites.size());
			for (int i = 0; i < sprites.size(); i++) {
				verts.insert(verts.end(), sprites[i].vertexArray.begin(), sprites[i].vertexArray.end());
			}
			
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex2D), verts.data(), GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		

		
		glUniformMatrix4fv(_glsl->getUniformLocation("projection"), 1, GL_FALSE, &(_cam->getMatrix()[0][0]));


		//render
		glBindVertexArray(_vao);

		int offset = 0;
		for (int i = 0; i < sprites.size(); i++) {
		
			glBindTexture(GL_TEXTURE_2D, sprites[i].texture);

			glDrawArrays(GL_TRIANGLES, offset, 6);
			offset += 6;
		}
		glBindVertexArray(0);

		_glsl->unuse();

		//clear		
		sprites.clear();


	}


	std::vector<Vertex2D> Renderer_2D::createVertexArray(const glm::vec4& destRect, const glm::vec4& uv, float depth) {
		std::vector<Vertex2D> array;
		Vertex2D v;
		//bottom left
		v.pos.x = destRect.x;
		v.pos.y = destRect.y;
		v.pos.z = depth;

		v.uv.x = uv.x;
		v.uv.y = uv.y;		
		array.emplace_back(v);
		//bottom right
		v.pos.x = destRect.x + destRect.z;
		v.pos.y = destRect.y;

		v.uv.x = uv.x + uv.z;
		v.uv.y = uv.y;
		array.emplace_back(v);
		//top right
		v.pos.x = destRect.x + destRect.z;
		v.pos.y = destRect.y + destRect.w;
		v.uv.x = uv.x + uv.z;
		v.uv.y = uv.y + uv.w;
		array.emplace_back(v);
		//top right
		v.pos.x = destRect.x + destRect.z;
		v.pos.y = destRect.y + destRect.w;
		v.uv.x = uv.x + uv.z;
		v.uv.y = uv.y + uv.w;
		array.emplace_back(v);
		//top left
		v.pos.x = destRect.x;
		v.pos.y = destRect.y + destRect.w;
		v.uv.x = uv.x;
		v.uv.y = uv.y + uv.w;
		array.emplace_back(v);
		//bottom left
		v.pos.x = destRect.x;
		v.pos.y = destRect.y;
		v.uv.x = uv.x;
		v.uv.y = uv.y;
		array.emplace_back(v);		

		return array;
	}
	std::vector<Vertex2DRGBA> Renderer_2D::createVertexArray(const glm::vec4& destRect, const glm::vec4& uv, ngiv::ColorRGBA8 col) {
		std::vector<Vertex2DRGBA> array;
		Vertex2DRGBA v;
		//bottom left
		v.col.r = col.r;
		v.col.g = col.g;
		v.col.b = col.b;
		v.col.a = col.a;


		v.pos.x = destRect.x;
		v.pos.y = destRect.y;
		v.uv.x = uv.x;
		v.uv.y = uv.y;		
		array.emplace_back(v);
		//bottom right
		v.pos.x = destRect.x + destRect.z;
		v.pos.y = destRect.y;
		v.uv.x = uv.x + uv.z;
		v.uv.y = uv.y;
		array.emplace_back(v);
		//top right
		v.pos.x = destRect.x + destRect.z;
		v.pos.y = destRect.y + destRect.w;
		v.uv.x = uv.x + uv.z;
		v.uv.y = uv.y + uv.w;
		array.emplace_back(v);
		//top right
		v.pos.x = destRect.x + destRect.z;
		v.pos.y = destRect.y + destRect.w;
		v.uv.x = uv.x + uv.z;
		v.uv.y = uv.y + uv.w;
		array.emplace_back(v);
		//top left
		v.pos.x = destRect.x;
		v.pos.y = destRect.y + destRect.w;
		v.uv.x = uv.x;
		v.uv.y = uv.y + uv.w;
		array.emplace_back(v);
		//bottom left
		v.pos.x = destRect.x;
		v.pos.y = destRect.y;
		v.uv.x = uv.x;
		v.uv.y = uv.y;
		array.emplace_back(v);

		return array;
	}

	void Renderer_2D::dispose() {
		glDeleteBuffers(1, &_vbo);
		glDeleteVertexArrays(1, &_vao);
		sprites.clear();
		
		if (ownglsl) {
			_glsl->dispose();			
		}

	}


}