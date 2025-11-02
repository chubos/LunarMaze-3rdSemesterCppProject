#include "Player.h"
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

Player::Player(const std::string& texturePath, sf::Vector2f startPosition, float speed)
	: Character(texturePath, startPosition, speed)
{
}

void Player::handleInput()
{
	velocity = { 0.f, 0.f };

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		velocity.y -= speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		velocity.y += speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		velocity.x -= speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		velocity.x += speed;

}

void Player::update(float dt, Map* map)
{
	handleInput();

	if (velocity.x != 0.f && velocity.y != 0.f) {
		float inv = 1.f / std::sqrt(2.f);
		velocity.x *= inv;
		velocity.y *= inv;
	}

	sf::Vector2f oldPosition = position;

	position.x += velocity.x * dt;
	setPosition(position);
	if (map && map->isWallCollision(getBounds())) {
		position.x = oldPosition.x;
		setPosition(position);
	}

	position.y += velocity.y * dt;
	setPosition(position);
	if (map && map->isWallCollision(getBounds())) {
		position.y = oldPosition.y;
		setPosition(position);
	}

	if (map) map->collectCrystalAt(getBounds());
}
