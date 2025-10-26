#pragma once
#include "Character.h"
#include <cstdlib>
class Enemy : public Character
{
	sf::Vector2f direction;
	float changeDirectionTimer = 0.f;

public:
	Enemy(const std::string& texturePath, sf::Vector2f startPosition, float speed);

	void update(float dt) override;
	void changeDirection();
};

