#include "Player.h"
#include <SFML/Window/Keyboard.hpp>

Player::Player(const std::string& texturePath, sf::Vector2f startPosition, float speed)
	: Character(texturePath, startPosition, speed)
{
}

void Player::handleInput(float dt)
{
	sf::Vector2f move(0.f, 0.f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		move.y -= speed * dt;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		move.y += speed * dt;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		move.x -= speed * dt;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		move.x += speed * dt;

	position += move;
	sprite.setPosition(position);

}

void Player::update(float dt)
{
	handleInput(dt);
}
