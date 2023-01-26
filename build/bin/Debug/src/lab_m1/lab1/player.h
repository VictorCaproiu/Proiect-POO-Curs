#pragma once
#include "core/gpu/mesh.h"


class Player
{
public:
	Player(glm::vec3, glm::vec3);
	virtual ~Player();
	Mesh* generateModel();

	void moveUp(float units);
	void moveDown(float units);
	void moveLeft(float units);
	void moveRight(float units);

	bool isInWindow(float x, float y);
	bool isInDeliveryZone(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d);

	glm::vec3 color;
	glm::vec2 position;
};
