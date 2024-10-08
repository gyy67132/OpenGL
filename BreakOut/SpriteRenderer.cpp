#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(const Shader& shader)
	:shader(shader)
{
	initRendererData();
}

SpriteRenderer::~SpriteRenderer()
{

}

void SpriteRenderer::DrawSprite(const Texture2D& texture, glm::vec2 position, glm::vec2 size,
	GLfloat rotate, glm::vec3 color)
{
	shader.use();
	
	texture.Bind();

	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0), rotate, glm::vec3(0.0,0.0,1.0));
	glm::mat4 posMat = glm::translate(glm::mat4(1.0), glm::vec3(position.x, position.y, 1.0));
	glm::mat4 mvMat = glm::scale(glm::mat4(1.0), glm::vec3(size, 1.0));
	shader.setMat4("model",  posMat * mvMat * rotateMat);

	shader.setVec3("color", color);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}


void SpriteRenderer::initRendererData()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLfloat vertex[] = {0.0,0.0, 0.0, 0.0,   1.0,0.0,1.0,0.0,
			0.0,1.0,0.0,1.0,  1.0,0.0,1.0,0.0,
			1.0,1.0,1.0,1.0,  0.0,1.0,0.0,1.0};
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
