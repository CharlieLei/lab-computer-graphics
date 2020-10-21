//
// Created by 01 on 2020/10/21.
//
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "Skybox.h"
#include "stb_image.h"
#include "Texture.h"

Skybox::Skybox() {
    setupVertices();
    setupWaveVertices();

    for (int i = 0; i < FACES; i++)
        IDs[i] = Texture::loadTexture(paths[i].c_str(), GL_CLAMP_TO_EDGE);
    waveID = Texture::loadTexture(wavePath.c_str(), GL_REPEAT);
}

void Skybox::setTexture(Shader &shader) {
    shader.use();
    shader.setInt("skybox", 0);
}

void Skybox::DrawSky(Shader &shader, Camera &camera) {
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

void Skybox::DrawWave(Shader &shader, Camera &camera) {
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

void Skybox::DrawReflection(Shader &shader, Camera &camera) {
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

void Skybox::setupVertices() {
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

void Skybox::setupWaveVertices() {
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









