//
// Created by 01 on 2020/10/13.
//

#ifndef INC_3_3DMODEL_MODEL_H
#define INC_3_3DMODEL_MODEL_H

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
};


class Model {
public:
    std::vector<Vertex> vertices; // 节点列表
    std::vector<unsigned int> indices; // 节点下标列表

    Model(std::string path);
    void Draw(Shader shader);

private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
    bool loadObjFile(std::string &path);
};


#endif //INC_3_3DMODEL_MODEL_H
