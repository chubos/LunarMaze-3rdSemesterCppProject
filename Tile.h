#pragma once
#include <SFML/Graphics.hpp>

enum class TileType {
	Wall,
	Floor,
	Crystal
};


class Tile
{
	TileType type;
	sf::RectangleShape shape;
	bool collected = false;

public:
	Tile(TileType type, sf::Vector2f position, float size);

	void draw(sf::RenderWindow& window);
	TileType getType() const;
	bool isCollected() const;
	void collect();
	sf::FloatRect getBounds() const;
};

