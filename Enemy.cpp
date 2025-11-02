#include "Enemy.h"

Enemy::Enemy(const std::string& texturePath, sf::Vector2f startPosition, float speed)
	: Character(texturePath, startPosition, speed)
{
	changeDirection();
}

void Enemy::update(float dt, Map* map)
{
	changeDirectionTimer += dt;
	if (changeDirectionTimer > 2.f) {
		changeDirection();
		changeDirectionTimer = 0.f;
	}
	position += direction * dt;
	setPosition(position);
}

void Enemy::changeDirection()
{
	int d = rand() % 4;
	switch (d) {
	case 0: direction = { speed, 0 }; break;
	case 1: direction = { -speed, 0 }; break;
	case 2: direction = { 0, speed }; break;
	case 3: direction = { 0, -speed }; break;
	}
}
