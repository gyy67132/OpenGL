#pragma once

#include "Utils.h"
#include <iostream>
#include <sstream>
class Shader
{
public:
	GLuint ID;
	Shader(){}
	Shader(Shader &sahder) {
		ID = sahder.ID;
	}
	Shader(const char *vertexPath, const char *fragmentPath)
	{
		/*std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream;
			std::stringstream fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e) {
			std::cout << "shader file error: " << e.what() << std::endl;;
		}*/

		ID = Utils::createShaderProgram(vertexPath, fragmentPath);
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
private:

};

