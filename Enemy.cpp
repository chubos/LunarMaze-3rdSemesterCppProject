#include "Enemy.h"

Enemy::Enemy(const std::string& texPath, sf::Vector2f startPos, float spd)
	: Character(texPath, startPos, spd)
{
	direction = { 1.f, 0.f }; // Ustawienie domyœlnego kierunku ruchu (np. w prawo).
	changeDirTimer = 0.f; // Wyzerowanie timera zmiany kierunku.
}

bool Enemy::canMoveTo(const Map& map, sf::Vector2f pos) // Sprawdza, czy wróg mo¿e siê przesun¹æ do danej pozycji bez kolizji ze œcian¹.
{
	float tileSize = map.getTileSize(); // Pobranie rozmiaru kafelka z mapy.

	sf::FloatRect next(pos.x, pos.y, tileSize, tileSize); // Utworzenie tymczasowego prostok¹ta kolizyjnego dla docelowej pozycji.

	return !map.isWallCollision(next); // Sprawdzenie na mapie, czy docelowy prostok¹t koliduje ze œcian¹. Zwraca true, jeœli nie ma kolizji ze œcian¹.
}