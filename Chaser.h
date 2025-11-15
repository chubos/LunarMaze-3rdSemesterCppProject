// Zapewnia, ¿e ten plik nag³ówkowy zostanie do³¹czony tylko raz.
#pragma once
// Do³¹czenie nag³ówka klasy bazowej Enemy (Wróg).
#include "Enemy.h"

// Klasa Chaser (Goni¹cy) dziedziczy publicznie po klasie Enemy.
class Chaser : public Enemy {
public:
	// Konstruktor: przyjmuje œcie¿kê do tekstury, pozycjê startow¹ i prêdkoœæ.
	Chaser(const std::string& texPath, sf::Vector2f startPos, float spd);
	// Implementacja wirtualnej metody update odziedziczonej z klasy bazowej,
	// zawiera logikê ruchu i zachowania Chaser'a (np. gonienie gracza).
	void update(float dt, Map* map = nullptr, std::optional<sf::Vector2f> playerPos = std::nullopt) override;
};