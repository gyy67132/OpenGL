#include "../include/GL/glew.h"
#include "../include/GLFW/glfw3.h"
#include "Utils.h"

#pragma comment(lib, "../lib/glew32.lib")
#pragma comment(lib, "../lib/glfw3.lib")
#pragma comment(lib, "../lib/SOIL.lib")
#pragma comment(lib, "opengl32.lib")



#include <iostream>

#define numVAOs 1
GLuint vao[numVAOs];
GLuint program;

void init(GLFWwindow *window)
{
	program = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double time)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	glClearColor(0.0, 0.0, 0.0, 1.0);

	glUseProgram(program);
	glDrawArrays(GL_POINTS, 0, 1);
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