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
	// fallback shape used when textures are not available
	sf::RectangleShape shape;
	// sprite used when texture for this tile type is available
	sf::Sprite sprite;
	bool collected = false;
	bool useTexture = false;
	float tileSize = 0.f;

public:
	Tile(TileType type, sf::Vector2f position, float size);

	void draw(sf::RenderWindow& window, sf::Vector2f offset = {0.f, 0.f});
	TileType getType() const;
	bool isCollected() const;
	void collect();
	sf::FloatRect getBounds() const;
};

