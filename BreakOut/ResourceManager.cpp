#include "ResourceManager.h"

#include "../include/SOIL2/SOIL2.h"
#include "../include/SOIL2/stb_image.h"

#include <iostream>

std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture2D> ResourceManager::Textures;

Shader ResourceManager::LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, string name)
{
	Shaders[name] = Shader(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

Shader ResourceManager::GetShader(string name)
{
	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar* file, GLboolean alpha, string name)
{
	Texture2D texture;
	if (alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	int width, height;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

	texture.Generate(width, height, image);

	SOIL_free_image_data(image);

	Textures[name] = texture;
	return texture;
}

Texture2D ResourceManager::GetTexture(string name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	std::map<std::string, Shader>::iterator it;
	for (it = Shaders.begin() ; it != Shaders.end(); it++)
		glDeleteProgram((*it).second.ID);
	
	for (auto it : Textures)
		glDeleteTextures(1, &(it.second.ID));
	
	Textures.clear();
	Shaders.clear();
}
