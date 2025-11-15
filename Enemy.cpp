#include "Enemy.h" // Do³¹czenie pliku nag³ówkowego klasy Enemy.

// ## Konstruktor klasy Enemy
// Wywo³uje konstruktor klasy bazowej Character i ustawia pocz¹tkowe wartoœci dla wroga.
Enemy::Enemy(const std::string& texPath, sf::Vector2f startPos, float spd)
	: Character(texPath, startPos, spd) // Wywo³anie konstruktora Character.
{
	// Ustawienie domyœlnego kierunku ruchu (np. w prawo).
	direction = { 1.f, 0.f };
	// Wyzerowanie timera zmiany kierunku.
	changeDirTimer = 0.f;
}

// ## Sprawdzenie mo¿liwoœci ruchu
// Metoda pomocnicza: sprawdza, czy wróg mo¿e siê przesun¹æ do danej pozycji bez kolizji ze œcian¹.
bool Enemy::canMoveTo(const Map& map, sf::Vector2f pos)
{
	// Pobranie rozmiaru kafelka z mapy.
	float tileSize = map.getTileSize();

	// Utworzenie tymczasowego prostok¹ta kolizyjnego (sf::FloatRect) dla docelowej pozycji.
	// Zak³adamy, ¿e hitbox wroga jest w rozmiarze kafelka (dla uproszczenia).
	sf::FloatRect next(pos.x, pos.y, tileSize, tileSize);

	// Sprawdzenie na mapie, czy docelowy prostok¹t koliduje ze œcian¹.
	// Zwraca true, jeœli NIE ma kolizji ze œcian¹ (czyli ruch jest mo¿liwy).
	return !map.isWallCollision(next);
}