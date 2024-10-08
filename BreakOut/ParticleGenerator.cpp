#include "ParticleGenerator.h"


ParticleGenerator::ParticleGenerator(const Shader& shader, const Texture2D& texture, GLuint amount)
	:shader(shader), texture(texture), amount(amount)
{
	init();
}

void ParticleGenerator::Update(GLfloat dt, GameObject& object, GLuint newParticles, glm::vec2 offset)
{
	for (GLuint i = 0; i < newParticles; i++)
	{
		int unusedParticle = this->firstUnusedParticle();
		respawnParticle(particles[unusedParticle], object, offset);
	}

	for (GLuint i = 0; i < amount; i++)
	{
		Particle& p = particles[i];
		p.Life -= dt;
		if (p.Life > 0.0f)
		{
			p.Position -= p.Velocity * dt ;
			p.Color.a -= dt * 2.5;
		}
	}
	
}

void ParticleGenerator::Draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	shader.use();

	for (Particle &particle : particles) 
	{
		if (particle.Life > 0.0f)
		{
			shader.setVec2("offset", particle.Position);
			shader.setVec4("ParticleColor",  particle.Color);
			texture.Bind();
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void ParticleGenerator::init()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLfloat vertex[] = { 0.0,0.0, 0.0, 0.0,   1.0,0.0,1.0,0.0,
			0.0,1.0,0.0,1.0,  1.0,0.0,1.0,0.0,
			1.0,1.0,1.0,1.0,  0.0,1.0,0.0,1.0 };

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT),(void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	for (GLuint i = 0; i < amount; ++i)
		particles.push_back(Particle());
}

GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle()
{
	for (GLuint i = lastUsedParticle; i < amount; i++) {
		if (particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}

	for (GLuint i = 0; i < lastUsedParticle; i++)
	{
		if (particles[i].Life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}
	lastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset)
{
	GLfloat random = ((rand() % 100) - 50) / 10.0f;
	GLfloat rColor = 0.5 + ((rand()%50)/100.0f);
	particle.Position = object.Position + random + offset;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = object.Velocity * 0.1f;
}
