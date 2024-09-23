#include "../include/GL/glew.h"
#include "../include/GLFW/glfw3.h"
#include "../include/SOIL2/stb_image.h"

#include "Camera.h"
#include "Shader.h"

#pragma comment(lib, "../lib/glew32.lib")
#pragma comment(lib, "../lib/glfw3.lib")
#pragma comment(lib, "../lib/SOIL.lib")
#pragma comment(lib, "../lib/stb_image.lib")
#pragma comment(lib, "opengl32.lib") 

#include <iostream>
#include <stack>

GLuint loadTexture(const char const* path);
void processInput(GLFWwindow* window);
void window_mouse_callback(GLFWwindow* window, double xPos, double yPos);
void window_size_callback(GLFWwindow* window, int newW, int newH);
void showFramebufferError(GLenum status);
void init(GLFWwindow* window);

#define numVAOs 2
#define numVBOs 2
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

Camera camera;

GLfloat cameraX, cameraY,cameraZ;
GLfloat cubeLocX, cubeLocY, cubeLocZ;

glm::mat4 mv_matrix;
glm::mat4 proj_matrix;

int width, height;
float aspect;

GLuint mvLoc;
GLuint projLoc;

bool firstMouse = true;
float lastX = 400, lastY = 300;

Shader shader;
Shader shader2;
GLuint textureCube;
GLuint fbo;
GLuint textureColorbuffer;
void setupVertices() 
{
	float cubeVertices[] = {
		// positions          // texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	   // positions   // texCoords
	   -1.0f,  1.0f,  0.0f, 1.0f,
	   -1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

	   -1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	//创建VAO
	glGenVertexArrays(2, vao);
	//标记活跃
	glBindVertexArray(vao[0]);
	
	//创建VBO
	glGenBuffers(numVBOs, vbo);

	//标记第vbo[0]个缓冲区活跃
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//将数据复制进活跃的GPU第vbo[0]缓冲区
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	//将着色器layout=0的值关联到vbo[0]缓冲区
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*5, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 4, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 4, (void*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	textureCube = loadTexture("container.jpg");
	
	//创建帧缓存
	glGenFramebuffers(1, &fbo);
	//激活帧缓存
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//纹理附件
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//附加纹理
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	//渲染缓冲对象附件
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//创建深度和模版渲染缓冲对象
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	
	//附加渲染缓冲对象
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	/*if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR FRAMEBUFFER:: Framebuffer is not complete" << std::endl;*/
	showFramebufferError(status);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void display(GLFWwindow* window, double time)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//将着色器程序载入GPU
	shader.use();
	//
	shader.setMat4("proj_matrix", proj_matrix);
	//将矩阵数据发送到统一变量
	glm::mat4 mMat = glm::rotate(glm::mat4(1.0), 45.0f, glm::vec3(1.0, 1.0, 0.0));
	mMat = glm::scale(mMat, glm::vec3(2.0, 2.0, 2.0));
	mMat = glm::translate(mMat, glm::vec3(-1.0, 0.0, 0.0));
	shader.setMat4("mv_matrix", camera.viewMatrix() * mMat);
	
	glBindVertexArray(vao[0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureCube);
	
	glDrawArrays(GL_TRIANGLES, 0, 36);	

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);


	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	shader2.use();
	glBindVertexArray(vao[1]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}



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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	/*
	* 创建GLFW窗口，同时创建OpenGL上下文,上下文指的是OpenGL实例及其状态信息，其中包括颜色缓冲区等项
	*/
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello OpenGL",NULL, NULL);
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
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window, window_size_callback);

	//隐藏鼠标
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetCursorPosCallback(window, window_mouse_callback);

	init(window);

	while (!glfwWindowShouldClose(window))
	{
		display(window, glfwGetTime());
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


void init(GLFWwindow* window)
{
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 32.0f;
	cubeLocX = 0.0f; cubeLocY = -2.0; cubeLocZ = 0.0f;
	setupVertices();

	glfwGetFramebufferSize(window, &width, &height);
	aspect = width / height;
	proj_matrix = glm::perspective(60.0f, aspect, 0.1f, 1000.0f);

	Shader shader_("vertShader.glsl", "fragShader.glsl");
	shader = shader_;

	shader.setInt("texture1", 0);

	Shader shader2_("vertShader2.glsl", "fragShader2.glsl");
	shader2 = shader2_;
	shader2.setInt("texture1", 0);
}

GLuint loadTexture(const char const* path)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true);
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

void window_mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	float xOffset = 0, yOffset = 0;
	/*if (firstMouse) {
		xOffset = xPos - lastX;
		yOffset = lastY - yPos;
		firstMouse = false;
	}*/

	xOffset = xPos - lastX;
	yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	float sensitivity = 0.05;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	camera.Yaw += xOffset;
	camera.Pitch += yOffset;
	/*if (camera.Pitch > 89.0f)
		camera.Pitch = 89.0f;
	if(camera.Pitch < -89.0f)
		camera.Pitch = -89.0f;*/

	glm::vec3 front;
	front.x = sin(glm::radians(camera.Yaw)) * cos(glm::radians(camera.Pitch));
	front.y = sin(glm::radians(camera.Pitch));
	front.z = -cos(glm::radians(camera.Yaw)) * cos(glm::radians(camera.Pitch));
	camera.Front = glm::normalize(front);
}

void processInput(GLFWwindow* window)
{
	float cameraSpeed = 0.05f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.Position += cameraSpeed * camera.Front;
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.Position -= cameraSpeed * camera.Front;
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.Position -= glm::normalize(glm::cross(camera.Front, camera.WorldUp)) * cameraSpeed;
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.Position += glm::normalize(glm::cross(camera.Front, camera.WorldUp)) * cameraSpeed;
}


void window_size_callback(GLFWwindow* window, int newW, int newH)
{
	aspect = (float)newW / (float)newH;
	glViewport(0, 0, newW, newH);
	proj_matrix = glm::perspective(60.0f, aspect, 0.1f, 1000.0f);
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

		

		std::cout<< "Error building frambuffer: " << err << std::endl;
		return;
	}
}