#include "TerrainGenerator.h"
#include <random>
#include "TextureLoader.h"

namespace ngiv {



	TerrainGenerator::TerrainGenerator()
	{
	}

	TerrainGenerator::~TerrainGenerator()
	{
	}

    void TerrainGenerator::init(int seed, float multiplier, float meshsizeMultiplier,int chunksize,const glm::vec3& pos_offset) {
		_seed = seed;
		_multiplier = multiplier;
		this->meshsizeMultiplier = meshsizeMultiplier;
		createmainmesh();
        _pos = pos_offset;
        _viewPos = glm::vec2(0);

        _chunksize = chunksize;
        _noise.SetNoiseType(FastNoise::Perlin);
        _noise.SetFrequency(0.05);
    //    _noise.SetFractalOctaves(8);
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
		_mainmesh = newmesh;
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




	void TerrainGenerator::draw(Renderer_3D* rend,int viewDistance, glm::vec3 camPos) {
        //check if render modification is needed
        glm::vec2 coord = getChunkCoord(camPos);

        if(first_loop){
            first_loop = false;
            _viewPos = coord;
            create(viewDistance);
            _rendid = rend->addtodrawMeshInstanced(_mainmesh,offsetpos);
            return;
        }


        if(coord != _viewPos){
            _viewPos = coord;
            create(viewDistance);
        }

        rend->redrawMeshInstanced(_rendid, _mainmesh,offsetpos);
	}


    glm::vec2 TerrainGenerator::getChunkCoord(glm::vec3 pos){
        int x = pos.x;
        int y = pos.z;


        return glm::vec2(x / _chunksize,y / _chunksize);
    }

    void TerrainGenerator::create(int viewDistance) {

        int x = _viewPos.x;
        int y = _viewPos.y;
        int min_x = (x - viewDistance) * _chunksize;
        int min_y = (y - viewDistance) * _chunksize;

        int max_x = (x + viewDistance) * _chunksize;
        int max_y = (y + viewDistance) * _chunksize;

        //TODO:
        // ADD SOME SORT OF CACHING
        offsetpos.clear();

		for (int i = min_x; i < max_x-1; i++) {
			for (int j = min_y; j < max_y-1; j++) {
                    createMeshPositionWithHeight(i,j,glm::vec4(_noise.GetNoise(i,j), _noise.GetNoise(i+1,j), _noise.GetNoise(i+1,j+1), _noise.GetNoise(i,j+1)));
			}
		}

	}



}

/*
LEGACY
//#include "Noise.h"


		fvert.TexCoords = glm::vec2(0.0f);
		svert.TexCoords = glm::vec2(1.0f,0.0f);
		tvert.TexCoords = glm::vec2(1.0f);
		ffvert.TexCoords = glm::vec2(0.0f, 1.0f);


		int width = _maxDistance;
		int height = _maxDistance;
		if (octaves == 0) {
			// we want most octaves
			octaves = log2((double)width);
		}


		std::vector<std::vector<float>>* noise = ngiv::Noise::get2DNoise(_seed, width, height, 2.0f, octaves);
				createMeshPositionWithHeight(i,j,glm::vec4((*noise)[i][j], (*noise)[i + 1][j], (*noise)[i+1][j+1], (*noise)[i][j+1]));

*/


