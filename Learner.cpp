#include "Learner.h"
#include <random>
#include <cmath>

Learner::Learner(const std::string& texPath, sf::Vector2f startPos, float spd)
    : Enemy(texPath, startPos, spd)
{
}

void Learner::update(float dt, Map* map, std::optional<sf::Vector2f> playerPos)
{
    if (!map || !playerPos.has_value()) {
        // Fallback to random-like movement if no player info
        changeDirTimer += dt;
        if (changeDirTimer > changeDirInterval) {
            changeDirTimer = 0.f;
            static std::mt19937 rng{ std::random_device{}() };
            std::uniform_int_distribution<int> d(0, 3);
            int dir = d(rng);
            if (dir == 0) direction = { 1.f, 0.f };
            else if (dir == 1) direction = { -1.f, 0.f };
            else if (dir == 2) direction = { 0.f, 1.f };
            else direction = { 0.f, -1.f };
        }

        sf::Vector2f newPos = position + direction * speed * dt * 0.8f;
        if (canMoveTo(*map, newPos))
            position = newPos;

        setPosition(position);
        return;
    }

    // Record player's tile
    int tx = static_cast<int>(playerPos->x / map->getTileSize());
    int ty = static_cast<int>(playerPos->y / map->getTileSize());
    auto key = std::make_pair(tx, ty);
    visitHeatmap[key]++;

    // Find most visited tile
    std::pair<int,int> maxKey = key;
    int maxVal = 0;
    for (const auto& kv : visitHeatmap) {
        if (kv.second > maxVal) {
            maxVal = kv.second;
            maxKey = kv.first;
        }
    }

    sf::Vector2f target(maxKey.first * map->getTileSize(), maxKey.second * map->getTileSize());
    sf::Vector2f diff = target - position;
    float len = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    if (len == 0.f) return;

    sf::Vector2f dir = diff / len;
    sf::Vector2f newPos = position + dir * speed * dt * 0.8f;
    if (canMoveTo(*map, newPos))
        position = newPos;

    setPosition(position);
}
