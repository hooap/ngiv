#include "ModelLoader.h"
#include "TextureLoader.h"
#include "Misc.h"
#include <map>

namespace ngiv
{


OBJ* ModelLoader::loadModel(std::string name, glm::vec3 pos, std::string path, bool setPosOnCenter, glm::vec3 scale)
{
    static std::map<std::string, OBJ*> model_container;
    if (model_container.find(path) != model_container.end())
    {
        OBJ* newmodel = new OBJ();
        OBJ* loadedmodel = model_container.find(path)->second;

        newmodel->meshes = loadedmodel->meshes;
        newmodel->name = name;
        newmodel->center_of_mass = loadedmodel->center_of_mass;
        newmodel->empty = false;
        newmodel->filepath = path;
        newmodel->scale = scale;
        newmodel->directory = path.substr(0, path.find_last_of('/'));

        if (setPosOnCenter)
        {
            newmodel->pos = pos - newmodel->center_of_mass;
        }
        else
        {
            newmodel->pos = pos;
        }

        return newmodel;
    }

    OBJ* model = new OBJ();
    model->scale = scale;
    model->empty = false;
    model->filepath = path;
    model->name = name;
    Assimp::Importer importer;
    model->meshes = new std::vector<Mesh>();

    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::string m = importer.GetErrorString();
        error("ERROR::ASSIMP::" + m, false);
        return nullptr;
    }
    model->directory = path.substr(0, path.find_last_of('/'));
    if (path == model->directory)
    {
        model->directory = path.substr(0, path.find_last_of('\\'));
    }


    processNode(model,scene->mRootNode, scene);

    //Calculate a good enough center of mass
    float x_max = -1,x_min = -1;
    float y_max = -1, y_min = -1;
    float z_max = -1, z_min = -1;

    bool first = true;
    for (int i = 0; i < model->meshes->size(); i++)
    {
        std::vector<Mesh>* v = model->meshes;
        for (int j = 0; j < (*v)[i].vertices.size(); j++)
        {
            Vertex3D* c = &(*v)[i].vertices[j];
            if (first)
            {
                first = false;
                x_max = c->Position.x;
                x_min = c->Position.x;
                y_max = c->Position.y;
                y_min = c->Position.y;
                z_max = c->Position.z;
                z_min = c->Position.z;
                continue;
            }
            if (c->Position.x > x_max)
            {
                x_max = c->Position.x;
            }
            if (c->Position.x < x_min)
            {
                x_min = c->Position.x;
            }
            if (c->Position.y > y_max)
            {
                y_max = c->Position.y;
            }
            if (c->Position.y < y_min)
            {
                y_min = c->Position.y;
            }
            if (c->Position.z > z_max)
            {
                z_max = c->Position.z;
            }
            if (c->Position.z < z_min)
            {
                z_min = c->Position.z;
            }
        }
    }
    model->xdif = (x_max - x_min);
    model->ydif = (y_max - y_min);
    model->zdif = (z_max - z_min);
    model->center_of_mass = glm::vec3((x_max + x_min) / 2.0f, (y_max + y_min) / 2.0f, (z_max + z_min) / 2.0f);
    model->center_of_mass *= model->scale;

    if (setPosOnCenter)
    {
        model->pos = pos - model->center_of_mass;
    }
    else
    {
        model->pos = pos;
    }

    importer.FreeScene();
    model_container.insert(std::make_pair(path, model));
    return model;
}

void ModelLoader::processNode(OBJ* o, aiNode *node, const aiScene *scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        o->meshes->push_back(processMesh(o, mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(o,node->mChildren[i], scene);
    }
}

Mesh ModelLoader::processMesh(OBJ* o, aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex3D> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex3D vertex;

        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
        }
        else
        {
            error("NGIV::MODEL::HASNONORMALS::NOSUPPORT", true);
        }

        vertex.Normal = vector;


        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;


            vec.y = mesh->mTextureCoords[0][i].y;

            vertex.TexCoords = vec;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }


    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps = loadMaterialTextures(o,material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        std::vector<Texture> specularMaps = loadMaterialTextures(o, material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }


    return Mesh(vertices, indices, textures);
}

std::vector<Texture> ModelLoader::loadMaterialTextures(OBJ* o, aiMaterial *mat, aiTextureType type,
        std::string typeName)
{

    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        Texture texture;
        texture.id = TextureLoader::LoadTexture(o->directory + "/" + str.C_Str());
        texture.type = typeName;
        texture.path = str;
        textures.push_back(texture);
    }
    return textures;

}


}
