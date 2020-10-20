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
    unsigned int terrainTextureID;

    Terrain() {
        // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
        stbi_set_flip_vertically_on_load(true);
        setupVertices();
        terrainTextureID = loadTexture(terrainPath, GL_CLAMP_TO_EDGE);
    }

    void setTexture(Shader &shader) {
        shader.use();
        shader.setInt("heightTexture", 0);
        shader.setInt("terrainTexture", 1);
    }

    void Draw(Shader &shader, Camera &camera) {
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix();

        shader.use();
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, terrainTextureID);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

private:
    unsigned int VAO, VBO, EBO;

    const char *heightPath = "../texture/heightmap.bmp";
    const char *terrainPath = "../texture/terrain-texture3.bmp";

    std::vector<Vertex> vertices; // 每个像素需要6个点，每个点需要5个float值
    std::vector<unsigned int> indices;

    void setupVertices() {
        stbi_set_flip_vertically_on_load(true);
        unsigned char *heightmapData;
        int heightmapWidth, heightmapHeight, heightmapNrComponents;
        heightmapData = stbi_load(heightPath, &heightmapWidth, &heightmapHeight, &heightmapNrComponents, 1);
        if (heightmapData == nullptr) {
            std::cerr << "Texture failed to load at path: " << heightPath << std::endl;
            return;
        }

//        std::cout << "------------------" << std::endl;
//        for (int i = 0; i < heightmapHeight; i++) {
//            for (int j = 0; j < heightmapWidth; j++) {
//                std::cout << (int)heightmapData[i * heightmapHeight + j] << ", ";
//            }
//            std:: cout << std::endl;
//            std:: cout << std::endl;
//        }
//        std::cout << "------------------" << std::endl;
//        for (int y = 0; y < height; y++) {
//            for (int x = 0; x < width; x++) {
//                histogram[pixels[x + y * height]]++;
//            }
//        }

        float lengthInterval = LENGTH / (float) (LENGTH_VERTEX_NUM-1);
        float widthInterval = WIDTH / (float) (WIDTH_VERTEX_NUM-1);
        for (int i = 0; i < LENGTH_VERTEX_NUM; i++) {
            for (int j = 0; j < WIDTH_VERTEX_NUM; j++) {
                float x = lengthInterval * i, z = widthInterval * j;
                float u = x / (LENGTH), v = z / (WIDTH);
                int heightmap_u = u * (heightmapHeight-1), heightmap_v = v * (heightmapWidth-1);
                float y = heightmapData[heightmap_u * heightmapHeight + heightmap_v] / 256.0 * 1.0;

                Vertex vertex{glm::vec3(x, z, y), glm::vec2(u, v)};
                vertices.push_back(vertex);
            }
        }

        for (int i = 0; i < LENGTH_VERTEX_NUM-1; i++) {
            for (int j = 0; j < WIDTH_VERTEX_NUM-1; j++) {
                indices.push_back(j * (WIDTH_VERTEX_NUM) + i);
                indices.push_back((j+1) * (WIDTH_VERTEX_NUM) + i);
                indices.push_back((j+1) * (WIDTH_VERTEX_NUM) + (i+1));

                indices.push_back(j * (WIDTH_VERTEX_NUM) + i);
                indices.push_back(j * (WIDTH_VERTEX_NUM) + (i+1));
                indices.push_back((j+1) * (WIDTH_VERTEX_NUM) + (i+1));
            }
        }

//        std::cout << "-----------vertices--------------" << std::endl;
//        for (int i = 0; i < vertices.size(); i++) {
//            int heightmap_u = vertices[i].TexCoords.x * heightmapHeight, heightmap_v = vertices[i].TexCoords.y * heightmapWidth;
//            float y = heightmapData[heightmap_u * heightmapHeight + heightmap_v];
////            std::cout << i << " " << vertices[i].Position.x << " " << vertices[i].Position.y << " " << vertices[i].Position.z << " "
////                      << y << std::endl;
//            std::cout << i << " " << heightmap_u << " " << heightmap_v << " "
//                      << y << std::endl;
//        }
//        std::cout << "-----------indices---------------" << std::endl;
//        for (int i = 0; i < indices.size(); i++) {
//            if (i > 0 && i % 3 == 0) std::cout << std::endl;
//            if (i > 0 && i % 6 == 0) std::cout << std::endl;
//            std::cout << indices[i] << " ";
//        }

        stbi_image_free(heightmapData);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, TexCoords));
    }

    unsigned int loadTexture(const char *path, GLenum param) {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
        if (data) {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

            stbi_image_free(data);
        } else {
            std::cerr << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }
};

#endif //INC_6_TERRAINENGINE_TERRAIN_H
