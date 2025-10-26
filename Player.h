#pragma once
#include "Character.h"
class Player : public Character
{
public:
	Player(const std::string& texturePath, sf::Vector2f startPosition, float speed);

	void handleInput(float dt);
	void update(float dt) override;
};

