#pragma once // Zapewnia, ¿e ten plik nag³ówkowy zostanie do³¹czony tylko raz.
// Do³¹czenie nag³ówka klasy bazowej Enemy (Wróg).
#include "Enemy.h"
// Do³¹czenie standardowej biblioteki dla generowania liczb pseudolosowych.
#include <random>

// Klasa Random (Losowy) dziedziczy publicznie po klasie Enemy.
// Reprezentuje wroga, który porusza siê w sposób przypadkowy.
class Random : public Enemy {
public:
	// Konstruktor.
	Random(const std::string& texPath, sf::Vector2f startPos, float spd);
	// Implementacja logiki AI (w tym przypadku - losowego wyboru kierunku ruchu).
	void update(float dt, Map* map = nullptr, std::optional<sf::Vector2f> playerPos = std::nullopt) override;
private:
	// Statyczny generator liczb pseudolosowych (np. Mersenne Twister),
	// wspó³dzielony przez wszystkie instancje klasy Random.
	static std::mt19937 rng;
};