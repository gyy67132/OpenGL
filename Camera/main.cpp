#include "../include/GL/glew.h"
#include "../include/GLFW/glfw3.h"

#include "Camera.h"

#pragma comment(lib, "../lib/glew32.lib")
#pragma comment(lib, "../lib/glfw3.lib")
#pragma comment(lib, "../lib/SOIL.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(include, "../include") 

#include <iostream>
#include <stack>

#define numVAOs 1
#define numVBOs 2
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

Camera camera;

GLuint program;

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

void setupVertices() 
{
	float vertexPositions[108] = { -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,    //front
		1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,    //right
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  //back
		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,  //left
		-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, //LF
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f  //RR
	};

	//创建VAO
	glGenVertexArrays(1, vao);
	//标记活跃
	glBindVertexArray(vao[0]);
	
	//创建VBO
	glGenBuffers(numVBOs, vbo);

	//标记第vbo[0]个缓冲区活跃
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//将数据复制进活跃的GPU第vbo[0]缓冲区
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}


void init(GLFWwindow *window)
{
	program = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 32.0f;
	cubeLocX = 0.0f; cubeLocY = -2.0; cubeLocZ = 0.0f;
	setupVertices();

	//获取mv_matrix变量的位置
	mvLoc = glGetUniformLocation(program, "mv_matrix");
	projLoc = glGetUniformLocation(program, "proj_matrix");	

	glfwGetFramebufferSize(window, &width, &height);
	aspect = width / height;
	proj_matrix = glm::perspective(60.0f, aspect, 0.1f, 1000.0f);
	
}

void display(GLFWwindow* window, double time)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);


	//将着色器程序载入GPU
	glUseProgram(program);

	//
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj_matrix));

	double t = time;
	glm::mat4 vMat = glm::translate(glm::mat4(1.0), glm::vec3(-cameraX, -cameraY, -cameraZ));
	
	//将矩阵数据发送到统一变量
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(camera.viewMatrix()));
	
	//标记第vbo[0]个缓冲区活跃
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//将着色器layout=0的值关联到vbo[0]缓冲区
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 18);	
}

void window_size_callback(GLFWwindow *window, int newW, int newH)
{
	aspect = (float)newW / (float)newH;
	glViewport(0, 0, newW, newH);
	proj_matrix = glm::perspective(60.0f, aspect, 0.1f, 1000.0f);
}

void window_mouse_callback(GLFWwindow *window, double xPos, double yPos)
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

void processInput(GLFWwindow *window)
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
	glfwSetCursorPosCallback(window, window_mouse_callback);

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