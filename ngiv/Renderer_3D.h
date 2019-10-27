#pragma once
#include "Camera3D.h"
#include "GLSLProgram.h"
#include "OBJ.h"

#include <vector>
#include "Collision_Box.h"

namespace ngiv {	

	struct Instance_Offset_Data {
		glm::vec4 y4offset;
		glm::vec2 xzoffset;
	};



	class Renderer_3D
	{
	public:
		Renderer_3D();
		~Renderer_3D();

		void init(Camera3D* cam, int width, int height, GLSLProgram* glsl = nullptr);
		void loadSkybox(std::string path, std::vector<std::string> facenames = std::vector<std::string>());

		void redraw_static();


		void draw(OBJ* m);
		int addtolist_draw(OBJ* m, bool isstatic);
		

		void drawCollisionBox(Collision_Object* sp);
		int addtolist_drawCollisionBox(Collision_Object* sp);

		
	//	void drawMultipleMesh(const std::vector<std::vector<Mesh>>& meshes, const std::vector<glm::vec3>& posoffsets,glm::vec3 scale = glm::vec3(1));


		int addtolist_drawMeshInstanced(const Mesh_I& meshes, const std::vector<Instance_Offset_Data>& posoffsets);



		void render();
	private:

		void render_individual(const Mesh& mesh,const glm::mat4& model, int loc, GLSLProgram* glsl, int& indiccounter);
		bool initialized = false;


		void renderWithGLSL(GLSLProgram& glsl);
		void renderWithGLSLinstanced(GLSLProgram& glsl);


		void renderStrips(GLSLProgram& glsl);

		void dispose();
		GLuint VAO, VBO, EBO;	
		GLuint VBO_INS_I;
		GLuint VBO_INS_D;

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
		std::vector<unsigned int> _meshes_static_index;
		
		std::vector<Mesh> _meshes_dynamic_data;
		std::vector<glm::mat4> _meshes_dynamic_model;
		std::vector<unsigned int> _meshes_dynamic_index;

		//static instanced
		std::vector<Mesh_I> _static_meshes_instanced_data;
		std::vector<std::vector<Instance_Offset_Data>> _static_meshes_instanced_offsetpos;


		std::vector<Mesh> _mesh_strip;
		std::vector<glm::vec3> _mesh_strip_poss;

		Camera3D* _cam;

		//MAIN GLSL
		GLSLProgram _g_glsl;
		GLSLProgram _g_glsl_instanced;


		//utils and coloring
		GLSLProgram _shading_glsl;
		GLSLProgram _lightbox_glsl;
		GLSLProgram _skybox_glsl;
		
		unsigned int _skyboxtexture = 0;

		bool flighton = false;
		bool ownglsl = false;

		int _width;
		int _height;
	};




}