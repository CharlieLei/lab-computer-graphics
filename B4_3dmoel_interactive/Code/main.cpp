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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 5.0f));

// rotation
float LastX;
float LastY;
bool FirstMouse = true;
glm::vec3 rotationVec(0.0);
double scaleFactor = 1.0;

// lighting
glm::vec3 lightPos(2.0, 2.0, 1.0);

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "B4.3D model interactive", nullptr, nullptr);
    if (window == nullptr) {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

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
    Shader shader("../Code/shader/vertex.glsl", "../Code/shader/fragment.glsl");

    // vertices of cubic
    // ----------------------
    Model aModel("../Code/object/eight.uniform.obj");


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
        model = glm::rotate(model, glm::radians(rotationVec.z), glm::vec3(0.0, 0.0, 1.0));
        model = glm::rotate(model, glm::radians(rotationVec.y), glm::vec3(0.0, 1.0, 0.0));
        model = glm::rotate(model, glm::radians(rotationVec.x), glm::vec3(1.0, 0.0, 0.0));
        model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix();

        // draw
        // -------------
        aModel.Draw(shader, model, view, projection, glm::vec3(1.0, 0.0, 0.0), camera.Position, lightPos);

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
    if (FirstMouse) {
        LastX = xpos;
        LastY = ypos;
        FirstMouse = false;
    }

    float xoffset = xpos - LastX;
    float yoffset = LastY - ypos; // reversed since y-coordinates go from bottom to top
    rotationVec += glm::vec3(-yoffset * 0.5, xoffset * 0.5, 0.0);

    LastX = xpos;
    LastY = ypos;
}

// glfw: 鼠标滚轮回调函数
// --------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    scaleFactor += yoffset * 0.1;
    scaleFactor = max(0.1, min(scaleFactor, 10.0)); // 设置最大最小缩小比例
}

// glfw: 修改窗口尺寸
// ----------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}