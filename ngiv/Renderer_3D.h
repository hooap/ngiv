#pragma once
#include "Camera3D.h"
#include "GLSLProgram.h"
#include "OBJ.h"

#include <vector>
#include "Collision_Box.h"

namespace ngiv {	

	class Renderer_3D
	{
	public:
		Renderer_3D();
		~Renderer_3D();

		void init(Camera3D* cam, int width, int height, GLSLProgram* glsl = nullptr);
		void loadSkybox(std::string path, std::vector<std::string> facenames = std::vector<std::string>());

		void draw(OBJ* m);
		void drawCollisionBox(Collision_Object* sp);
		void drawMultipleMesh(const std::vector<std::vector<Mesh>>& meshes, const std::vector<std::vector<glm::vec3>>& poss, glm::vec3 scale);


		void render();
	private:
		bool initialized = false;


		void renderWithGLSL(GLSLProgram& glsl);
		void renderStrips(GLSLProgram& glsl);

		void dispose();
		GLuint VAO, VBO, EBO;	

		std::vector<glm::vec3> lightpositions;
		std::vector<glm::vec3> lightcolors;
		
		unsigned int gBuffer;
		unsigned int gPosition, gNormal, gAlbedoSpec;
		unsigned int rboDepth;


		std::vector<Mesh> _meshes;
		std::vector<glm::vec3> _meshes_poss;
		std::vector<glm::vec3> _meshes_scale;

		std::vector<Mesh> _mesh_strip;
		std::vector<glm::vec3> _mesh_strip_poss;

		Camera3D* _cam;

		GLSLProgram _shading_glsl;
		GLSLProgram _g_glsl;
		GLSLProgram _lightbox_glsl;
		GLSLProgram _skybox_glsl;
		GLSLProgram _basicglsl;
		
		unsigned int _skyboxtexture = 0;

		bool flighton = false;
		bool ownglsl = false;

		int _width;
		int _height;
	};


}