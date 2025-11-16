#pragma once
#include "Enemy.h"
#include <random>

class Random : public Enemy {
public:
	Random(const std::string& texPath, sf::Vector2f startPos, float spd);
	void update(float dt, Map* map = nullptr, std::optional<sf::Vector2f> playerPos = std::nullopt) override; // Implementacja logiki AI (losowego wyboru kierunku ruchu).

private:
	
	static std::mt19937 rng;// Statyczny generator liczb pseudolosowych, wspó³dzielony przez wszystkie instancje klasy Random.
};