#include "GameLevel.h"

#include "ResourceManager.h"

void GameLevel::Load(const GLchar* file, GLuint levelWidth, GLuint levelHeight)
{
	Bricks.clear();

	GLuint tileCode;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<GLuint>> tileData;

	if (fstream)
	{
		while (std::getline(fstream, line))
		{
			std::istringstream sstream(line);
			vector<GLuint> row;
			while (sstream >> tileCode)
				row.push_back(tileCode);
			tileData.push_back(row);
		}

		if (tileData.size() > 0)
			init(tileData, levelWidth, levelHeight);
	}
}

void GameLevel::Draw(SpriteRenderer& renderer)
{
	for (const GameObject &obj : Bricks)
		if(!obj.Destroyed)
			renderer.DrawSprite(obj.Sprite, glm::vec2(obj.Position.x, 600 - obj.Position.y - obj.Size.y), obj.Size, obj.Rotation, obj.Color);
}

GLboolean GameLevel::IsCompleted()
{
	return true;
}


void GameLevel::init(vector<vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{
	GLuint height = tileData.size();
	GLuint width = tileData[0].size();
	GLfloat unit_width = levelWidth / (GLfloat)width;
	GLfloat unit_height = levelHeight / (GLfloat)height;

	for(GLuint y = 0; y < height; ++y)
		for (GLuint x = 0; x < width; ++x)
		{
			glm::vec2 pos(unit_width * x, unit_height * y);
			glm::vec2 size(unit_width, unit_height);
			if (tileData[y][x] == 1)
			{				
				GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"),glm::vec3(0.8,0.8,0.7));
				obj.IsSolid = GL_TRUE;
				Bricks.push_back(obj);
			}
			else if (tileData[y][x] > 1)
			{
				glm::vec3 color = glm::vec3(1.0);
				if (tileData[y][x] == 2)
					color = glm::vec3(0.2, 0.6,1.0);
				else if(tileData[y][x] == 3)
					color = glm::vec3(0.0, 0.7, 0.0);
				else if (tileData[y][x] == 4)
					color = glm::vec3(0.8, 0.8, 0.4);
				else if (tileData[y][x] == 5)
					color = glm::vec3(1.0, 0.5, 0.0);

				GameObject obj(pos, size, ResourceManager::GetTexture("block"), color);
				Bricks.push_back(obj);
			}
		}
}
