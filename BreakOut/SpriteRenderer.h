#pragma once

#include "Shader.h"
#include "Texture2D.h"
#include "Utils.h"

class SpriteRenderer
{
public:
	SpriteRenderer(const Shader& shader);
	~SpriteRenderer();

	void DrawSprite(const Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(2, 2),
		GLfloat rotate = 0.0, glm::vec3 color = glm::vec3(1.0));

private:
	Shader shader;
	GLuint VAO;

	void initRendererData();
};

