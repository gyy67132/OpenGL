#pragma once

#include "../include/GL/glew.h"
#include "Texture2D.h"
#include "Shader.h"

#include <map>
#include <string>
using namespace std;

class ResourceManager
{
public:
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture2D> Textures;

	static Shader LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, string name);
	static Shader GetShader(string name);

	static Texture2D LoadTexture(const GLchar* file, GLboolean alpha, string name);
	static Texture2D GetTexture(string name);

	static void Clear();

private:
	ResourceManager(){}

};

