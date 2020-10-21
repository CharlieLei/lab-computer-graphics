//
// Created by leich on 2020/10/19.
//

#ifndef INC_6_TERRAINENGINE_TERRAIN_H
#define INC_6_TERRAINENGINE_TERRAIN_H

#include <glad/glad.h>
#include <iostream>
#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"

#define DIMENSION 30

#define LENGTH 10.0 // x axis
#define WIDTH 10.0  // z axis
#define LENGTH_VERTEX_NUM 256
#define WIDTH_VERTEX_NUM 256

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoords;
};

class Terrain {
public:
    unsigned int terrainTextureID, detailTextureID;

    Terrain();

    void setTexture(Shader &shader);

    void Draw(Shader &shader, Camera &camera);

    void DrawReflection(Shader &shader, Camera &camera);

private:
    unsigned int VAO, VBO, EBO;

    const char *heightPath = "../texture/heightmap.bmp";
    const char *terrainPath = "../texture/terrain-texture3.bmp";
    const char *terrainDetailPath = "../texture/detail.bmp";

    std::vector<Vertex> vertices; // 每个像素需要6个点，每个点需要5个float值
    std::vector<unsigned int> indices;

    void setupVertices();
};

#endif //INC_6_TERRAINENGINE_TERRAIN_H
