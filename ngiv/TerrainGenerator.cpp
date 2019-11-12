#include "TerrainGenerator.h"
#include <random>
#include "Noise.h"
#include "TextureLoader.h"

namespace ngiv {

	/*
		fvert.TexCoords = glm::vec2(0.0f);
		svert.TexCoords = glm::vec2(1.0f,0.0f);
		tvert.TexCoords = glm::vec2(1.0f);
		ffvert.TexCoords = glm::vec2(0.0f, 1.0f);
		*/

	TerrainGenerator::TerrainGenerator()
	{
	}

	TerrainGenerator::~TerrainGenerator()
	{
	}

	void TerrainGenerator::createmainmesh() {

		GLuint specid = TextureLoader::LoadTexture("container2_specular.png");
		GLuint textid = TextureLoader::LoadTexture("container2.png");

		//HACKY TEXTURE
		Texture mytexture;
		mytexture.id = textid;
		mytexture.type = "texture_diffuse";
		std::vector<Texture> textures;
		textures.push_back(mytexture);

		mytexture.id = specid;
		mytexture.type = "texture_specular";
		textures.push_back(mytexture);

		std::vector<Vertex3D_Instanced> vertics;
		std::vector<unsigned int> indics;

		//Create vertics
		Vertex3D_Instanced fvert, svert, tvert, ffvert;
		fvert.Position = glm::vec3(0, 0, 0);
		svert.Position = glm::vec3(meshsizeMultiplier, 0, 0);
		tvert.Position = glm::vec3(meshsizeMultiplier, 0, meshsizeMultiplier);
		ffvert.Position = glm::vec3(0, 0, meshsizeMultiplier);


		fvert.TexCoords = glm::vec2(1.0f, 0);
		svert.TexCoords = glm::vec2(0.0f);
		tvert.TexCoords = glm::vec2(0.0f, 1.0f);
		ffvert.TexCoords = glm::vec2(1.0f);

		fvert.index = 0;
		svert.index = 1;
		tvert.index = 2;
		ffvert.index = 3;


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


		Mesh_I newmesh(vertics, indics, textures);
		mesh = newmesh;
	}

	void TerrainGenerator::init(int seed, float multiplier, float meshsizeMultiplier,int maxDistance,const glm::vec3& pos_offset) {
		_maxDistance = maxDistance;
		_seed = seed;
		_multiplier = multiplier;
		this->meshsizeMultiplier = meshsizeMultiplier;
		createmainmesh();
        _pos = pos_offset;

	}


	void TerrainGenerator::createMeshPositionWithHeight(int x, int z, glm::vec4 heights) {

		float xmain = x * meshsizeMultiplier;
		float zmain = z * meshsizeMultiplier;

		heights *= meshsizeMultiplier;

		Instance_Offset_Data offset;
		offset.xzoffset.x = xmain + _pos.x;
		offset.xzoffset.y = zmain + _pos.z;

		heights -= meshsizeMultiplier / 2.0f;
		offset.y4offset = heights  * _multiplier;
		offset.y4offset += _pos.y;

		offsetpos.push_back(offset);
	}

	void TerrainGenerator::create(int octaves) {

		int width = _maxDistance;
		int height = _maxDistance;
		if (octaves == 0) {
			// we want most octaves
			octaves = log2((double)width);
		}

		std::vector<std::vector<float>>* noise = ngiv::Noise::get2DNoise(_seed, width, height, 2.0f, octaves);


		for (int i = 0; i < width-1; i++) {
			for (int j = 0; j < height-1; j++) {
				createMeshPositionWithHeight(i,j,glm::vec4((*noise)[i][j], (*noise)[i + 1][j], (*noise)[i+1][j+1], (*noise)[i][j+1]));
			}
		}

	}

	void TerrainGenerator::updateDrawPoss() {
		throw "update draw poss not implemented";
	}

	void TerrainGenerator::set_draw(Renderer_3D* rend) {
		rend->addtolist_drawMeshInstanced(mesh,offsetpos);
	}


	void TerrainGenerator::update() {


	}


}
