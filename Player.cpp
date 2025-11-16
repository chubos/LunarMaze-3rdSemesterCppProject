#include "Player.h"
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

Player::Player(const std::string& texturePath, sf::Vector2f startPosition, float speed)
	: Character(texturePath, startPosition, speed) // Wywo³anie konstruktora klasy bazowej Character.
{
}

void Player::handleInput() // Obs³uga wejœcia u¿ytkownika
{
	velocity = { 0.f, 0.f }; // Zerowanie prêdkoœci na pocz¹tku klatki.

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) // Sprawdzenie klawiszy i ustawienie sk³adowej prêdkoœci w zale¿noœci od kierunku.
		velocity.y -= speed; // Góra (Y maleje).
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		velocity.y += speed; // Dó³ (Y roœnie).
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		velocity.x -= speed; // Lewo (X maleje).
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		velocity.x += speed; // Prawo (X roœnie).

}


void Player::update(float dt, Map* map, std::optional<sf::Vector2f> /*playerPos*/) // Metoda aktualizacji stanu gracza
{
	handleInput(); // Pobranie aktualnego wejœcia (ustawienie velocity).

	if (velocity.x != 0.f && velocity.y != 0.f) { // Normalizacja prêdkoœci ukoœnej. Jeœli ruch jest ukoœny (X i Y ró¿ne od 0), zmniejsz prêdkoœæ o 1/sqrt(2), aby zachowaæ sta³¹ prêdkoœæ ruchu we wszystkich kierunkach.
		float inv = 1.f / std::sqrt(2.f);
		velocity.x *= inv;
		velocity.y *= inv;
	}

	sf::Vector2f oldPosition = position; // Zapisanie pozycji przed ruchem.

	position.x += velocity.x * dt; // Przesuniêcie pozycji X.
	setPosition(position); // Ustawienie nowej pozycji (aktualizuje te¿ sprite).
	if (map && map->isWallCollision(getBounds())) { // Sprawdzenie kolizji ze œcian¹.
		position.x = oldPosition.x; // Wycofanie pozycji X w razie kolizji.
		setPosition(position); // Ponowne ustawienie pozycji (tylko Y ulegnie zmianie w nastêpnym kroku).
	}

	position.y += velocity.y * dt; // Przesuniêcie pozycji Y.
	setPosition(position); // Ustawienie nowej pozycji.
	if (map && map->isWallCollision(getBounds())) { // Sprawdzenie kolizji ze œcian¹.
		position.y = oldPosition.y; // Wycofanie pozycji Y w razie kolizji.
		setPosition(position); // Ostateczne ustawienie pozycji.
	}

}