#include "Tile.h"

Tile::Tile(TileType type, sf::Vector2f position, float size) 
	: type(type)
{
	shape.setSize({ size, size });
	shape.setPosition(position);

	if (type == TileType::Wall)
		shape.setFillColor(sf::Color(60, 60, 80));
	else if (type == TileType::Floor)
		shape.setFillColor(sf::Color(20, 20, 40));
	else if (type == TileType::Crystal)
		shape.setFillColor(sf::Color(0, 200, 255));
}

void Tile::draw(sf::RenderWindow& window) {
	if (type != TileType::Crystal || !collected)
		window.draw(shape);
}

TileType Tile::getType() const
{
	return type;
}

bool Tile::isCollected() const
{
	return collected;
}

void Tile::collect()
{
	collected = true;
}
sf::FloatRect Tile::getBounds() const {
	return shape.getGlobalBounds();
}