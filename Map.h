#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Tile.h"

class Map
{
	std::vector<Tile> tiles; // Wektor przechowuj¹cy wszystkie kafelki mapy.
	int width = 0; // Szerokoœæ mapy w kafelkach.
	int height = 0; // Wysokoœæ mapy w kafelkach.
	float tileSize = 50.f; // Rozmiar pojedynczego kafelka w pikselach.
	int score = 0; // Wynik.


public:
	Map() = default;
	bool loadFromFile(const std::string& filename);
	void draw(sf::RenderWindow& window, sf::Vector2f offset = { 0.f, 0.f });

	const std::vector<Tile>& getTiles() const;
	bool isWallAt(float x, float y) const;
	bool isWallCollision(sf::FloatRect bounds) const;

	bool collectCrystalAt(sf::FloatRect bounds);
	int getScore() const;

	float getTileSize() const { return tileSize; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }
};