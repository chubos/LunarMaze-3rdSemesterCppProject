#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Tile.h"

class Map
{
	std::vector<Tile> tiles; // Wektor przechowuj¹cy wszystkie kafelki mapy (jednowymiarowy).
	int width = 0; // Szerokoœæ mapy w kafelkach.
	int height = 0; // Wysokoœæ mapy w kafelkach.
	float tileSize = 50.f; // Rozmiar pojedynczego kafelka w pikselach.
	int score = 0; // Aktualny wynik po zebraniu kryszta³ów.


public:
	Map() = default;
	bool loadFromFile(const std::string& filename);// Wczytuje dane mapy z pliku.
	void draw(sf::RenderWindow& window, sf::Vector2f offset = { 0.f, 0.f }); // Rysuje mapê na oknie, opcjonalnie z przesuniêciem (offset).

	const std::vector<Tile>& getTiles() const; // Zwraca referencjê do wektora kafelków.
	bool isWallAt(float x, float y) const; // Sprawdza, czy w danej globalnej pozycji (x, y) znajduje siê œciana.
	bool isWallCollision(sf::FloatRect bounds) const; // Sprawdza, czy podany prostok¹t kolizyjny (granice postaci) koliduje z jak¹kolwiek œcian¹ na mapie.

	bool collectCrystalAt(sf::FloatRect bounds); // Sprawdza, czy w danym obszarze znajduje siê kryszta³ do zebrania, jeœli tak, zbiera go.
	int getScore() const; //Zwraca aktualny wynik.

	float getTileSize() const { return tileSize; } // Zwraca rozmiar kafelka w pikselach.
	int getWidth() const { return width; } // Zwraca szerokoœæ mapy w kafelkach.
	int getHeight() const { return height; } // Zwraca wysokoœæ mapy w kafelkach.
};