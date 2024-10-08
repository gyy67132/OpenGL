#include "BallObject.h"

BallObject::BallObject()
{

}

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
{
	Size = glm::vec2(20, 20);
	Color = glm::vec3(1, 1, 1);
	Position = pos;
	Velocity = velocity;
	Sprite = sprite;
	Radius = radius;
	Stuck = true;
	Sticky = false;
	PassThrough = false;
}

glm::vec2 BallObject::Move(GLfloat dt, GLuint window_width, GLuint window_height)
{
	if (!Stuck)
	{
		Position += Velocity * dt;
		if (Position.x >= window_width - Size.x) {
			Velocity.x = -Velocity.x;
			Position.x = window_width - Size.x;
		}
		else if (Position.x <= 0)
		{
			Velocity.x = -Velocity.x;
			Position.x = 0.0;
		}

		if (Position.y >= window_height - Size.y) {
			Velocity.y = -Velocity.y;
			Position.y = window_height - Size.y;
		}
		else if (Position.y <= 0)
		{
			//Velocity.y = -Velocity.y;
			//Position.y = 0.0;
		}
	}
	return glm::vec2(0, 0);
}

void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{

}
