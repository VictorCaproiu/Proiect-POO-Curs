#include "lab_m1/lab1/player.h"

Mesh* Player::generateModel()
{
	Mesh* mesh = new Mesh("player");

	std::vector<VertexFormat> vertices;
	vertices.emplace_back(glm::vec3(-1, 1, 0), color);
	vertices.emplace_back(glm::vec3(1, 1, 0), color);
	vertices.emplace_back(glm::vec3(-1, -1, 0), color);
	vertices.emplace_back(glm::vec3(1, -1, 0), color);

	mesh->InitFromData(vertices, { 0, 1, 2, 3, 2, 1 });

	return mesh;
}

void Player::moveUp(float units)
{
	position.y += units;
}

void Player::moveDown(float units)
{
	moveUp(-units);
}

void Player::moveLeft(float units)
{
	moveRight(-units);
}

void Player::moveRight(float units)
{
	position.x += units;
}

Player::Player(glm::vec3 position, glm::vec3 color)
{
	this->color = color;
	this->position = position;
}

Player::~Player()
{
}

bool Player::isInWindow(float x, float y)
{
	if (-x > position.x || position.x > x)
	{
		return false;
	}

	if (-y > position.y || position.y > y)
	{
		return false;
	}

	return true;
}

bool Player::isInDeliveryZone(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d)
{
	if (a.x > position.x || position.x > b.x)
	{
		return false;
	}

	if (c.y > position.y || position.y > a.y)
	{
		return false;
	}

	return true;
}
