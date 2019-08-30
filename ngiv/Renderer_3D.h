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

		void redraw_static();
		int draw(OBJ* m, bool isstatic);
		void drawCollisionBox(Collision_Object* sp);
		void drawMultipleMesh(const std::vector<std::vector<Mesh>>& meshes, const std::vector<std::vector<glm::vec3>>& poss, glm::vec3 scale, bool isstatic);


		void render();
	private:

		void render_individual(const Mesh& mesh,const glm::mat4& model, int loc, GLSLProgram* glsl, int& indiccounter);
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


		std::vector<Vertex3D> static_vertics;
		std::vector<unsigned int> static_indics;
		std::vector<unsigned int> static_locs;
		unsigned int static_last_loc;


		std::vector<Mesh> _meshes_static_data;
		std::vector<glm::mat4> _meshes_static_model;
		std::vector<unsigned int> _meshes_static_id;
		
		std::vector<Mesh> _meshes_dynamic_data;
		std::vector<glm::mat4> _meshes_dynamic_model;
		std::vector<unsigned int> _meshes_dynamic_id;


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