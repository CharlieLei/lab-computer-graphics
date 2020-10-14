//
// Created by 01 on 2020/10/13.
//

#include "Model.h"

Model::Model(std::string path) {
    loadObjFile(path);
    setupMesh();
}

void Model::Draw(Shader shader) {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Model::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
//    glEnableVertexAttribArray(2);
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

bool Model::loadObjFile(std::string &path) {
    // 根据后辍判断是否为obj文件
    if (path.substr(path.size() - 4, 4) != ".obj")
        return false;

    std::ifstream file(path);

    if (!file.is_open())
        return false;

    std::vector<Vertex> aVertices;
    std::vector<unsigned int> aIndices;

    std::string currLine, keyword;
    while(std::getline(file, currLine)) {
        std::istringstream iss(currLine);
        iss >> keyword;
        if (keyword == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            Vertex vertex{};
            vertex.Position = glm::vec3(x, y, z);
            vertex.Normal = glm::vec3(0.0, 0.0, 0.0);
            aVertices.push_back(vertex);
        }
        if (keyword == "f") {
            int a, b, c; // obj文件中面的下标从1开始
            iss >> a >> b >> c;
            aIndices.push_back(a-1);
            aIndices.push_back(b-1);
            aIndices.push_back(c-1);
        }
    }

    std::vector<glm::vec3> aFaceNormals;
    for (int i = 0; i < aIndices.size(); i += 3) {
        int v0 = aIndices[i], v1 = aIndices[i+1], v2 = aIndices[i+2];
        glm::vec3 edge1 = aVertices[v1].Position - aVertices[v0].Position;
        glm::vec3 edge2 = aVertices[v2].Position - aVertices[v1].Position;
        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
        aFaceNormals.push_back(normal);
    }

    for (int i = 0; i < aIndices.size(); i++) {
        int f = i / 3;
        int v = aIndices[i];
        aVertices[v].Normal += aFaceNormals[f];
    }

    for (int i = 0; i < aVertices.size(); i++) {
        aVertices[i].Normal = glm::normalize(aVertices[i].Normal);
    }

    this->vertices = aVertices;
    this->indices = aIndices;

    return true;
}
