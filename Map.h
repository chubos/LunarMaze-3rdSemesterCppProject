#pragma once
#include <SFML/Graphics.hpp>
class Map
{
	sf::RectangleShape background;

public:
	Map();
	void draw(sf::RenderWindow& window);
};

