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
	sf::Sprite sprite;
	bool collected = false;
	bool useTexture = false; // Flaga okreœlaj¹ca, czy u¿ywaæ tekstury (sprite) czy kszta³tu (shape).
	float tileSize = 0.f; // Rozmiar kafelka w pikselach.

public:
	Tile(TileType type, sf::Vector2f position, float size);
	void draw(sf::RenderWindow& window, sf::Vector2f offset = { 0.f, 0.f });

	TileType getType() const;
	bool isCollected() const;
	void collect(); // Oznacza kafelek jako zebrany.
	sf::FloatRect getBounds() const;
};