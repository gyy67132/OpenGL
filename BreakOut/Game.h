#pragma once

#include "../include/GL/glew.h"
#include "../include/GLFW/glfw3.h"

#include "GameLevel.h"
#include "BallObject.h"
#include "ParticleGenerator.h"
#include "PostProcessor.h"
#include "PowerUp.h"

#define WindowsWidth 800
#define WindowsHeight 600

enum Direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game
{
public:
	GameState State;
	GLboolean Keys[1024];
	GLuint Width, Height;
	Game(GLuint width, GLuint height);
	~Game();

	void Init();

	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();

	vector<GameLevel> Levels;
	GLuint Level;

	GameObject paddle;
	BallObject ballObject;

	void CheckCollision();
	GLboolean DoCollisions(GameObject& one, GameObject& obj);
	GLboolean DoCollisions(GameObject& obj);
	void PaddleCollision();
	Direction VectorDirection(glm::vec2 target);
	ParticleGenerator *particleGenerator;
	PostProcessor* postProcessor;

	GLfloat ShakeTime;

	std::vector<PowerUp> PowerUps;
	void SpawnPowerUps(GameObject& block);
	void UpdatePowerUps(GLfloat dt);
	void ActivatePowerUp(PowerUp& powerUp);
};

