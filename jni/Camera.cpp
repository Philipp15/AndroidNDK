#include "Camera.h"

#include "glm/gtx/transform.hpp"
#include "Helpers.h"
#include <string>

glm::vec3 experimentVector = glm::vec3(0.0f, 0.0f, -1.0f);

Camera::Camera() : viewDirection(0.0f, 0.0f, -1.0f),
UP(0.0f, 1.0f, 0.0f), position(0.0f,0.0f,10.0f)
{


}


glm::mat4 Camera::getWorldToViewMatrix() 
{
		glm::vec3 someVector;

		someVector = position + viewDirection;

//		std::cout << std::to_string(someVector.x) << "    " << std::to_string(someVector.y) << "    " << std::to_string(someVector.z) << std::endl;

		return glm::lookAt(position, position + viewDirection, UP);

}

void Camera::mouseUpdate(const glm::vec2& newMousePosition)
{

	glm::vec2 delta = newMousePosition - oldMousePosition;

	if (glm::length(delta) > 50.0f)
	{
		oldMousePosition = newMousePosition;
		return;
	}

	
	viewDirection = glm::mat3(glm::rotate(delta.x * -0.005f, UP)) * viewDirection;

	glm::vec3 toRotateRound = glm::cross(viewDirection, UP);
	viewDirection = glm::mat3(glm::rotate(delta.y * -0.005f, toRotateRound)) * viewDirection;


	oldMousePosition = newMousePosition;

}

const float MOVEMENT_SPEED = 0.1f;

void Camera::moveForward()
{
	position += MOVEMENT_SPEED * viewDirection;

}
void Camera::moveBackward()
{
	position -= MOVEMENT_SPEED * viewDirection;
}

void Camera::moveUp()
{
	position += MOVEMENT_SPEED * UP;
}
void Camera::moveDown()
{
	position += -MOVEMENT_SPEED * UP;
}
void Camera::strafeLeft()
{
	glm::vec3 strafeDirection = glm::cross(viewDirection, UP);
	position += -MOVEMENT_SPEED * strafeDirection;
}
void Camera::strafeRight()
{
	glm::vec3 strafeDirection = glm::cross(viewDirection, UP);
	position += MOVEMENT_SPEED * strafeDirection;
}

