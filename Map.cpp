#include "Map.h"

Map::Map()
{
	background.setSize({ 800, 600 });
	background.setFillColor(sf::Color(10, 10, 30));
}

void Map::draw(sf::RenderWindow& window) {
	window.draw(background);
}