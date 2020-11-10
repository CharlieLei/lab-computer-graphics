//
// Created by 01 on 2020/10/13.
//

#include "Model.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

Model::Model(std::string path) {
    std::vector<Vertex *> vs;
    std::vector<Face *> fs;
    if (loadObjFile(path, vs, fs)) {
        constructHalfEdges(vs, fs);
    }
}

void Model::Draw(Shader shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection,
                 glm::vec3 color, glm::vec3 viewPos, glm::vec3 lightPos) {
    shader.use();

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    shader.setVec3("viewPos", viewPos);
    shader.setVec3("light.position", lightPos);
    shader.setVec3("light.intensity", 1.0, 1.0, 1.0);
    shader.setVec3("material.ambient", 0.1, 0.1, 0.1);
    shader.setVec3("material.diffuse", 0.5, 0.5, 0.5);
    shader.setVec3("material.specular", 1.0, 1.0, 1.0);
    shader.setFloat("material.shininess", 32.0);
    shader.setVec3("material.color", color);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glUseProgram(0);
}

bool Model::loadObjFile(std::string &path, std::vector<Vertex *> &vs, std::vector<Face *> &fs) {
    vs.clear();
    fs.clear();

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "ERROR::Model could not open obj file: " << path << std::endl;
        return false;
    }

    std::string line, keyword;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> keyword;
        if (keyword == "v") {
            //顶点
            Vertex *v = new Vertex();
            iss >> v->position.x >> v->position.y >> v->position.z;
            vs.push_back(v);
        } else if (keyword == "f") {
            // 面
            Face *f = new Face();
            int index;
            while (iss >> index) {
                f->indexList.push_back(index - 1);
            }
            fs.push_back(f);
        }
    }
    return true;
}

void Model::constructHalfEdges(std::vector<Vertex *> vs, std::vector<Face *> fs) {
    modelHalfEdges.clear();
    modelVertices = vs;
    modelFaces = fs;

    // 为了便于构建半边的对应的另一个半边
    std::map<std::pair<int, int>, HalfEdge *> edgePoints2HalfEdge;
    std::map<HalfEdge *, std::pair<int, int>> halfEdge2EdgePoints;

    for (int i = 0; i < modelFaces.size(); i++) {
        int currHalfEdgeSize = modelHalfEdges.size();     // 当前已经处理的半边个数
        int vertexSize = modelFaces[i]->indexList.size(); // 第i个面的顶点的下标个数

        // 处理第i个面
        for (int j = 0; j < vertexSize; j++) {
            HalfEdge *e = new HalfEdge();
            int v_j = modelFaces[i]->indexList[j]; // 第i个面的第j个顶点
            e->f = modelFaces[i];
            e->v = modelVertices[v_j];
            modelHalfEdges.push_back(e);

            modelFaces[i]->e = e;
            modelVertices[v_j]->e = e;

            // 找到当前半边所对应边的起点和终点
            int pre_j = (vertexSize + j - 1) % vertexSize; // 由于当前面中的所有半边围绕而成，所有前一个点就是对应边的起点
            int v_pre_j = modelFaces[i]->indexList[pre_j]; // 边的起点
            edgePoints2HalfEdge[std::make_pair(v_pre_j, v_j)] = e;       // v_pre_j -> v_j 对应的就是当前半边
            halfEdge2EdgePoints[e] = std::make_pair(v_pre_j, v_j);
        }

        // 将第i个面中的所有半边通过它们的next指针连起来
        for (int j = 0; j < vertexSize; j++) {
            modelHalfEdges[currHalfEdgeSize + j]->next = modelHalfEdges[currHalfEdgeSize + (j + 1) % vertexSize];
        }
    }

    for (int i = 0; i < modelHalfEdges.size(); i++) {
        // 当前半边所对应的（起点，终点）
        std::pair<int, int> vertexPair = halfEdge2EdgePoints[modelHalfEdges[i]];
        // 当前半边对应的另一个半边就是（终点，起点）所对应的边
        modelHalfEdges[i]->pair = edgePoints2HalfEdge[std::make_pair(vertexPair.second, vertexPair.first)];
    }

    computeNormals();
    setupBuffers();
}

void Model::computeNormals() {
    for (int i = 0; i < modelVertices.size(); i++) {
        // 以当前点为中心，找出其四周的面
        glm::vec3 normal = glm::vec3(0.0, 0.0, 0.0);
        HalfEdge *e0 = modelVertices[i]->e;

        HalfEdge *e = e0;
        if (e->pair == nullptr)
            continue;
        normal += getWeightedNormalOfFaceOnTheSideOfThisHalfEdge(e);
        if (e->next->pair == nullptr)
            continue;
        e = e->next->pair;

        while (e != e0) {
            normal += getWeightedNormalOfFaceOnTheSideOfThisHalfEdge(e);
            if (e->next->pair == nullptr)
                break;
            e = e->next->pair;
        }

        modelVertices[i]->normal = glm::normalize(normal);
    }
}

void Model::setupBuffers() {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i < modelVertices.size(); i++) {
        vertices.push_back(modelVertices[i]->position.x);
        vertices.push_back(modelVertices[i]->position.y);
        vertices.push_back(modelVertices[i]->position.z);
        vertices.push_back(modelVertices[i]->normal.x);
        vertices.push_back(modelVertices[i]->normal.y);
        vertices.push_back(modelVertices[i]->normal.z);
    }

    for (int i = 0; i < modelFaces.size(); i++) {
        for (int j = 0; j < modelFaces[i]->indexList.size(); j++) {
            indices.push_back(modelFaces[i]->indexList[j]);
        }
    }

    indicesSize = (int) indices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

glm::vec3 Model::getWeightedNormalOfFaceOnTheSideOfThisHalfEdge(HalfEdge *e) {
    glm::vec3 v0 = e->v->position;       // 边的终点
    glm::vec3 v1 = e->pair->v->position; // 边的起点
    glm::vec3 v2 = e->next->v->position; // 从终点出发的边，这一条新半边的终点

    double a = glm::length(v2 - v0);
    double b = glm::length(v1 - v0);
    double c = glm::length(v2 - v1);
    double s = (a + b + c) / 2.0;
    double area = sqrt(s * (s - a) * (s - b) * (s - c));

    return glm::vec3(area) * glm::cross(v2 - v0, v1 - v0);
}
