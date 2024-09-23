#include "../include/GL/glew.h"
#include "../include/GLFW/glfw3.h"
#include "Utils.h"

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

GLuint program;

GLfloat cameraX, cameraY,cameraZ;
GLfloat cubeLocX, cubeLocY, cubeLocZ;

glm::mat4 mv_matrix;
glm::mat4 proj_matrix;

int width, height;
float aspect;

GLuint mvLoc;
GLuint projLoc;

std::stack<glm::mat4> mvStack;

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

	float pyramidPositions[54] =
	{ -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,    //front
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
	

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);
	
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
	mvStack.push(vMat);

	mvStack.push(mvStack.top());
	mvStack.top() *= glm::rotate(glm::mat4(1.0), float(time)*10.0f, glm::vec3(1.0, 0.0, 0.0));
	//将矩阵数据发送到统一变量
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	
	//标记第vbo[0]个缓冲区活跃
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//将着色器layout=0的值关联到vbo[0]缓冲区
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 18);
	mvStack.pop();

	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0), glm::vec3(sin(float(time))*12.0, 0.0f, cos((float)time)*12.0f));
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::rotate(glm::mat4(1.0), float(time)*10, glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	//标记第vbo[0]个缓冲区活跃
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//将着色器layout=0的值关联到vbo[0]缓冲区
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	mvStack.pop();

	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0), glm::vec3(0.0f, sin(float(time)) * 2.0, cos((float)time) * 2.0f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0), float(time)*10, glm::vec3(0.0, 0.0, 1.0));
	mvStack.top() *= glm::scale(glm::mat4(1.0), glm::vec3(0.25, 0.25, 0.25));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 18);
	
	mvStack.pop();
	mvStack.pop();
	mvStack.pop();
}

void window_size_callback(GLFWwindow *window, int newW, int newH)
{
	aspect = (float)newW / (float)newH;
	glViewport(0, 0, newW, newH);
	proj_matrix = glm::perspective(60.0f, aspect, 0.1f, 1000.0f);
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

	init(window);

	while (!glfwWindowShouldClose(window))
	{
		display(window, glfwGetTime());
		//开启垂直同步
		glfwSwapBuffers(window);
		//处理窗口事件
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}