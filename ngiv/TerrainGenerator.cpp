#include "TerrainGenerator.h"
#include <random>
#include "Noise.h"
#include "TextureLoader.h"

namespace ngiv {



	TerrainGenerator::TerrainGenerator()
	{
		pos = glm::vec3(0);
	}


	TerrainGenerator::~TerrainGenerator()
	{
	}


	void TerrainGenerator::init(int seed, float multiplier,int maxDistance) {
		_maxDistance = maxDistance;
		_seed = seed;
		_multiplier = multiplier;
	}

	void TerrainGenerator::createMeshNormals(int width,int height) {
		
		for (int x = 0; x < meshes.size(); x++) {
			for (int z = 0; z < meshes[x].size(); z++) {
				Mesh* m = &meshes[x][z];

				//in same mesh
				float h0 = m->vertices[0].Position.y;
				float h1 = m->vertices[1].Position.y;
				float h2 = m->vertices[2].Position.y;
				float h3 = m->vertices[3].Position.y;




				//3 normal
				float dif3 = h3 - h0;
				float dif2 = h2 - h0;
				float dif1 = h1 - h0;


				glm::vec3 percendular1 = -glm::normalize(glm::cross(glm::normalize(glm::vec3(1, dif1, 0)), glm::normalize(glm::vec3(1, dif2, 1))));
				glm::vec3 percendular3 = glm::normalize(glm::cross(glm::normalize(glm::vec3(0,dif3,1)), glm::normalize(glm::vec3(1,dif2,1))));




				
				m->vertices[1].Normal = percendular1;
				m->vertices[3].Normal = percendular3;

				glm::vec3 medium = percendular1 + percendular3;
				medium = glm::normalize(medium);
				m->vertices[0].Normal = medium;
				m->vertices[2].Normal = medium;





			}
		}
	}


	void TerrainGenerator::createSquareMeshWithHeight(int x, int z, glm::vec4 heights) {

		GLuint specid = TextureLoader::LoadTexture("container2_specular.png");
		GLuint textid = TextureLoader::LoadTexture("container2.png");


		heights *= _multiplier;

		//HACKY TEXTURE
		Texture mytexture;
		mytexture.id = textid;
		mytexture.type = "texture_diffuse";
		std::vector<Texture> textures;
		textures.push_back(mytexture);

		mytexture.id = specid;
		mytexture.type = "texture_specular";
		textures.push_back(mytexture);
			
				
		std::vector<Vertex3D> vertics;
		std::vector<unsigned int> indics;

		//Create vertics
		Vertex3D fvert, svert, tvert, ffvert;
		fvert.Position = glm::vec3(0, heights.x, 0);
		svert.Position = glm::vec3(1, heights.y, 0);
		tvert.Position = glm::vec3(1, heights.z, 1);
		ffvert.Position = glm::vec3(0, heights.w, 1);

		/*
		fvert.TexCoords = glm::vec2(0.0f);
		svert.TexCoords = glm::vec2(1.0f,0.0f);
		tvert.TexCoords = glm::vec2(1.0f);
		ffvert.TexCoords = glm::vec2(0.0f, 1.0f);
		*/
		fvert.TexCoords = glm::vec2(1.0f, 0);
		svert.TexCoords = glm::vec2(0.0f);
		tvert.TexCoords = glm::vec2(0.0f, 1.0f);
		ffvert.TexCoords = glm::vec2(1.0f);


		vertics.push_back(fvert);
		vertics.push_back(svert);
		vertics.push_back(tvert);
		vertics.push_back(ffvert);
		//Create indics
		unsigned int i0 = 0;
		unsigned int i1 = 1;
		unsigned int i2 = 2;
		unsigned int i3 = 3;

		indics.push_back(i0);
		indics.push_back(i1);
		indics.push_back(i2);
		indics.push_back(i0);
		indics.push_back(i2);
		indics.push_back(i3);


		Mesh mesh(vertics, indics, textures);
		meshes.back().push_back(mesh);
		meshpointers.push_back(&meshes.back().back());

		
		return;
	}

	void TerrainGenerator::create() {

		int width = 32;
		int height = 32;
		std::vector<std::vector<float>>* noise = ngiv::Noise::get2DNoise(_seed, width, height, 1.5f, 6);
		

		for (int i = 0; i < width-1; i++) {	
			meshes.emplace_back();
			for (int j = 0; j < height-1; j++) {
				createSquareMeshWithHeight(i,j,glm::vec4((*noise)[i][j], (*noise)[i + 1][j], (*noise)[i+1][j+1], (*noise)[i][j+1]));
			}
		}			
		
	//	meshes.emplace_back();
	//	createSquareMeshWithHeights(0, 0, glm::vec4(0,0,1,0));
	//	createSquareMeshWithHeights(0, 1, glm::vec4(-3, -3, -3, -3));

		createMeshNormals(width,height);		
		updateDrawPoss();

	}

	void TerrainGenerator::updateDrawPoss() {
		//create poss
		poss.clear();
		for (int i = 0; i < meshes.size(); i++) {
			poss.emplace_back();
			for (int j = 0; j < meshes[i].size(); j++) {
				poss.back().push_back(glm::vec3(i, 0, j) + pos);
			}
		}

	}

	void TerrainGenerator::draw(Renderer_3D* rend) {				
		rend->drawMultipleMesh(meshes,poss,glm::vec3(1.0f),true);
	}


	void TerrainGenerator::update() {


	}


}