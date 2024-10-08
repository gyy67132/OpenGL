#include "GameObject.h"


GameObject::GameObject()
{
}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color,
	glm::vec2 velocity)
{
	Rotation = 0.0f;
	Destroyed = false;
	IsSolid = false;
	Position = pos;
	Size = size;
	Velocity = velocity;
	Color = color;
	Sprite = sprite;
}

void GameObject::Draw(SpriteRenderer& renderer)
{
	renderer.DrawSprite(Sprite, Position, Size, Rotation, Color);
}