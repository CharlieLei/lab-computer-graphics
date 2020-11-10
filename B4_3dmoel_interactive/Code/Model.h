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

#include "tool/Shader.h"

class HalfEdge;
class Vertex;
class Face;
class Model;

class HalfEdge {
public:
    HalfEdge *pair, *next; // 当前半边所对应的另一个半边 围绕面f的下一条半边
    Vertex *v; // 边的终点
    Face *f;   // 当前半边方向的面
};

class Vertex {
public:
    HalfEdge *e; // 以当前点为终点的半边
    glm::vec3 position;
    glm::vec3 normal;
};

class Face {
public:
    HalfEdge *e; // 当前面的某一条半边
    std::vector<int> indexList; // 顶点下标的列表
};


class Model {
public:
    Model(std::string path);
    void Draw(Shader shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection,
              glm::vec3 color, glm::vec3 viewPos, glm::vec3 lightPos);

private:
    std::vector<Face*> modelFaces;
    std::vector<Vertex*> modelVertices;
    std::vector<HalfEdge*> modelHalfEdges;
    unsigned int VAO, VBO, EBO;
    int indicesSize;

    bool loadObjFile(std::string &path, std::vector<Vertex*>& vs, std::vector<Face*>& fs);
    void constructHalfEdges(std::vector<Vertex*> vs, std::vector<Face*> fs);
    void computeNormals();
    void setupBuffers();

    /**
     * 计算由半边e所对应面的法向量，按面的面积加权
     * @param e
     * @return
     */
    glm::vec3 getWeightedNormalOfFaceOnTheSideOfThisHalfEdge(HalfEdge *e);
};


#endif //INC_3_3DMODEL_MODEL_H
