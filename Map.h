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
	void draw(sf::RenderWindow& window);

	const std::vector<Tile>& getTiles() const;
	bool isWallAt(float x, float y) const;

	bool isWallCollision(sf::FloatRect bounds) const;
	bool collectCrystalAt(sf::FloatRect bounds);
	int getScore() const;
	
	float getTileSize() const { return tileSize; }
};

