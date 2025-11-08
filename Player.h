#pragma once
#include "Character.h"
#include "Map.h"
#include <optional>

class Player : public Character
{
	sf::Vector2f velocity;
public:
	Player(const std::string& texturePath, sf::Vector2f startPosition, float speed);

	void handleInput();
	void update(float dt, Map* map = nullptr, std::optional<sf::Vector2f> playerPos = std::nullopt) override;
};
