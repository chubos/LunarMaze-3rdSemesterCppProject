#pragma once
#include "Enemy.h"
#include <map>

class Learner : public Enemy {
public:
    Learner(const std::string& texPath, sf::Vector2f startPos, float spd);
    void update(float dt, Map* map = nullptr, std::optional<sf::Vector2f> playerPos = std::nullopt) override;

private:
    std::map<std::pair<int,int>, int> visitHeatmap;
};
