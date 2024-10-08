#pragma once

#include "GameObject.h"

#include <vector>

using namespace std;

#include "SpriteRenderer.h"

class GameLevel
{
public:
	vector<GameObject> Bricks;

	GameLevel() {}

	void Load(const GLchar* file, GLuint levelWidth, GLuint levelHeight);

	void Draw(SpriteRenderer &renderer);

	GLboolean IsCompleted();

private:
	void init(vector<vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};

