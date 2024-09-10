#include "../include/GL/glew.h"
#include "../include/GLFW/glfw3.h"
#include "Utils.h"

#pragma comment(lib, "../lib/glew32.lib")
#pragma comment(lib, "../lib/glfw3.lib")
#pragma comment(lib, "../lib/SOIL.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(include, "../include") 

#include <iostream>



#define numVAOs 1
#define numVBOs 2
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLuint program;

GLfloat cameraX, cameraY,cameraZ;
GLfloat cubeLocX, cubeLocY, cubeLocZ;

glm::mat4 mv_matrix;
glm::mat4 proj_matrix;

int width, height;
float aspect;

void setupVertices() 
{
	float vertexPositions[108] = {
		-1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
	};

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

}


void init(GLFWwindow *window)
{
	program = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;
	cubeLocX = 0.0f; cubeLocY = -2.0; cubeLocZ = 0.0f;
	setupVertices();
}

void display(GLFWwindow* window, double time)
{
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	GLuint mvLoc = glGetUniformLocation(program, "mv_matrix");
	GLuint projLoc = glGetUniformLocation(program, "proj_matrix");

	glm::mat4 vMat= glm::translate(glm::mat4(1.0), glm::vec3(-cameraX, -cameraY, -cameraZ));
	glm::mat4 mMat = glm::translate(glm::mat4(1.0), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));
	mv_matrix = vMat * mMat;
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv_matrix));

	glfwGetFramebufferSize(window, &width, &height);
	aspect = width / height;
	proj_matrix = glm::perspective(60.0f, aspect, 0.1f, 1000.0f);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj_matrix));

	glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

int main()
{
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello OpenGL",NULL, NULL);
	glfwMakeContextCurrent(window); 

	if (glewInit() != GLEW_OK)
		exit(EXIT_FAILURE);

	glfwSwapInterval(1);

	init(window);

	while (!glfwWindowShouldClose(window))
	{
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}