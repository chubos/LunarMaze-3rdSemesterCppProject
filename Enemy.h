// Zapewnia, ¿e ten plik nag³ówkowy zostanie do³¹czony tylko raz.
#pragma once
// Do³¹czenie nag³ówka klasy bazowej Character (Postaæ).
#include "Character.h"
// Do³¹czenie nag³ówka klasy Map (Mapa), u¿ywanej w metodach.
#include "Map.h"

// Klasa Enemy (Wróg) dziedziczy po klasie Character.
// Jest to klasa bazowa dla wszystkich wrogów (np. duchów).
class Enemy : public Character
{
protected:
	sf::Vector2f direction; // Wektor kierunku ruchu.
	float changeDirTimer = 0.f; // Licznik czasu do zmiany kierunku.
	float changeDirInterval = 2.5f; // Interwa³, co ile ma nastêpowaæ zmiana kierunku.

public:
	// Konstruktor.
	Enemy(const std::string& texPath, sf::Vector2f startPos, float spd);
	// Domyœlny wirtualny destruktor.
	virtual ~Enemy() = default;

	// Czysto wirtualna implementacja metody update (wymaga implementacji w klasach pochodnych),
	// w której klasy pochodne (np. ró¿ne typy duchów) implementuj¹ swoj¹ logikê AI.
	void update(float dt, Map* map = nullptr, std::optional<sf::Vector2f> playerPos = std::nullopt) override = 0;
	// Przes³oniêcie metody draw; po prostu wywo³uje metodê rysowania z klasy bazowej Character.
	void draw(sf::RenderWindow& win, sf::Vector2f offset = { 0.f, 0.f }) override { Character::draw(win, offset); }

protected:
	// Metoda pomocnicza dla klas pochodnych do sprawdzania kolizji ruchu
	// (czy postaæ mo¿e siê ruszyæ na dan¹ pozycjê).
	bool canMoveTo(const Map& map, sf::Vector2f pos);
};