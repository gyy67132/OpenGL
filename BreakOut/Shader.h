#pragma once

#include "Utils.h"
#include "../include/GL/glew.h"
#include "../include/GLFW/glfw3.h"
#include <iostream>
#include <sstream>

class Shader
{
public:
	GLuint ID;
	Shader(){}
	Shader(const Shader &shader) {
		ID = shader.ID;
	}
	Shader(const char *vertexPath, const char *fragmentPath,const char *geometryPath = "")
	{
		if(strlen(geometryPath) == 0)
			ID = Utils::createShaderProgram(vertexPath, fragmentPath);
		else
			ID = Utils::createShaderProgram(vertexPath, fragmentPath, geometryPath);
	}
	~Shader() {}

	void use() const {
		glUseProgram(ID);
	}

	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void setInt(const std::string& name, const int& value)
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setFloat(const std::string& name, const float& value)
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setVec3(const std::string& name, const glm::vec3& value)
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
	}

	void setVec2(const std::string& name, const glm::vec2& value)
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), value.x, value.y) ;
	}

	void setVec4(const std::string& name, const glm::vec4& value)
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), value.x, value.y,value.z, value.w);
	}
private:

};

