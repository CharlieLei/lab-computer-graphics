#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Shader.h"

using namespace std;

void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "C1.Triangle", nullptr, nullptr);
	if (window == nullptr)
	{
		cerr << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cerr << "Failed to initialize GLAD" << endl;
		return -1;
	}

	// build and compile shaders
	// -------------------------
	Shader triangleShader("triangleVertex.glsl", "triangleFrag.glsl");
	Shader quadrangleShader("quadrangleVertex.glsl", "quadrangleFrag.glsl");

	// vertices of triangle
	// --------------------
	float triangle[] = {
		0.0, 0.4, 0.0,   1.0, 0.0, 0.0,
		-0.4, -0.5, 0.0, 0.0, 1.0, 0.0,
		0.2, 0.0, 0.0,   0.0, 0.0, 1.0
	};

	unsigned int triVBO, triVAO;
	glGenVertexArrays(1, &triVAO);
	glGenBuffers(1, &triVBO);

	glBindVertexArray(triVAO);
	glBindBuffer(GL_ARRAY_BUFFER, triVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// vertices of quadrangle
	// ----------------------
    float quadrangle[] = {
            -0.2f, -0.5f, 0.0f, 1.0, 0.0, 0.0,
            0.2f, -0.5f, 0.0f,  0.0, 0.5, 0.0,
            -0.2f, 0.5f, 0.0f,  0.0, 0.0, 1.0,
            0.2f, 0.5f, 0.0f,   1.0, 0.0, 0.0,
    };
    unsigned int quadIndices[] ={
            0, 1, 2,
            1, 2, 3
    };

	unsigned int quadVBO, quadVAO, quadEBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glGenBuffers(1, &quadEBO);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadrangle), quadrangle, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2, 0.3, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		float angle = (float)glfwGetTime() * 50.0;
		glm::mat4 triTrans(1.0);
		triTrans = glm::translate(triTrans, glm::vec3(0.5, 0.0, 0.0));
		triTrans = glm::rotate(triTrans, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
		triangleShader.setMat4("transform", triTrans);

		// draw triangle
		// -------------
		triangleShader.use();
		glBindVertexArray(triVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glm::mat4 quadTrans(1.0);
        quadTrans = glm::translate(quadTrans, glm::vec3(-0.5, 0.0, 0.0));
        quadTrans = glm::rotate(quadTrans, glm::radians(-angle), glm::vec3(0.0, 0.0, 1.0));
        quadrangleShader.setMat4("transform", quadTrans);

		// draw quadrangle
		// --------------
		quadrangleShader.use();
		glBindVertexArray(quadVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}