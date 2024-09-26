#include "../include/GL/glew.h"
#include "../include/GLFW/glfw3.h"
#include "Utils.h"

#pragma comment(lib, "../lib/glew32.lib")
#pragma comment(lib, "../lib/glfw3.lib")
#pragma comment(lib, "../lib/SOIL.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "../lib/assimp-vc143-mt.lib")

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "FAILED TO CREATE GLWINDOW" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewInit();

	Model model("./nanosuit/nanosuit.obj");
	//Model model("./Tree/Tree.obj");

	//Model model("./free-polestar-1/source/Polestar1_Final01.fbx");
	Shader shader("vertShader.glsl", "fragShader.glsl");
	Camera camera;

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		shader.setMat4("mv_matrix", camera.viewMatrix());
		glm::mat4 projection = glm::perspective(75.0f, (float)800.0 / (float)600.0, (float)0.1, (float)100.0);
		shader.setMat4("proj_matrix", projection);
		model.draw(shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}