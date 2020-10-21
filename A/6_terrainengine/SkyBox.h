//
// Created by 01 on 2020/10/19.
//

#ifndef INC_6_TERRAINENGINE_SKYBOX_H
#define INC_6_TERRAINENGINE_SKYBOX_H

#define DIMENSION 30
#define FACES 5
#define UP_FACE 4

#define LENGTH 50.0
#define WIDTH 60.0
#define HEIGHT 30.0

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"

class SkyBox {
public:
    unsigned int IDs[FACES]; //frontID, backID, leftID, rightID, UpID;
    unsigned int waveID;

    SkyBox() {
        setupVertices();
        setupWaveVertices();

        // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
        stbi_set_flip_vertically_on_load(true);
        for (int i = 0; i < FACES; i++)
            IDs[i] = loadTexture(paths[i], GL_CLAMP_TO_EDGE);
        waveID = loadTexture(wavePath, GL_REPEAT);
    }

    void setTexture(Shader &shader) {
        shader.use();
        shader.setInt("skybox", 0);
    }

    void DrawSky(Shader &shader, Camera &camera) {
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix();
        glm::vec2 texTranslate = glm::vec2(0.0f);

        shader.use();
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec2("texTranslate", texTranslate);

        for (int i = 0; i < FACES; i++) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, IDs[i]);
            glBindVertexArray(VAOs[i]);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    void DrawWave(Shader &shader, Camera &camera) {
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix();
        float dist = (float) glfwGetTime() * 0.1;
        glm::vec2 texTranslate = glm::vec2(dist, dist);

        shader.use();
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec2("texTranslate", texTranslate);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, waveID);
        glBindVertexArray(waveVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void DrawReflection(Shader &shader, Camera &camera) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1.0, -1.0, 1.0));
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix();
        glm::vec2 texTranslate = glm::vec2(0.0f);

        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec2("texTranslate", texTranslate);

        for (int i = 0; i < FACES; i++) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, IDs[i]);
            glBindVertexArray(VAOs[i]);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

private:
    unsigned int VAOs[FACES], VBOs[FACES];
    unsigned int waveVAO, waveVBO;

    std::vector<std::string> paths = {
            "../texture/SkyBox/SkyBox0.bmp",
            "../texture/SkyBox/SkyBox2.bmp",
            "../texture/SkyBox/SkyBox3.bmp",
            "../texture/SkyBox/SkyBox1.bmp",
            "../texture/SkyBox/SkyBox4.bmp"
    };

    std::string wavePath = "../texture/SkyBox/SkyBox5.bmp";

    float vertices[FACES][DIMENSION] = {
            // front
            {
                    LENGTH, 2 * HEIGHT, -WIDTH, 1.0, 1.0,
                    LENGTH, 0.0, -WIDTH, 1.0, 0.0,
                    -LENGTH, 0.0, -WIDTH, 0.0, 0.0,
                    -LENGTH, 0.0, -WIDTH, 0.0, 0.0,
                    -LENGTH, 2 * HEIGHT, -WIDTH, 0.0, 1.0,
                    LENGTH, 2 * HEIGHT, -WIDTH, 1.0, 1.0
            },
            // back
            {
                    -LENGTH, 2 * HEIGHT, WIDTH, 1.0, 1.0,
                    -LENGTH, 0.0, WIDTH, 1.0, 0.0,
                    LENGTH, 0.0, WIDTH, 0.0, 0.0,
                    LENGTH, 0.0, WIDTH, 0.0, 0.0,
                    LENGTH, 2 * HEIGHT, WIDTH, 0.0, 1.0,
                    -LENGTH, 2 * HEIGHT, WIDTH, 1.0, 1.0
            },
            // left
            {
                    -LENGTH, 2 * HEIGHT, -WIDTH, 1.0, 1.0,
                    -LENGTH, 0.0, -WIDTH, 1.0, 0.0,
                    -LENGTH, 0.0, WIDTH, 0.0, 0.0,
                    -LENGTH, 0.0, WIDTH, 0.0, 0.0,
                    -LENGTH, 2 * HEIGHT, WIDTH, 0.0, 1.0,
                    -LENGTH, 2 * HEIGHT, -WIDTH, 1.0, 1.0
            },
            // right
            {
                    LENGTH, 2 * HEIGHT, WIDTH, 1.0, 1.0,
                    LENGTH, 0.0, WIDTH, 1.0, 0.0,
                    LENGTH, 0.0, -WIDTH, 0.0, 0.0,
                    LENGTH, 0.0, -WIDTH, 0.0, 0.0,
                    LENGTH, 2 * HEIGHT, -WIDTH, 0.0, 1.0,
                    LENGTH, 2 * HEIGHT, WIDTH, 1.0, 1.0
            },
            // up
            {
                    LENGTH, 2 * HEIGHT, WIDTH, 1.0, 1.0,
                    LENGTH, 2 * HEIGHT, -WIDTH, 1.0, 0.0,
                    -LENGTH, 2 * HEIGHT, -WIDTH, 0.0, 0.0,
                    -LENGTH, 2 * HEIGHT, -WIDTH, 0.0, 0.0,
                    -LENGTH, 2 * HEIGHT, WIDTH, 0.0, 1.0,
                    LENGTH, 2 * HEIGHT, WIDTH, 1.0, 1.0
            },
    };

    float waveVertices[DIMENSION] = {
            LENGTH, 0.0, -WIDTH, 10.0, 10.0,
            LENGTH, 0.0, WIDTH, 10.0, 0.0,
            -LENGTH, 0.0, WIDTH, 0.0, 0.0,
            -LENGTH, 0.0, WIDTH, 0.0, 0.0,
            -LENGTH, 0.0, -WIDTH, 0.0, 10.0,
            LENGTH, 0.0, -WIDTH, 10.0, 10.0,
    };

    void setupVertices() {
        glGenVertexArrays(FACES, VAOs);
        glGenBuffers(FACES, VBOs);

        for (int i = 0; i < FACES; i++) {
            glBindVertexArray(VAOs[i]);
            glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[i]), vertices[i], GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) nullptr);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        }
    }

    void setupWaveVertices() {
        glGenVertexArrays(1, &waveVAO);
        glGenBuffers(1, &waveVBO);

        glBindVertexArray(waveVAO);
        glBindBuffer(GL_ARRAY_BUFFER, waveVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(waveVertices), waveVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    }

    unsigned int loadTexture(std::string &path, GLenum param) {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
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


#endif //INC_6_TERRAINENGINE_SKYBOX_H
