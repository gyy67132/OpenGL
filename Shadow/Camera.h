#pragma once

#include "Utils.h"

const float YAW = 0.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;


	Camera(glm::vec3 position = glm::vec3(0,0,7), glm::vec3 up = glm::vec3(0,1,0), float yaw = YAW, float pitch = PITCH)
		:Front(glm::vec3(0, 0, -1)),MovementSpeed(SPEED),MouseSensitivity(SENSITIVITY),Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;

	}

	glm::mat4 viewMatrix()
	{
		//glm::mat4 vMat = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, -8));

		float radius = 8.0f;
		glm::vec3 position = glm::vec3(sin(glfwGetTime()) * radius, 0, cos(glfwGetTime()) * radius);
		glm::mat4 viewM = glm::lookAt(Position, Position + Front, WorldUp);
		return viewM;
	}

private:
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x =
		front.y = sin(glm::radians(Pitch));
		//front.z = 

	}
};

