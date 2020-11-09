#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "tool/Shader.h"
#include "tool/Camera.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void processInput(GLFWwindow *window);

#define SQRT_TWO 1.414213562373095

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(SCR_WIDTH, SCR_HEIGHT,glm::vec3(-1.0, 1.0, 7.0));

// lighting
glm::vec3 lightPos(0.0, 2.0, 5.0);

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "C2.Pyramid", nullptr, nullptr);
    if (window == nullptr) {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        cerr << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader shader("../Code/vertex.glsl", "../Code/fragment.glsl");

    // vertices of pyramid
    // ----------------------
    float vertices[] = {
            0.0, SQRT_TWO, 0.0, 0.5, 0.5, 0.0, SQRT_TWO, 1.0, 0.0,
            1.0, 0.0, 1.0,      0.0, 0.5, 0.5, SQRT_TWO, 1.0, 0.0,
            1.0, 0.0, -1.0,     0.0, 1.0, 0.0, SQRT_TWO, 1.0, 0.0,

            0.0, SQRT_TWO, 0.0, 0.5, 0.5, 0.0, 0.0, 1.0, -SQRT_TWO,
            1.0, 0.0, -1.0,     0.0, 1.0, 0.0, 0.0, 1.0, -SQRT_TWO,
            -1.0, 0.0, -1.0,    0.5, 0.0, 0.5, 0.0, 1.0, -SQRT_TWO,

            0.0, SQRT_TWO, 0.0, 0.5, 0.5, 0.0, -SQRT_TWO, 1.0, 0.0,
            -1.0, 0.0, -1.0,    0.5, 0.0, 0.5, -SQRT_TWO, 1.0, 0.0,
            -1.0, 0.0, 1.0,     0.0, 0.0, 1.0, -SQRT_TWO, 1.0, 0.0,

            0.0, SQRT_TWO, 0.0, 0.5, 0.5, 0.0, 0.0, 1.0, SQRT_TWO,
            -1.0, 0.0, 1.0,     0.0, 0.0, 1.0, 0.0, 1.0, SQRT_TWO,
            1.0, 0.0, 1.0,      0.0, 0.5, 0.5, 0.0, 1.0, SQRT_TWO,

            1.0, 0.0, 1.0,      0.0, 0.5, 0.5, 0.0, -1.0, 0.0,
            1.0, 0.0, -1.0,     0.0, 1.0, 0.0, 0.0, -1.0, 0.0,
            -1.0, 0.0, -1.0,    0.5, 0.0, 0.5, 0.0, -1.0, 0.0,

            1.0, 0.0, 1.0,      0.0, 0.5, 0.5, 0.0, -1.0, 0.0,
            -1.0, 0.0, -1.0,    0.5, 0.0, 0.5, 0.0, -1.0, 0.0,
            -1.0, 0.0, 1.0,     0.0, 0.0, 1.0, 0.0, -1.0, 0.0,
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        Timer::refresh();

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model(1.0);
        model = glm::translate(model, glm::vec3(1.0, 2.0, 3.0));
        model = glm::translate(model, glm::vec3(0.0, -SQRT_TWO / 2.0, 0.0));
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix();

        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("viewPos", camera.Position);
        shader.setVec3("light.position", lightPos);
        shader.setVec3("light.intensity", 5.0, 5.0, 5.0);
        shader.setVec3("material.ambient", 0.2, 0.2, 0.2);
        shader.setVec3("material.diffuse",  0.8, 0.8, 0.8);
        shader.setVec3("material.specular", 1.0, 1.0, 1.0);
        shader.setFloat("material.shininess", 32.0);

        // draw pyramid
        // -------------
        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 18);

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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT);
}

// glfw: 鼠标移动回调函数
// --------------------
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    camera.ProcessMouseMovement(xpos, ypos, true);
}

// glfw: 修改窗口尺寸
// ----------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

