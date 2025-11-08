#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Tile.h"
class Map
{
	std::vector<Tile> tiles;
	int width = 0;
	int height = 0;
	float tileSize = 50.f;
	int score = 0;


public:
	Map() = default;
	bool loadFromFile(const std::string& filename);
	// draw with optional offset (pixels) to position map inside window
	void draw(sf::RenderWindow& window, sf::Vector2f offset = {0.f, 0.f});

	const std::vector<Tile>& getTiles() const;
	bool isWallAt(float x, float y) const;

	bool isWallCollision(sf::FloatRect bounds) const;
	bool collectCrystalAt(sf::FloatRect bounds);
	int getScore() const;
	
	// Return tile size in pixels
	float getTileSize() const { return tileSize; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }
};

