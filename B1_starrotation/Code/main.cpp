#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "tool/Shader.h"
#include "tool/Texture.h"
#include "Star.h"

using namespace std;

void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Spiral_Type spiralType = ARCHIMEDEAN;
const int STAR_NUM = 50;

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "B1.StarRotation", nullptr, nullptr);
    if (window == nullptr) {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        cerr << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // 启用混色
    // -------
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); // 设置混色函数取得半透明效果
    glEnable(GL_BLEND);

    // build and compile shaders
    // -------------------------
    Shader shader("../Code/vertex.glsl", "../Code/fragment.glsl");

    // texture
    // -------
    Texture texture("../Code/Star.bmp");

    // model of star
    // ----------------------
    Star star[STAR_NUM];
    float spin = 0.0;
    bool isFermatRadiusNeg = false;
    for (int i = 0; i < STAR_NUM; i++) {
        float r = (rand() % 256) / 256.0; //rand() % 256;
        float g = (rand() % 256) / 256.0; //rand() % 256;
        float b = (rand() % 256) / 256.0; //rand()% 256;
        star[i].spin = spin;
        star[i].Color = glm::vec3(r, g, b);
        star[i].isFermatRadiusNeg = isFermatRadiusNeg;

        spin += 0.30; // glm::radians(20.0);
        isFermatRadiusNeg = !isFermatRadiusNeg;
    }


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        float angle = glm::radians((float) glfwGetTime() * 50.0);

        shader.use();
        texture.use();
        for (int i = 0; i < STAR_NUM; i++) {
            glm::mat4 transform(1.0);
            transform = glm::rotate(transform, angle, glm::vec3(0.0, 0.0, 1.0));
            transform = glm::translate(transform, star[i].GetPosition());
            transform = glm::rotate(transform, star[i].spin, glm::vec3(0.0, 0.0, 1.0));
            transform = glm::scale(transform, glm::vec3(0.1, 0.1, 0.1));
            shader.setMat4("transform", transform);
            shader.setVec3("Color", star[i].Color);

            star[i].SetSpiralType(spiralType);

            star[i].Draw();
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
        spiralType = ARCHIMEDEAN;
    }
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
        spiralType = LOGARITHMIC;
    }
    if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {
        spiralType = FERMAT;
    }
}