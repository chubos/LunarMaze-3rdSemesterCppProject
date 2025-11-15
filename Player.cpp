#include "Player.h" // Do³¹czenie nag³ówka klasy Player.
#include <SFML/Window/Keyboard.hpp> // Do³¹czenie SFML dla obs³ugi klawiatury.
#include <cmath> // Do³¹czenie dla funkcji std::sqrt.

// ## Konstruktor klasy Player
Player::Player(const std::string& texturePath, sf::Vector2f startPosition, float speed)
	: Character(texturePath, startPosition, speed) // Wywo³anie konstruktora klasy bazowej Character.
{
}

// ## Obs³uga wejœcia u¿ytkownika
// Ustawia wektor prêdkoœci (velocity) na podstawie wciœniêtych klawiszy WASD.
void Player::handleInput()
{
	velocity = { 0.f, 0.f }; // Zerowanie prêdkoœci na pocz¹tku klatki.

	// Sprawdzenie klawiszy i ustawienie sk³adowej prêdkoœci w zale¿noœci od kierunku.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		velocity.y -= speed; // Góra (Y maleje).
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		velocity.y += speed; // Dó³ (Y roœnie).
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		velocity.x -= speed; // Lewo (X maleje).
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		velocity.x += speed; // Prawo (X roœnie).

}

// ## Metoda aktualizacji stanu gracza
void Player::update(float dt, Map* map, std::optional<sf::Vector2f> /*playerPos*/)
{
	handleInput(); // Pobranie aktualnego wejœcia (ustawienie velocity).

	// Normalizacja prêdkoœci ukoœnej:
	// Jeœli ruch jest ukoœny (X i Y ró¿ne od 0), zmniejsz prêdkoœæ o 1/sqrt(2)
	// aby zachowaæ sta³¹ prêdkoœæ ruchu we wszystkich kierunkach.
	if (velocity.x != 0.f && velocity.y != 0.f) {
		float inv = 1.f / std::sqrt(2.f);
		velocity.x *= inv;
		velocity.y *= inv;
	}

	sf::Vector2f oldPosition = position; // Zapisanie pozycji przed ruchem.

	// 1. Ruch w osi X i sprawdzenie kolizji.
	position.x += velocity.x * dt; // Przesuniêcie pozycji X.
	setPosition(position); // Ustawienie nowej pozycji (aktualizuje te¿ sprite).
	if (map && map->isWallCollision(getBounds())) { // Sprawdzenie kolizji ze œcian¹.
		position.x = oldPosition.x; // Wycofanie pozycji X w razie kolizji.
		setPosition(position); // Ponowne ustawienie pozycji (tylko Y ulegnie zmianie w nastêpnym kroku).
	}

	// 2. Ruch w osi Y i sprawdzenie kolizji.
	position.y += velocity.y * dt; // Przesuniêcie pozycji Y.
	setPosition(position); // Ustawienie nowej pozycji.
	if (map && map->isWallCollision(getBounds())) { // Sprawdzenie kolizji ze œcian¹.
		position.y = oldPosition.y; // Wycofanie pozycji Y w razie kolizji.
		setPosition(position); // Ostateczne ustawienie pozycji.
	}

	// Zbieranie przedmiotów jest obs³ugiwane w Game::update.
}