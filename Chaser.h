#pragma once
#include "Enemy.h"

class Chaser : public Enemy {
public:
    Chaser(const std::string& texPath, sf::Vector2f startPos, float spd);
    void update(float dt, Map* map = nullptr, std::optional<sf::Vector2f> playerPos = std::nullopt) override;
};
