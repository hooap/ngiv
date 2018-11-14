#pragma once
#include "Renderer_3D.h"

namespace ngiv {


	class TerrainGenerator
	{
	public:
		TerrainGenerator();
		~TerrainGenerator();

		void init(int seed, float multiplier, int maxDistance);
		void create();
		
		void updateDrawPoss();
		void draw(Renderer_3D* rend);
		void update();


		glm::vec3 getAdditionalPos() { return pos; }
		std::vector<Mesh*> getMeshes() { return meshpointers; }
		
	private:

		void createSquareMeshWithHeight(int x, int z, glm::vec4 heights);
		void createMeshNormals(int width, int height);


		
		std::vector<std::vector<glm::vec3>> poss;
		std::vector<Mesh*> meshpointers;
		std::vector<std::vector<Mesh>> meshes;
		glm::vec3 pos;

		int _maxDistance;
		int _seed;
		float _multiplier;

	};


}