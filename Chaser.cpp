#include "Chaser.h"

Chaser::Chaser(const std::string& texPath, sf::Vector2f startPos, float spd)
    : Enemy(texPath, startPos, spd)
{
}

void Chaser::update(float dt, Map* map, std::optional<sf::Vector2f> playerPos)
{
    if (!map || !playerPos.has_value()) return;

    sf::Vector2f player = playerPos.value();
    sf::Vector2f diff = player - position;
    float len = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    if (len == 0.f) return;

    sf::Vector2f dir = diff / len;
    sf::Vector2f newPos = position + dir * speed * dt;

    if (canMoveTo(*map, newPos)) {
        position = newPos;
    } else {
        // Try axis-aligned moves to slide along obstacles
        sf::Vector2f tryX = position;
        tryX.x += dir.x * speed * dt;
        if (canMoveTo(*map, tryX)) {
            position = tryX;
        } else {
            sf::Vector2f tryY = position;
            tryY.y += dir.y * speed * dt;
            if (canMoveTo(*map, tryY)) {
                position = tryY;
            }
        }
    }

    setPosition(position);
}
