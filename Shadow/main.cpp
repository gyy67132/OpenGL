#include "../include/GL/glew.h"
#include "../include/GLFW/glfw3.h"
#include "../include/SOIL2/stb_image.h"

#include <map>

#include "Shader.h"
#include "Camera.h"

#pragma comment(lib, "../lib/glew32.lib")
#pragma comment(lib, "../lib/glfw3.lib")
#pragma comment(lib, "../lib/SOIL.lib")
#pragma comment(lib, "../lib/stb_image.lib")
#pragma comment(lib, "../lib/freetype.lib")
#pragma comment(lib, "opengl32.lib") 

GLuint loadTexture(const char* path);
void processInput(GLFWwindow* window);
void renderCube();
void renderScene(Shader& shader);
void showFramebufferError(GLenum status);

Camera camera;
bool blinnKeyPressed = false;
bool blinn = false;

GLuint cubeVAO = -1;
GLuint planeVAO = -1;

int main()
{
	/*
	* GLFW库 窗口管理库，事件，鼠标
	*/
	if (!glfwInit())
		exit(EXIT_FAILURE);

	/*
	* 指定计算机必须与OpenGL版本4.3兼容
	*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/*
	* 创建GLFW窗口，同时创建OpenGL上下文,上下文指的是OpenGL实例及其状态信息，其中包括颜色缓冲区等项
	*/
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello OpenGL", NULL, NULL);
	/*
	* GLFW创建的窗口并不会与OpenGL上下文关联，必须用次函数
	*/
	glfwMakeContextCurrent(window);

	/*
	* OpenGL API
	*/
	if (glewInit() != GLEW_OK)
		exit(EXIT_FAILURE);

	
	//开启垂直同步
	//glfwSwapInterval(1);

	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 800, 600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::mat4 projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
	glm::mat4 view = camera.viewMatrix();

	Shader shader("vertShader.glsl", "fragShader.glsl");
	shader.use();
	shader.setInt("floorTexture", 0);
	shader.setInt("depthMapTexture", 1);
	
	//shader.setInt("floorTexture", 0);
	Shader debugShader("debugVertShader.glsl", "debugFragShader.glsl");
	
	GLfloat vertices[] = {
		-0.9, -0.9, 0.0,0.0,
		0.9, -0.9, 1.0,0.0,
		-0.9, 0.9, 0.0,1.0,

		0.9, -0.9, 1.0,0.0,
		0.9, 0.9, 1.0,1.0,
		-0.9, 0.9, 0.0,1.0
	};
	GLuint debugVAO, debugVBO;
	glGenVertexArrays(1, &debugVAO);
	glGenBuffers(1, &debugVBO);
	glBindVertexArray(debugVAO);
	glBindBuffer(GL_ARRAY_BUFFER, debugVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void*)(2*sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLuint textureID = loadTexture("wood.png");

	Shader depthShader("depthVertShader.glsl", "depthFragShader.glsl");
	glm::vec3 lightPos =  glm::vec3(-2.0, 4.0, -1.0);
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0),glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 7.5f);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		
		glClearColor(0.9, 0.1, 0.1, 1.0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
		depthShader.use();
		depthShader.setMat4("lightSpaceMatrix", lightProjection * lightView);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		renderScene(depthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		debugShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glBindVertexArray(debugVAO);
		glDrawBuffer(GL_FRONT);
		glDrawArrays(GL_TRIANGLES, 0, 6);*/

		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setVec3("lightPos", lightPos);
		shader.setVec3("viewPos", camera.Position);
		shader.setInt("blinn", blinn);
		shader.setMat4("lightSpaceMatrix", lightProjection * lightView);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		renderScene(shader);

		//开启垂直同步
		glfwSwapBuffers(window);
		//处理窗口事件
		glfwPollEvents();

		processInput(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void renderScene(Shader &shader)
{
	if (planeVAO == -1)
	{
		float planeVertices[] = {
			// positions            // normals         // texcoords
			 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
			-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
			-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

			 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
			-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
			 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
		};



		GLuint planeVBO;
		glGenVertexArrays(1, &planeVAO);
		glGenBuffers(1, &planeVBO);
		glBindVertexArray(planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	shader.setMat4("model", glm::mat4(1.0));
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glm::mat4 model;
	model = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 1.5, 0.0));
	shader.setMat4("model", glm::scale(model, glm::vec3(0.5)));
	renderCube();

	model = glm::translate(glm::mat4(1.0), glm::vec3(2.0, 0.0, 1.0));
	shader.setMat4("model", glm::scale(model, glm::vec3(0.5)));
	renderCube();

	model = glm::translate(glm::mat4(1.0), glm::vec3(-1.0, 0.0, 2.0));
	model = glm::rotate(model, 60.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	shader.setMat4("model", glm::scale(model, glm::vec3(0.25)));
	renderCube();
}

void renderCube()
{
	if (cubeVAO == -1)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};

		GLuint cubeVBO;
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
	}

	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnKeyPressed)
	{
		blinnKeyPressed = true;
		blinn = !blinn;
	}

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
	{
		blinnKeyPressed = false;
	}
}

GLuint loadTexture(const char* path)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;

	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		std::cout << "texture load error " << path << std::endl;
	}

	stbi_image_free(data);

	return textureID;
}

void showFramebufferError(GLenum status)
{
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		//error
		std::string err;

		switch (status)
		{
		case GL_FRAMEBUFFER_UNDEFINED:
			err = "GL_FRAMEBUFFER_UNDEFINED";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			err = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			err = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT ";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			err = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER ";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			err = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER ";
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			err = "GL_FRAMEBUFFER_UNSUPPORTED ";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			err = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE ";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			err = "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS  ";
			break;
		}



		std::cout << "Error building frambuffer: " << err << std::endl;
		return;
	}
}