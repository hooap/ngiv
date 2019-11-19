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

	struct _OBJDATA {
        unsigned int mesh_size;
        unsigned int index_i;
        unsigned int index_loc_i;
        unsigned int index_start_loc;
        unsigned int index_end_loc;
	};



	class Renderer_3D
	{
	public:
		Renderer_3D();
		~Renderer_3D();

		void init(Camera3D* cam, int width, int height, GLSLProgram* glsl = nullptr);
		void loadSkybox(std::string path, std::vector<std::string> facenames = std::vector<std::string>());


		void redraw(OBJ* m, int id);
		int addtodraw(OBJ* m);



		void redrawCollisionBox(int id,Collision_Object* sp);
		int addtodrawCollisionBox(Collision_Object* sp);


		int addtodrawMeshInstanced(const Mesh_I& mesh, const std::vector<Instance_Offset_Data>& posoffsets);
		void redrawMeshInstanced(int id ,const Mesh_I& mesh, const std::vector<Instance_Offset_Data>& posoffsets);



		void render();
	private:

		void render_individual(const Mesh& mesh,const glm::mat4& model,unsigned int loc, GLSLProgram* glsl, int& indiccounter);
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


		///DATA
		//objdata
		std::vector<_OBJDATA> _database;

        std::vector<Mesh> _meshes;
        std::vector<glm::mat4> _mesh_models;
		std::vector<Vertex3D> _vertics;
		std::vector<unsigned int> _indics;
		std::vector<std::pair<unsigned int,unsigned int>> _locs;

		//dynamic instanced
        std::vector<Mesh_I> _meshes_instanced_data;
		std::vector<std::vector<Instance_Offset_Data>> _meshes_instanced_offsetpos;


        //collisionbox data
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
