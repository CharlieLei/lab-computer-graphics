//
// Created by 01 on 2020/10/21.
//

#include <glm/gtc/matrix_transform.hpp>
#include "Terrain.h"
#include "Texture.h"

const float DOWN_OFFSET = 0.3;

Terrain::Terrain() {
    setupVertices();
    terrainTextureID = Texture::loadTexture(terrainPath, GL_CLAMP_TO_EDGE);
    detailTextureID = Texture::loadTexture(terrainDetailPath, GL_MIRRORED_REPEAT);
}

void Terrain::setTexture(Shader &shader) {
    shader.use();
    shader.setInt("terrainTexture", 0);
    shader.setInt("detailTexture", 1);
}

void Terrain::Draw(Shader &shader, Camera &camera) {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix();
    glm::vec4 clipPlane = glm::vec4(0.0, 1.0, 0.0, 0.0);

    shader.use();
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setVec4("clipPlane", clipPlane);

    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, terrainTextureID);
    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, detailTextureID);

//        glEnable(GL_CLIP_DISTANCE0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
//        glDisable(GL_CLIP_DISTANCE0);
}

void Terrain::DrawReflection(Shader &shader, Camera &camera) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0, DOWN_OFFSET + 0.5, 0.0));
    model = glm::scale(model, glm::vec3(1.0, -1.0, 1.0));
    model = glm::translate(model, glm::vec3(0.0, DOWN_OFFSET, 0.0));
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix();
    glm::vec4 clipPlane = glm::vec4(0.0, -1.0, 0.0, 0.0);

    shader.use();
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setVec4("clipPlane", clipPlane);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, terrainTextureID);
    glEnable(GL_CLIP_DISTANCE0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glDisable(GL_CLIP_DISTANCE0);
}

void Terrain::setupVertices() {
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

    float lengthInterval = LENGTH / (float) (LENGTH_VERTEX_NUM - 1);
    float widthInterval = WIDTH / (float) (WIDTH_VERTEX_NUM - 1);
    for (int i = 0; i < LENGTH_VERTEX_NUM; i++) {
        for (int j = 0; j < WIDTH_VERTEX_NUM; j++) {
            float x = lengthInterval * i, z = widthInterval * j;
            float u = x / (LENGTH), v = z / (WIDTH);
            int heightmap_u = u * (heightmapHeight - 1), heightmap_v = v * (heightmapWidth - 1);
            float y = heightmapData[heightmap_v * heightmapWidth + heightmap_u] / 256.0 * 1.0 - DOWN_OFFSET;

            Vertex vertex{glm::vec3(x, y, z), glm::vec2(u, v)};
            vertices.push_back(vertex);
        }
    }

    for (int i = 0; i < LENGTH_VERTEX_NUM - 1; i++) {
        for (int j = 0; j < WIDTH_VERTEX_NUM - 1; j++) {
            indices.push_back(j * (WIDTH_VERTEX_NUM) + i);
            indices.push_back((j + 1) * (WIDTH_VERTEX_NUM) + i);
            indices.push_back((j + 1) * (WIDTH_VERTEX_NUM) + (i + 1));

            indices.push_back(j * (WIDTH_VERTEX_NUM) + i);
            indices.push_back(j * (WIDTH_VERTEX_NUM) + (i + 1));
            indices.push_back((j + 1) * (WIDTH_VERTEX_NUM) + (i + 1));
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










