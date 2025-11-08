#pragma once
#include "Character.h"
#include "Map.h"

class Enemy : public Character
{
protected:
	sf::Vector2f direction;
	float changeDirTimer = 0.f;
	float changeDirInterval = 0.5f;

public:
	Enemy(const std::string& texPath, sf::Vector2f startPos, float spd);
	virtual ~Enemy() = default;

	// Derived ghosts implement their AI in update
	void update(float dt, Map* map = nullptr, std::optional<sf::Vector2f> playerPos = std::nullopt) override = 0;
	void draw(sf::RenderWindow& win, sf::Vector2f offset = {0.f, 0.f}) override { Character::draw(win, offset); }

protected:
	// Helper for derived classes to test movement collision
	bool canMoveTo(const Map& map, sf::Vector2f pos);
};

