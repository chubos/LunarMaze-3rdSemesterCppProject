#include "Random.h"

std::mt19937 Random::rng{ std::random_device{}() };

Random::Random(const std::string& texPath, sf::Vector2f startPos, float spd)
    : Enemy(texPath, startPos, spd)
{
}

void Random::update(float dt, Map* map, std::optional<sf::Vector2f> /*playerPos*/)
{
    if (!map) return;

    changeDirTimer += dt;
    if (changeDirTimer > changeDirInterval) {
        changeDirTimer = 0.f;
        std::uniform_int_distribution<int> d(0, 3);
        int dir = d(rng);
        if (dir == 0) direction = { 1.f, 0.f };
        else if (dir == 1) direction = { -1.f, 0.f };
        else if (dir == 2) direction = { 0.f, 1.f };
        else direction = { 0.f, -1.f };
    }

    sf::Vector2f newPos = position + direction * speed * dt;
    if (canMoveTo(*map, newPos))
        position = newPos;

    setPosition(position);
}
