#pragma once // Zapewnia, ¿e ten plik nag³ówkowy zostanie do³¹czony tylko raz.
// Do³¹czenie nag³ówka klasy bazowej Enemy (Wróg).
#include "Enemy.h"
// Do³¹czenie standardowego kontenera std::map.
#include <map>

// Klasa Learner (Ucz¹cy siê) dziedziczy publicznie po klasie Enemy.
// Reprezentuje wroga z bardziej zaawansowanym AI (np. uczeniem siê unikania czêsto odwiedzanych miejsc).
class Learner : public Enemy {
public:
	// Konstruktor.
	Learner(const std::string& texPath, sf::Vector2f startPos, float spd);
	// Implementacja logiki AI (np. unikanie miejsc z wysok¹ 'map¹ ciep³a').
	void update(float dt, Map* map = nullptr, std::optional<sf::Vector2f> playerPos = std::nullopt) override;

private:
	// Mapa ciep³a odwiedzin: przechowuje liczbê odwiedzin dla ka¿dego kafelka (para int,int).
	std::map<std::pair<int, int>, int> visitHeatmap;
};