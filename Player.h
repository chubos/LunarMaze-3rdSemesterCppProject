#pragma once // Zapewnia, ¿e ten plik nag³ówkowy zostanie do³¹czony tylko raz.
// Do³¹czenie nag³ówka klasy bazowej Character (Postaæ).
#include "Character.h"
// Do³¹czenie nag³ówka klasy Map (Mapa).
#include "Map.h"
// Umo¿liwia u¿ycie wartoœci opcjonalnych (std::optional).
#include <optional>

// Klasa Player (Gracz) dziedziczy publicznie po klasie Character.
class Player : public Character
{
	sf::Vector2f velocity; // Wektor prêdkoœci gracza (u¿ywany do ruchu opartego na fizyce/kierunku).
public:
	// Konstruktor.
	Player(const std::string& texturePath, sf::Vector2f startPosition, float speed);

	// Obs³uguje wejœcie u¿ytkownika (np. klawisze WASD).
	void handleInput();
	// Aktualizuje stan gracza (ruch, kolizje, zbieranie przedmiotów).
	// Parametry map i playerPos s¹ ignorowane, poniewa¿ Player nie potrzebuje swojej w³asnej pozycji
	// ani pozycji innego gracza.
	void update(float dt, Map* map = nullptr, std::optional<sf::Vector2f> playerPos = std::nullopt) override;
};