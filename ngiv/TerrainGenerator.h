#pragma once
#include "Renderer_3D.h"

namespace ngiv
{



class TerrainGenerator
{
public:
    TerrainGenerator();
    ~TerrainGenerator();

    void init(int seed, float multiplier, float meshsizeMultiplier, int maxDistance,const glm::vec3& pos_offset);
    void create(int octaves = 0);

    void updateDrawPoss();
    void set_draw(Renderer_3D* rend);
    void update();


//		glm::vec3 getAdditionalPos() { return pos; }
    //std::vector<Mesh*> getMeshes() { return meshpointers; }

private:
    glm::vec3 _pos;

    void createmainmesh();
    void createMeshPositionWithHeight(int x, int z, glm::vec4 heights);


    std::vector<Instance_Offset_Data> offsetpos;
    Mesh_I mesh;


    float meshsizeMultiplier = 1.0f;

//		glm::vec3 pos;

    int _maxDistance;
    int _seed;
    float _multiplier;

};


}
