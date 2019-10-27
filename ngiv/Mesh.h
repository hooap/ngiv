#pragma once

#include <vector>
#include <glm\glm.hpp>
#include <string>
#include "GLSLProgram.h"
#include <assimp\Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <sstream>

namespace ngiv {
	struct Vertex3D
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct Vertex3D_Instanced
	{
		glm::vec3 Position;
		glm::vec2 TexCoords;
		float index;		
	};



	struct Texture {
		unsigned int id;
		std::string type;
		aiString path;
	};
	
	class Mesh
	{
	public:
		Mesh(std::vector<Vertex3D> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;		
		}

		Mesh() {}
		~Mesh(){
			dispose();
		}
		
		void dispose() {

		}
	
		void init(std::vector<Texture> textures, std::vector<unsigned int> indices, std::vector<Vertex3D> vertices) {
			this->textures = textures;
			this->indices = indices;
			this->vertices = vertices;
		}
			
		std::vector<Texture> textures;
		std::vector<unsigned int> indices;
		std::vector<Vertex3D> vertices;

		/*  Render data  */
		unsigned int VAO;
		unsigned int VBO, EBO;

					
	};


	class Mesh_I
	{
	public:
		Mesh_I(std::vector<Vertex3D_Instanced> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;
		}

		Mesh_I() {}
		~Mesh_I() {
			dispose();
		}

		void dispose() {

		}

		void init(std::vector<Texture> textures, std::vector<unsigned int> indices, std::vector<Vertex3D_Instanced> vertices) {
			this->textures = textures;
			this->indices = indices;
			this->vertices = vertices;
		}

		std::vector<Texture> textures;
		std::vector<unsigned int> indices;
		std::vector<Vertex3D_Instanced> vertices;

		/*  Render data  */
		unsigned int VAO;
		unsigned int VBO, EBO;


	};


}