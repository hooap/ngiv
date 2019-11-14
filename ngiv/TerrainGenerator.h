#pragma once
#include "Renderer_3D.h"
#include "FastNoise.h"


namespace ngiv
{



class TerrainGenerator
{
public:
    TerrainGenerator();
    ~TerrainGenerator();

    void init(int seed, float multiplier, float meshsizeMultiplier,int chunksize, const glm::vec3& pos_offset);

    void draw(Renderer_3D* rend, int viewDistance, glm::vec3 camPos);


//		glm::vec3 getAdditionalPos() { return pos; }
    //std::vector<Mesh*> getMeshes() { return meshpointers; }

private:
    bool first_loop = true;
    void create(int viewDistance);


    glm::vec2 getChunkCoord(glm::vec3 pos);
    glm::vec3 _pos;
    glm::vec2 _viewPos;
    int _chunksize;


    void createmainmesh();
    void createMeshPositionWithHeight(int x, int z, glm::vec4 heights);


    std::vector<Instance_Offset_Data> offsetpos;
    Mesh_I _mainmesh;


    float meshsizeMultiplier = 1.0f;


	glm::vec3 pos;
    FastNoise _noise;
    int _seed;
    float _multiplier;

};


}
