#include "Player.h"
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

Player::Player(const std::string& texturePath, sf::Vector2f startPosition, float speed)
	: Character(texturePath, startPosition, speed)
{
}

void Player::handleInput()
{
	velocity = { 0.f, 0.f }; // Zerowanie prêdkoœci na pocz¹tku klatki.

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		velocity.y -= speed; // Góra
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		velocity.y += speed; // Dó³
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		velocity.x -= speed; // Lewo
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		velocity.x += speed; // Prawo

}


void Player::update(float dt, Map* map, std::optional<sf::Vector2f> /*playerPos*/)
{
	handleInput(); // Pobranie aktualnego wejœcia (ustawienie velocity).

	if (velocity.x != 0.f && velocity.y != 0.f) { // Normalizacja prêdkoœci ukoœnej.
		float inv = 1.f / std::sqrt(2.f);
		velocity.x *= inv;
		velocity.y *= inv;
	}

	sf::Vector2f oldPosition = position; // Zapisanie pozycji przed ruchem.

	position.x += velocity.x * dt; // Ruch X
	setPosition(position);
	if (map && map->isWallCollision(getBounds())) { 
		position.x = oldPosition.x;
		setPosition(position);
	}

	position.y += velocity.y * dt; // Ruch Y
	setPosition(position);
	if (map && map->isWallCollision(getBounds())) {
		position.y = oldPosition.y;
		setPosition(position);
	}

}