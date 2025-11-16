#pragma once
#include "Enemy.h"
#include <map>

class Learner : public Enemy {
public:
	Learner(const std::string& texPath, sf::Vector2f startPos, float spd);
	void update(float dt, Map* map = nullptr, std::optional<sf::Vector2f> playerPos = std::nullopt) override; // Implementacja logiki AI (np. unikanie miejsc z wysok¹ 'map¹ ciep³a').

private:
	std::map<std::pair<int, int>, int> visitHeatmap; // Mapa ciep³a odwiedzin: przechowuje liczbê odwiedzin dla ka¿dego kafelka (para int,int).
};