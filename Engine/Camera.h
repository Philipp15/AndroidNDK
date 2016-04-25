#pragma once

#include "glm\glm.hpp"

class Camera
{
	glm::vec3 position;
	glm::vec3 viewDirection;
	 glm::vec3 UP;
	glm::vec2 oldMousePosition;
public:
	Camera();
	void mouseUpdate(const glm::vec2& newMousePosition);
	glm::mat4 getWorldToViewMatrix() ;


	void moveForward();
	void moveBackward();

	void moveUp();
	void moveDown();

	void strafeLeft();
	void strafeRight();


	void RightForward();
};

 