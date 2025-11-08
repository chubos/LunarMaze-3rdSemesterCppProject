#include "Enemy.h"

Enemy::Enemy(const std::string& texPath, sf::Vector2f startPos, float spd)
    : Character(texPath, startPos, spd)
{
    direction = { 1.f, 0.f };
    changeDirTimer = 0.f;
}

bool Enemy::canMoveTo(const Map& map, sf::Vector2f pos)
{
    float tileSize = map.getTileSize();
    sf::FloatRect next(pos.x, pos.y, tileSize, tileSize);
    return !map.isWallCollision(next);
}
