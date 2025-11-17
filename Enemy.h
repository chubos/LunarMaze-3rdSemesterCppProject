#pragma once
#include "Character.h"
#include "Map.h"

class Enemy : public Character
{
protected:
	sf::Vector2f direction; // Wektor kierunku ruchu.
	float changeDirTimer = 0.f; // Licznik czasu do zmiany kierunku.
	float changeDirInterval = 2.5f; // Interwa³, co ile ma nastêpowaæ zmiana kierunku.

public:
	Enemy(const std::string& texPath, sf::Vector2f startPos, float spd);
	virtual ~Enemy() = default;

	void update(float dt, Map* map = nullptr, std::optional<sf::Vector2f> playerPos = std::nullopt) override = 0;
	void draw(sf::RenderWindow& win, sf::Vector2f offset = { 0.f, 0.f }) override { Character::draw(win, offset); } 

protected:
	bool canMoveTo(const Map& map, sf::Vector2f pos); // Czy postaæ mo¿e siê ruszyæ na dan¹ pozycjê
};