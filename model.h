#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stb_image.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "mesh.h"
#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;


class model {
    public:
    /*  成员函数   */
    Model(GLchar* path)
    {
        this->loadModel(path);
    }

    void Draw(Shader shader)
    {
        for(GLuint i = 0; i < this->meshes.size(); i++)
            this->meshes[i].Draw(shader);
    }

    private:
    /*  模型数据  */
    vector<Mesh> meshes;
    string directory;

    /*  私有成员函数   */
    void loadModel(string path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    }

    void processNode(aiNode* node, const aiScene* scene)

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)

    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)

};

#endif


