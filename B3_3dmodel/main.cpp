#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "tool/Shader.h"
#include "tool/Camera.h"
#include "Model.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 5.0f));

bool keys[1024];

bool showVertices = true;
bool showMeshes = true;
bool showFaces = true;

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "B3.3D_model", nullptr, nullptr);
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
    Shader shader("../shader/vertex.glsl", "../shader/fragment.glsl");

    // vertices of cubic
    // ----------------------
    Model aModel("../object/eight.uniform.obj");


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
        glClearColor(0.9, 0.9, 0.9, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model(1.0);
//        model = glm::scale(model, glm::vec3(0.1,0.1,0.1));
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix();

        // draw
        // -------------
        if (showVertices) {
            glEnable(GL_POLYGON_OFFSET_POINT);
            glPolygonOffset(-2, -2);
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            aModel.Draw(shader, model, view, projection, glm::vec3(0.0));
            glDisable(GL_POLYGON_OFFSET_POINT);
        }

        if (showMeshes) {
            glEnable(GL_POLYGON_OFFSET_LINE);
            glPolygonOffset(-1,-1);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            aModel.Draw(shader, model, view, projection, glm::vec3(0.0));
        }

        if (showFaces) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            aModel.Draw(shader, model, view, projection, glm::vec3(1.0, 0.0, 0.0));
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

    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
        showVertices = false;
        showMeshes = true;
        showFaces = false;
    }
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
        showVertices = true;
        showMeshes = false;
        showFaces = false;
    }
    if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {
        showVertices = false;
        showMeshes = false;
        showFaces = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS) {
        showVertices = false;
        showMeshes = true;
        showFaces = true;
    }

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