#pragma once
#include <glm\glm.hpp>
#include "Mesh.h"
#include "OBJ.h"

namespace ngiv {

	class ModelLoader
	{
	public:

		static OBJ* loadModel(std::string name,glm::vec3 pos, std::string path, bool setPosOnCenter, glm::vec3 scale = glm::vec3(1));
	private:
		
		static void processNode(OBJ* o,aiNode *node, const aiScene *scene);
		static Mesh processMesh(OBJ* o, aiMesh *mesh, const aiScene *scene);
		static std::vector<Texture> loadMaterialTextures(OBJ* o, aiMaterial *mat, aiTextureType type,
			std::string typeName);
	};


}