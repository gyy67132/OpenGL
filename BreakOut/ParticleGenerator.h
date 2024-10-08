#pragma once

#include "Utils.h"
#include "Shader.h"
#include "Texture2D.h"
#include "GameObject.h"

struct Particle
{
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	GLfloat Life;

	Particle() :Position(0.0), Velocity(0.0), Color(1.0), Life(0.0) {};
};



class ParticleGenerator
{
public:
	ParticleGenerator(const Shader& shader,const Texture2D& texture, GLuint amount);

	void Update(GLfloat dt, GameObject& object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0, 0.0));

	void Draw();

private:
	std::vector<Particle> particles;
	GLuint amount;

	Shader shader;
	Texture2D texture;
	GLuint VAO;

	void init();
	GLuint firstUnusedParticle();

	void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

