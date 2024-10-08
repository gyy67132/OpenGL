#include "ResourceManager.h"
#include "Game.h"

#include "Shader.h"

#pragma comment(lib, "../lib/glew32.lib")
#pragma comment(lib, "../lib/glfw3.lib")
#pragma comment(lib, "../lib/SOIL.lib")
#pragma comment(lib, "opengl32.lib")

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

Game Breakout(800, 600);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(800, 600, "BreakOut", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewInit();
	glGetError();

	glfwSetKeyCallback(window, key_callback);

	glViewport(0, 0, 800, 600);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Breakout.Init();
	Breakout.State = GAME_ACTIVE;

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		Breakout.ProcessInput(deltaTime);

		Breakout.Update(deltaTime);

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Breakout.Render();

		glfwSwapBuffers(window);
	}

	ResourceManager::Clear();
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Breakout.Keys[key] = GL_TRUE;
		else if(action == GLFW_RELEASE)
			Breakout.Keys[key] = GL_FALSE;
	}
}