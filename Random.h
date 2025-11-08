#pragma once
#include "Enemy.h"
#include <random>

class Random : public Enemy {
public:
    Random(const std::string& texPath, sf::Vector2f startPos, float spd);
    void update(float dt, Map* map = nullptr, std::optional<sf::Vector2f> playerPos = std::nullopt) override;
private:
    static std::mt19937 rng;
};
