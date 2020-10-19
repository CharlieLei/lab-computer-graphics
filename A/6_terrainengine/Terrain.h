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

#define LENGTH 10.0
#define WIDTH 10.0

class Terrain {
public:
    unsigned int heightTextureID, terrainTextureID;

    Terrain() {
        setupVertices();
        // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
        stbi_set_flip_vertically_on_load(true);
        heightTextureID = loadTexture(heightPath, GL_CLAMP_TO_EDGE);
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

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, heightTextureID);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, terrainTextureID);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

private:
    unsigned int VAO, VBO;

    const char *heightPath = "../texture/heightmap.bmp";
    const char *terrainPath = "../texture/terrain-texture3.bmp";

    float vertices[DIMENSION] = {
        -LENGTH, 0.0, WIDTH, 0.0, 0.0,
        LENGTH, 0.0, WIDTH, 1.0, 0.0,
        LENGTH, 0.0, -WIDTH, 1.0, 1.0,
        -LENGTH, 0.0, WIDTH, 0.0, 0.0,
        -LENGTH, 0.0, -WIDTH, 0.0, 1.0,
        LENGTH, 0.0, -WIDTH, 1.0, 1.0
    };

    void setupVertices() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
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
