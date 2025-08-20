#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

#include "../../OpenGL/Shader.h"
#include "Mesh.h"

class Model
{
public:
    Model(const char* path)
    {
        loadModel(path);
    }

    void Draw(
        Shader& shader,
        const glm::mat4& model
    );
private:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(
        aiMaterial *mat, 
        aiTextureType type, 
        std::string typeName
    );
};
